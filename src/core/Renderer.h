#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Scene.h"
#include "Camera.h"
#include "RenderTarget.h"
#include "Window.h"

class Renderer {
private:
	void load();
	void unload() { glDeleteVertexArrays(1, &VertexArrayID); }
public:
	GLuint VertexArrayID;
	bool loaded = false;
	Window *window;
    glm::vec3 clearColor = glm::vec3(0, 0, 0);
    
	Renderer()
	{
	}

	void render(Scene *scene, Camera *camera) {
		render(scene, camera, 0);
	}

	void render(Scene *scene, Camera *camera, RenderTarget *renderTarget);

	void loadIfNeeded();
};
