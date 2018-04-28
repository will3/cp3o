//#include "VoxelShadowMap.h"
//#include "Raycast.h"
//
//bool calc_light(DirectionalLight *light, Chunks *chunks, Coord3 coord) {
//    float3 position = float3(coord.i + 0.5f, coord.j + 0.5f, coord.k + 0.5f);
//    float3 direction = float3(light->inverse_direction[0], light->inverse_direction[1], light->inverse_direction[2]);
//    bool hit = Raycast::raycast(chunks, position, direction, 100, true).v > 0;
//
//    if (hit) {
//        return false;
//    }
//
//    return true;
//}
//
//int calc_light(DirectionalLight *light, Chunk *chunk, Chunks *chunks, int d, int i, int j, int k, bool front) {
//    // facing light
//    bool no_light = light->inverse_direction[d] == 0 || (light->inverse_direction[d] > 0 != front);
//    if (no_light) {
//        return 0;
//    }
//
//    int u = (d + 1) % 3;
//    int v = (d + 2) % 3;
//
//    Coord3 chunks_coord = Coord3(i, j, k).rotate(d) + chunk->get_offset();
//    float3 position = float3(chunks_coord.i, chunks_coord.j, chunks_coord.k);
//    position[u] += 0.5f;
//    position[v] += 0.5f;
//
//    float3 direction = float3(light->inverse_direction[0], light->inverse_direction[1], light->inverse_direction[2]);
//    bool hit = Raycast::raycast(chunks, position, direction, 100, true, 128).v > 0;
//
//    if (hit) {
//        return 0;
//    }
//
//    return max_light_value;
//}
//
//void VoxelShadowMap::calc_shadow(Chunk * chunk, DirectionalLight *light)
//{
//    for (int d = 0; d < 3; d++) {
//        for (int i = 0; i <= CHUNK_SIZE; i++) {
//            for (int j = 0; j < CHUNK_SIZE; j++) {
//                for (int k = 0; k < CHUNK_SIZE; k++) {
//                    Coord3 coord_front = Coord3(i - 1, j, k).rotate(d);
//                    int a = chunk->get_global(coord_front);
//
//                    Coord3 coord_back = Coord3(i, j, k).rotate(d);
//                    int b = chunk->get_global(coord_back);
//
//                    if ((a == 0 && b == 0) || (a != 0 && b != 0))
//                    {
//                        continue;
//                    }
//
//                    bool front = a != 0;
//
//                    if (i == 0 && front) {
//                        continue;
//                    }
//
//                    if (i == CHUNK_SIZE && !front) {
//                        continue;
//                    }
//
//                    int amount = calc_light(light, chunk, chunk->chunks, d, i, j, k, front);
//
//                    Coord3 coord = front ? coord_front : coord_back;
//                    set(coord, d, front, amount);
//                }
//            }
//        }
//    }
//}
//
//void VoxelShadowMap::set(Coord3 coord, int d, int front, int v) {
//    total[coord] += v;
//    count[coord] += 1.0;
//}
//
//int VoxelShadowMap::get(Coord3 coord) {
//    return total[coord] / count[coord];
//}

