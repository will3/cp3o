#pragma once

#include "GL/glew.h"
#include "Node.h"
#include "glm\glm.hpp"

class DirectionalLight : public Node
{
public:
	glm::vec3 direction;
	glm::vec3 inverse_direction;
	glm::vec3 position;
	float intensity = 1.0;

	DirectionalLight(float intensity) : intensity(intensity) { };

	~DirectionalLight() {};

	void setPosition(glm::vec3 position) {
		this->position = position;

		updateDirection();
	}

	void updateDirection() {
		inverse_direction = glm::normalize(position);
		direction = inverse_direction * -1.0f;
	}
};

