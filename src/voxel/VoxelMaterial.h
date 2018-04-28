#pragma once

#include "Material.h"

class VoxelMaterial : public Material {
	GLuint MatrixID;

	//void load() {
	//	programID = LoadShaders("shaders/voxel.vertexshader", "shaders/voxel.fragmentshader");
	//	MatrixID = glGetUniformLocation(programID, "MVP");
	//}

	//void bind_uniforms() override {
	//	glm::mat4 MVP = current_camera->Projection * current_camera->View * current_node->matrix;
	//	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	//}
};