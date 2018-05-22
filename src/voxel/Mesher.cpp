#include <vector>
#include <algorithm>
#include <iterator>
#include "Mesher.h"
#include "Chunk.h"
#include "Material.h"
#include "Mesh.h"
#include <functional>

static Mask frontMask;
static Mask backMask;

void getAO(bool front, int i, int j, int k, int d, VoxelChunk *chunk, VoxelBSP *bsp, int *ao0, int *ao1, int *ao2, int *ao3);

bool Mesher::stopMerge(MaskValue & c, MaskValue & next) {
    return next.v != c.v || next.has_ao() || next.lighting != c.lighting || next.color != c.color;
}

void Mesher::copyQuads(Mask& mask, VoxelGeometry *geometry, int x, int y, int w, int h, int ao0, int ao1, int ao2, int ao3, int l, glm::vec3 color) {
    bool front = mask.front;
    float light_strength = 0.6f;
    auto vertices = geometry->positions;
    auto colors = geometry->colors;
    auto lightings = geometry->lightings;
    auto indices = geometry->indices;
    int i = mask.i;
    int d = mask.d;
    int index = geometry->positions->size() / 3;

    Coord3 v0 = Coord3(i, x, y).rotate(d);
    Coord3 v1 = Coord3(i, x + w, y).rotate(d);
    Coord3 v2 = Coord3(i, x + w, y + h).rotate(d);
    Coord3 v3 = Coord3(i, x, y + h).rotate(d);

    vertices->push_back(v0.i, v0.j, v0.k);
    vertices->push_back(v1.i, v1.j, v1.k);
    vertices->push_back(v2.i, v2.j, v2.k);
    vertices->push_back(v3.i, v3.j, v3.k);

	colors->push_back(color.r, color.g, color.b);
	colors->push_back(color.r, color.g, color.b);
	colors->push_back(color.r, color.g, color.b);
	colors->push_back(color.r, color.g, color.b);

    lightings->push_back(getLight(ao0), getLight(ao1), getLight(ao2), getLight(ao3));

	bool flippedQuad = ao0 + ao2 > ao1 + ao3;

    if (front) {
		if (flippedQuad) {
			indices->push_back(index);
			indices->push_back(index + 1);
			indices->push_back(index + 3);
			indices->push_back(index + 1);
			indices->push_back(index + 2);
			indices->push_back(index + 3);
		}
		else {
			indices->push_back(index);
			indices->push_back(index + 1);
			indices->push_back(index + 2);
			indices->push_back(index + 2);
			indices->push_back(index + 3);
			indices->push_back(index);
		}
    }
	else {
		if (flippedQuad) {
			indices->push_back(index + 3);
			indices->push_back(index + 1);
			indices->push_back(index);
			indices->push_back(index + 3);
			indices->push_back(index + 2);
			indices->push_back(index + 1);
		}
		else {
			indices->push_back(index + 2);
			indices->push_back(index + 1);
			indices->push_back(index);
			indices->push_back(index);
			indices->push_back(index + 3);
			indices->push_back(index + 2);
		}
    }
}

void Mesher::copyQuads(Mask& mask, int size, VoxelGeometry *geometry) {
    int n = 0;
    MaskValue c;
    int w, h;
    auto& data = mask.data;

    for (int j = 0; j < size; j++) {
        for (int i = 0; i < size; ) {
            c = data[n];

            if (c.v == 0) {
                i++;
                n++;
                continue;
            }

            // Check AO
            if (c.has_ao()) {
                copyQuads(mask, geometry, j, i, 1, 1, c.ao0, c.ao1, c.ao2, c.ao3, c.lighting, c.color);
                i++;
                n++;
                continue;
            }

            int lighting = c.lighting;

            // Compute width
            for (w = 1; i + w < size; ++w) {
                MaskValue &next = data[n + w];
                if (stopMerge(c, next)) {
                    break;
                }
            }

            // Compute height
            bool done = false;
            for (h = 1; j + h < size; h++) {
                for (int k = 0; k < w; k++) {
                    MaskValue &next = data[n + k + h * size];
                    if (stopMerge(c, next)) {
                        done = true;
                        break;
                    }
                }
                if (done) {
                    break;
                }
            }

            // Add Quad
            copyQuads(mask, geometry, j, i, h, w, 0, 0, 0, 0, lighting, c.color);

            //Zero-out mask
            for (int l = 0; l < h; l++) {
                for (int k = 0; k < w; k++) {
                    data[n + k + l * size] = 0;
                }
            }

            i += w; n += w;
        }
    }
}

