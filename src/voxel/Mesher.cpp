#include <vector>
#include <algorithm>
#include <iterator>
#include "Mesher.h"
#include "Chunk.h"
#include "Material.h"
#include "Mesh.h"
#include <functional>

ao_type Mesher::get_ao(int s1, int s2, int c) {
    if (s1 && s2) {
        return 3;
    }

    return s1 + s2 + c;
}

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

    colors->push_back((int)(color.r * 255), (int)(color.g * 255), (int)(color.b * 255));
    colors->push_back((int)(color.r * 255), (int)(color.g * 255), (int)(color.b * 255));
    colors->push_back((int)(color.r * 255), (int)(color.g * 255), (int)(color.b * 255));
    colors->push_back((int)(color.r * 255), (int)(color.g * 255), (int)(color.b * 255));

    lightings->push_back(getLight(ao0), getLight(ao1), getLight(ao2), getLight(ao3));

    if (front) {
        indices->push_back(index);
        indices->push_back(index + 1);
        indices->push_back(index + 2);
        indices->push_back(index + 2);
        indices->push_back(index + 3);
        indices->push_back(index);
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

void Mesher::copy_quads(Mask& mask, VoxelGeometry *geometry) {
    int n = 0;
    MaskValue c;
    int w, h;
    auto data = mask.data;

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

VoxelGeometry* Mesher::mesh(Chunk<Voxel> *chunk, Chunks<Voxel> *chunks) {
    VoxelGeometry *geometry = new VoxelGeometry();

    std::vector<Mask *> masks;
    for (int d = 0; d < 3; d++) {
        for (int i = 0; i <= CHUNK_SIZE; i++) {
            Mask *front_mask = new Mask(i, d, true);
            Mask *back_mask = new Mask(i, d, false);
            for (int j = 0; j < CHUNK_SIZE; j++) {
                for (int k = 0; k < CHUNK_SIZE; k++) {
                    Coord3 coord_a = Coord3(i - 1, j, k).rotate(d);
                    Voxel a = getVoxel(coord_a, chunk, chunks);

                    Coord3 coord_b = Coord3(i, j, k).rotate(d);
                    Voxel b = getVoxel(coord_b, chunk, chunks);

                    bool front = a.solid;

                    if (a.solid == b.solid) {
                        continue;
                    }

                    if (i == 0 && front) {
                        continue;
                    }

                    if (i == CHUNK_SIZE && !front) {
                        continue;
                    }

//                    int aoX = front ? 1 : -1;
                    int aoX = front ? 0 : -1;

                    Coord3 c00 = Coord3(i + aoX, j - 1, k - 1).rotate(d);
                    Coord3 c01 = Coord3(i + aoX, j, k - 1).rotate(d);
                    Coord3 c02 = Coord3(i + aoX, j + 1, k - 1).rotate(d);
                    Coord3 c10 = Coord3(i + aoX, j - 1, k).rotate(d);
                    Coord3 c12 = Coord3(i + aoX, j + 1, k).rotate(d);
                    Coord3 c20 = Coord3(i + aoX, j - 1, k + 1).rotate(d);
                    Coord3 c21 = Coord3(i + aoX, j, k + 1).rotate(d);
                    Coord3 c22 = Coord3(i + aoX, j + 1, k + 1).rotate(d);

                    int s00 = getVoxel(c00, chunk, chunks).solid ? 1 : 0;
                    int s01 = getVoxel(c01, chunk, chunks).solid ? 1 : 0;
                    int s02 = getVoxel(c02, chunk, chunks).solid ? 1 : 0;
                    int s10 = getVoxel(c10, chunk, chunks).solid ? 1 : 0;
                    int s12 = getVoxel(c12, chunk, chunks).solid ? 1 : 0;
                    int s20 = getVoxel(c20, chunk, chunks).solid ? 1 : 0;
                    int s21 = getVoxel(c21, chunk, chunks).solid ? 1 : 0;
                    int s22 = getVoxel(c22, chunk, chunks).solid ? 1 : 0;

//                    Coord3 coord = front ? coord_a : coord_b;
//                    int light_amount = chunk->get_light(coord);
                    int light_amount = 15;

                    MaskValue v = MaskValue(1,
                        get_ao(s10, s01, s00), get_ao(s01, s12, s02), get_ao(s12, s21, s22), get_ao(s21, s10, s20),
                        light_amount);

                    Voxel voxel = front ? a : b;
                    v.color = voxel.color;

                    if (front) {
                        front_mask->set(j, k, v);
                    }
                    else {
                        back_mask->set(j, k, v);
                    }
                }
            }

            masks.push_back(front_mask);
            masks.push_back(back_mask);
        }
    }

    for (Mask *mask : masks) {
        copy_quads(*mask, geometry);
    }

    for (Mask *mask : masks) {
        delete mask;
    }

//    chunk->position = glm::vec3(chunk->get_offset().i, chunk->get_offset().j, chunk->get_offset().k);

    return geometry;
}

Voxel Mesher::getVoxel(Coord3 coord, Chunk<Voxel> *chunk, Chunks<Voxel> *chunks) {
    if (coord.i < 0 || coord.i >= CHUNK_SIZE ||
        coord.j < 0 || coord.j >= CHUNK_SIZE ||
        coord.k < 0 || coord.k >= CHUNK_SIZE) {
        Coord3 chunksCoord = coord + chunk->get_offset();
        return chunks->get(chunksCoord);
    }

    return chunk->get(coord);
}

int Mesher::getLight(int ao) {
//    float ao_strength = 0.1f;
    float ao_strength = 1.0f;
    float lightFloat = (1.0f - (ao / 3.0f * ao_strength));
    int light = floor(lightFloat * 16);
    return light;
}
