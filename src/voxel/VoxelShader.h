#pragma once

#include "Shader.h"

class VoxelShader : public Shader {
public:
    VoxelShader() {
        programName = "voxel";
        uniforms.add(UniformAttribute("MVP"));
    }
};