bool getSolid(Coord3& coord, VoxelChunk *chunk, VoxelBSP *bsp) {
	int size = chunk->size;
	auto origin = chunk->origin;
	Coord3 offset = Coord3(origin.x, origin.y, origin.z);
	if (coord.i < 0 || coord.i >= size ||
		coord.j < 0 || coord.j >= size ||
		coord.k < 0 || coord.k >= size) {
		Coord3 chunksCoord = coord + offset;
		int v = bsp->get(chunksCoord.i, chunksCoord.j, chunksCoord.k);
		return v > 0;
	}

	if (chunk == 0) {
		return false;
	}

	int v = chunk->getLocal(coord.i, coord.j, coord.k);
	return v > 0;
}

VoxelGeometry * Mesher::mesh(VoxelChunk *chunk, VoxelBSP *bsp, getColorFuncType getColor)
{
    VoxelGeometry *geometry = new VoxelGeometry();
	int size = chunk->size;

    for (int d = 0; d < 3; d++) {
        for (int i = 0; i <= size; i++) {
			frontMask.front = true;
			frontMask.i = i;
			frontMask.d = d;
			backMask.front = false;
			backMask.i = i;
			backMask.d = d;

            for (int j = 0; j < size; j++) {
                for (int k = 0; k < size; k++) {
                    Coord3 coord_a = Coord3(i - 1, j, k).rotate(d);
                    bool a = getSolid(coord_a, chunk, bsp);

                    Coord3 coord_b = Coord3(i, j, k).rotate(d);
					bool b = getSolid(coord_b, chunk, bsp);

					bool front = a;

                    if (a == b) {
                        continue;
                    }

                    if (i == 0 && front) {
                        continue;
                    }

                    if (i == size && !front) {
                        continue;
                    }

					int ao0, ao1, ao2, ao3;
					getAO(front, i, j, k, d, chunk, bsp, &ao0, &ao1, &ao2, &ao3);

                    int light_amount = 15;

                    MaskValue v = MaskValue(1, ao0, ao1, ao2, ao3, light_amount);

					Coord3& coordColor = front ? coord_a : coord_b;
                    v.color = getColor(coordColor);

                    if (front) {
						frontMask.set(j, k, v);
                    }
                    else {
                        backMask.set(j, k, v);
                    }
                }
            }

			copyQuads(frontMask, size, geometry);
			copyQuads(backMask, size, geometry);

			frontMask.clear();
			backMask.clear();
		}
    }

    return geometry;
}

int Mesher::getLight(int ao) {
//    float ao_strength = 0.1f;
    float ao_strength = 0.1f;
	float lightFloat = 1 - (ao / 3.0f * ao_strength);
    int light = floor(lightFloat * 15);
    return light;
}

int getAO(bool s1, bool s2, bool c) {
	if (s1 && s2) {
		return 3;
	}

	int num = 0;
	if (s1) {
		num += 1;
	}
	if (s2) {
		num += 1;
	}
	if (c) {
		num += 1;
	}
	return num;
}

void getAO(bool front, int i, int j, int k, int d, VoxelChunk *chunk, VoxelBSP *bsp, int *ao0, int *ao1, int *ao2, int *ao3) {
	int aoI = front ? i : i - 1;

	Coord3 c00 = Coord3(aoI, j - 1, k - 1).rotate(d);
	Coord3 c01 = Coord3(aoI, j, k - 1).rotate(d);
	Coord3 c02 = Coord3(aoI, j + 1, k - 1).rotate(d);
	Coord3 c10 = Coord3(aoI, j - 1, k).rotate(d);
	Coord3 c12 = Coord3(aoI, j + 1, k).rotate(d);
	Coord3 c20 = Coord3(aoI, j - 1, k + 1).rotate(d);
	Coord3 c21 = Coord3(aoI, j, k + 1).rotate(d);
	Coord3 c22 = Coord3(aoI, j + 1, k + 1).rotate(d);

	bool s00 = getSolid(c00, chunk, bsp);
	bool s01 = getSolid(c01, chunk, bsp);
	bool s02 = getSolid(c02, chunk, bsp);
	bool s10 = getSolid(c10, chunk, bsp);
	bool s12 = getSolid(c12, chunk, bsp);
	bool s20 = getSolid(c20, chunk, bsp);
	bool s21 = getSolid(c21, chunk, bsp);
	bool s22 = getSolid(c22, chunk, bsp);

	*ao0 = getAO(s10, s01, s00);
	*ao1 = getAO(s01, s12, s02);
	*ao2 = getAO(s12, s21, s22);
	*ao3 = getAO(s21, s10, s20);
}