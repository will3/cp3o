#pragma once

#include "Shader.h"

class DepthShader : public Shader {
public:
	DepthShader() {
		programName = "depth";
		uniforms.add(UniformAttribute("MVP"));
	}
};