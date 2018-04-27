#pragma once

#include <GL/glew.h>
#include <common/shader.hpp>
#include "Shader.h"

class Scene;
class Node;
class Camera;

class Material {
public:
	bool loaded = false;

	UniformAttributes uniforms;
	Scene *scene;
	Renderer *currentRenderer;
	Node *currentNode;
	Camera *currentCamera;

	virtual void load() {};

	virtual void unload() {};

	virtual void bind() {};

	Material() {};

	~Material() {};
};