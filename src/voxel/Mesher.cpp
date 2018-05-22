#include <vector>
#include <algorithm>
#include <iterator>
#include "Mesher.h"
#include "Chunk.h"
#include "Material.h"
#include "Mesh.h"
#include <functional>

bool Mesher::stop_merge(MaskValue & c, MaskValue & next) {
    return next.v != c.v || next.has_ao() || next.lighting != c.lighting || next.color != c.color;
}

void Mesher::copy_quads(Mask& mask, VoxelGeometry *geometry, int x, int y, int w, int h, int ao0, int ao1, int ao2, int ao3, int l, glm::vec3 color) {
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

void Mesher::copy_quads(Mask& mask, VoxelGeometry *geometry) {
    int n = 0;
    MaskValue c;
    int w, h;
    auto& data = mask.data;

    for (int j = 0; j < CHUNK_SIZE; j++) {
        for (int i = 0; i < CHUNK_SIZE; ) {
            c = data[n];

            if (c.v == 0) {
                i++;
                n++;
                continue;
            }

            // Check AO
            if (c.has_ao()) {
                copy_quads(mask, geometry, j, i, 1, 1, c.ao0, c.ao1, c.ao2, c.ao3, c.lighting, c.color);
                i++;
                n++;
                continue;
            }

            int lighting = c.lighting;

            // Compute width
            for (w = 1; i + w < CHUNK_SIZE; ++w) {
                MaskValue &next = data[n + w];
                if (stop_merge(c, next)) {
                    break;
                }
            }

            // Compute height
            bool done = false;
            for (h = 1; j + h < CHUNK_SIZE; h++) {
                for (int k = 0; k < w; k++) {
                    MaskValue &next = data[n + k + h * CHUNK_SIZE];
                    if (stop_merge(c, next)) {
                        done = true;
                        break;
                    }
                }
                if (done) {
                    break;
                }
            }

            // Add Quad
            copy_quads(mask, geometry, j, i, h, w, 0, 0, 0, 0, lighting, c.color);

            //Zero-out mask
            for (int l = 0; l < h; l++) {
                for (int k = 0; k < w; k++) {
                    data[n + k + l * CHUNK_SIZE] = 0;
                }
            }

            i += w; n += w;
        }
    }
}

VoxelGeometry * Mesher::mesh(getSolidFuncType getSolid, getColorFuncType getColor)
{
    VoxelGeometry *geometry = new VoxelGeometry();

    for (int d = 0; d < 3; d++) {
        for (int i = 0; i <= CHUNK_SIZE; i++) {
            Mask *front_mask = new Mask(i, d, true);
            Mask *back_mask = new Mask(i, d, false);
            for (int j = 0; j < CHUNK_SIZE; j++) {
                for (int k = 0; k < CHUNK_SIZE; k++) {
                    Coord3 coord_a = Coord3(i - 1, j, k).rotate(d);
                    bool a = getSolid(coord_a);

                    Coord3 coord_b = Coord3(i, j, k).rotate(d);
					bool b = getSolid(coord_b);

					bool front = a;

                    if (a == b) {
                        continue;
                    }

                    if (i == 0 && front) {
                        continue;
                    }

                    if (i == CHUNK_SIZE && !front) {
                        continue;
                    }

                    int aoI = front ? i : i - 1;

                    Coord3 c00 = Coord3(aoI, j - 1, k - 1).rotate(d);
                    Coord3 c01 = Coord3(aoI, j, k - 1).rotate(d);
                    Coord3 c02 = Coord3(aoI, j + 1, k - 1).rotate(d);
                    Coord3 c10 = Coord3(aoI, j - 1, k).rotate(d);
                    Coord3 c12 = Coord3(aoI, j + 1, k).rotate(d);
                    Coord3 c20 = Coord3(aoI, j - 1, k + 1).rotate(d);
                    Coord3 c21 = Coord3(aoI, j, k + 1).rotate(d);
                    Coord3 c22 = Coord3(aoI, j + 1, k + 1).rotate(d);

					bool s00 = getSolid(c00);
					bool s01 = getSolid(c01);
					bool s02 = getSolid(c02);
					bool s10 = getSolid(c10);
					bool s12 = getSolid(c12);
					bool s20 = getSolid(c20);
					bool s21 = getSolid(c21);
					bool s22 = getSolid(c22);

                    int light_amount = 15;

                    MaskValue v = MaskValue(1,
                        get_ao(s10, s01, s00), get_ao(s01, s12, s02), get_ao(s12, s21, s22), get_ao(s21, s10, s20),
                        light_amount);

					Coord3& coordColor = front ? coord_a : coord_b;
                    v.color = getColor(coordColor);

                    if (front) {
                        front_mask->set(j, k, v);
                    }
                    else {
                        back_mask->set(j, k, v);
                    }
                }
            }

			copy_quads(*front_mask, geometry);
			copy_quads(*back_mask, geometry);
			delete front_mask;
			delete back_mask;
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

int Mesher::get_ao(bool s1, bool s2, bool c) {
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
