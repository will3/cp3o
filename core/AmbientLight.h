#pragma once
#include "GL/glew.h"
#include "Node.h"

class AmbientLight : public Node {
public:
	glm::vec3 color = glm::vec3(1.0, 1.0, 1.0);

	AmbientLight(glm::vec3 color) : color(color) {}
};