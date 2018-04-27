#pragma once

#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "types.h"

class Camera {
public:
	virtual void update_projection_matrix() {};
	void update_view_matrix() {
		View = glm::lookAt(position, target, up);
	};
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 target = glm::vec3(0, 0, 1);
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::mat4 Projection;
	glm::mat4 View;
};

class PerspectiveCamera : public Camera {
public:
	float fov;
	float ratio;
	float near;
	float far;

	PerspectiveCamera(float fov, float ratio, float near, float far) : fov(fov), ratio(ratio), near(near), far(far) {
		update_projection_matrix();
	};

	void update_projection_matrix() override {
		Projection = glm::perspective(glm::radians(fov), ratio, near, far);
	}
};

class OrthographicCamera : public Camera {
public:
	float left;
	float right;
	float bottom;
	float top;
	float near;
	float far;

	OrthographicCamera(float left, float right, float bottom, float top, float near, float far) :
		left(left), right(right), bottom(bottom), top(top), near(near), far(far) {
		update_projection_matrix();
	};
	void update_projection_matrix() override {
		Projection = glm::ortho<float>(left, right, bottom, top, near, far);
	}
};