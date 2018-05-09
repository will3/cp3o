#pragma once

#include "Chunk.h"
#include "Chunks.h"
#include "Mask.h"
#include "DirectionalLight.h"
#include "Mesh.h"
#include "VoxelGeometry.h"
#include <functional>
#include <glm/glm.hpp>

struct Voxel {
    bool solid;
    glm::vec3 color;
};

class Mesher {
private:
    static ao_type get_ao(int s1, int s2, int c);
    static bool stop_merge(MaskValue& c, MaskValue& next);
    static void copy_quads(Mask& mask, VoxelGeometry *geometry, int x, int y, int w, int h, int ao0, int ao1, int ao2, int ao3, int l, glm::vec3 color);
    static void copy_quads(Mask& mask, VoxelGeometry *geometry);
    static Voxel getVoxel(Coord3 coord, Chunk<Voxel> *chunk, Chunks<Voxel> *chunks);
    static int getLight(int ao);
public:
    static VoxelGeometry* mesh(Chunk<Voxel> *chunk, Chunks<Voxel> *chunks);
};

