#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include "Scene.h"
#include "Camera.h"
#include "RenderTarget.h"
#include "Window.h"

class Renderer {
public:
	GLuint VertexArrayID;
	bool loaded = false;
	Window *window;

	Renderer()
	{
	}

	void render(Scene *scene, Camera *camera) {
		render(scene, camera, 0);
	}

	void render(Scene *scene, Camera *camera, RenderTarget *renderTarget);

	void load();

	void unload() { glDeleteVertexArrays(1, &VertexArrayID); }
};