#pragma once

#include "Chunk.h"
#include "Chunks.h"
#include "Mask.h"
#include "DirectionalLight.h"
#include "Mesh.h"
#include "VoxelGeometry.h"
#include <functional>
#include <glm/glm.hpp>
#include "VoxelBSP.h"

typedef std::function<int(Coord3& coord)> getSolidFuncType;
typedef std::function<glm::ivec3(Coord3& coord)> getColorFuncType;

class Mesher {
private:
    static bool stop_merge(MaskValue& c, MaskValue& next);
    static void copy_quads(Mask& mask, VoxelGeometry *geometry, int x, int y, int w, int h, int ao0, int ao1, int ao2, int ao3, int l, glm::vec3 color);
    static void copy_quads(Mask& mask, VoxelGeometry *geometry);
    static int getLight(int ao);
public:
	static VoxelGeometry* mesh(VoxelChunk *chunk, VoxelBSP *bsp, getColorFuncType getColor);
};

