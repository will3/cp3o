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
    glm::ivec3 color;
};

typedef std::function<Voxel(Coord3 coord)> getVoxelFuncType;

class Mesher {
private:
    static int get_ao(Voxel& s1, Voxel& s2, Voxel& c);
    static bool stop_merge(MaskValue& c, MaskValue& next);
    static void copy_quads(Mask& mask, VoxelGeometry *geometry, int x, int y, int w, int h, int ao0, int ao1, int ao2, int ao3, int l, glm::vec3 color);
    static void copy_quads(Mask& mask, VoxelGeometry *geometry);
    static int getLight(int ao);
public:
	static VoxelGeometry* mesh(getVoxelFuncType getVoxel);
    static VoxelGeometry* mesh(Chunk<Voxel> *chunk, Chunks<Voxel> *chunks);
};

