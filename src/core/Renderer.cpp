#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Renderer.h"

void Renderer::loadIfNeeded() {
	if (!loaded) {
		load();
		loaded = true;
	}
}

void Renderer::render(Scene * scene, Camera * camera, RenderTarget * renderTarget) {
	loadIfNeeded();

	if (renderTarget == 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, window->width, window->height);
	}
	else {
		if (!renderTarget->loaded) {
			renderTarget->load();
			renderTarget->loaded = true;
		}
		renderTarget->bind();
		glViewport(0, 0, renderTarget->width, renderTarget->height);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (camera != 0) {
		camera->update_view_matrix();
	}

	for (Node *node : scene->nodes) {
		node->currentRenderer = this;
		node->update_matrix();
		node->render(camera);
	}
}

void Renderer::load() {
	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		throw std::runtime_error("Failed to initialize GLEW\n");
	}

	glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0);

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
}
