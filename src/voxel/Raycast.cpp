//#include "Raycast.h"
//
//RayCastResult Raycast::raycast(Chunks *chunks, float3 position, float3 direction, int max_d) {
//    return Raycast::raycast(chunks, position, direction, max_d, false);
//}
//
//RayCastResult Raycast::raycast(Chunks *chunks, float3 position, float3 direction, int max_d, bool ignore_first) {
//    return raycast(chunks, position, direction, max_d, ignore_first, 0);
//}
//
//RayCastResult Raycast::raycast(Chunks *chunks, float3 position, float3 direction, int max_d, bool ignore_first, int max_y) {
//    // unit vector
//    float length = sqrt(direction.x * direction.x + direction.y* direction.y + direction.z * direction.z);
//    direction = direction / length;
//
//    RayCastResult result = RayCastResult();
//    float t = 0.0;
//    float px = position.x;
//    float py = position.y;
//    float pz = position.z;
//    float dx = direction.x;
//    float dy = direction.y;
//    float dz = direction.z;
//
//    int ix = floor(px);
//    int iy = floor(py);
//    int iz = floor(pz);
//
//    float stepx = (dx > 0) ? 1 : -1;
//    float stepy = (dy > 0) ? 1 : -1;
//    float stepz = (dz > 0) ? 1 : -1;
//
//    // dx,dy,dz are already normalized
//    float txDelta = abs(1 / dx);
//    float tyDelta = abs(1 / dy);
//    float tzDelta = abs(1 / dz);
//
//    float xdist = (stepx > 0) ? (ix + 1 - px) : (px - ix);
//    float ydist = (stepy > 0) ? (iy + 1 - py) : (py - iy);
//    float zdist = (stepz > 0) ? (iz + 1 - pz) : (pz - iz);
//
//    // location of nearest voxel boundary, in units of t 
//    float txMax = (txDelta < INFINITY) ? txDelta * xdist : INFINITY;
//    float tyMax = (tyDelta < INFINITY) ? tyDelta * ydist : INFINITY;
//    float tzMax = (tzDelta < INFINITY) ? tzDelta * zdist : INFINITY;
//
//    float steppedIndex = -1;
//
//    bool first = true;
//    // main loop along raycast vector
//    while (t <= max_d) {
//        if (max_y != 0 && iy >= max_y) {
//            break;
//        }
//
//        // exit check
//        Coord3 coord = Coord3(ix, iy, iz);
//
//        int b = chunks->get(coord);
//
//        if (b) {
//            if (ignore_first && first) {
//                first = false;
//            }
//            else {
//                result.hit_pos[0] = px + t * dx;
//                result.hit_pos[1] = py + t * dy;
//                result.hit_pos[2] = pz + t * dz;
//
//                result.hit_norm[0] = result.hit_norm[1] = result.hit_norm[2] = 0;
//                if (steppedIndex == 0) result.hit_norm[0] = -stepx;
//                if (steppedIndex == 1) result.hit_norm[1] = -stepy;
//                if (steppedIndex == 2) result.hit_norm[2] = -stepz;
//
//                result.v = b;
//                return result;
//            }
//        }
//
//        // advance t to next nearest voxel boundary
//        if (txMax < tyMax) {
//            if (txMax < tzMax) {
//                ix += stepx;
//                t = txMax;
//                txMax += txDelta;
//                steppedIndex = 0;
//            }
//            else {
//                iz += stepz;
//                t = tzMax;
//                tzMax += tzDelta;
//                steppedIndex = 2;
//            }
//        }
//        else {
//            if (tyMax < tzMax) {
//                iy += stepy;
//                t = tyMax;
//                tyMax += tyDelta;
//                steppedIndex = 1;
//            }
//            else {
//                iz += stepz;
//                t = tzMax;
//                tzMax += tzDelta;
//                steppedIndex = 2;
//            }
//        }
//    }
//
//    // no voxel hit found
//    result.hit_pos[0] = px + t * dx;
//    result.hit_pos[1] = py + t * dy;
//    result.hit_pos[2] = pz + t * dz;
//    result.hit_norm[0] = result.hit_norm[1] = result.hit_norm[2] = 0;
//    result.v = 0;
//
//    return result;
//}

