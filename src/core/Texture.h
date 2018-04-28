#pragma once
#include <GL/glew.h>

class Texture {
	GLuint handle;

	int width;
	int height;

	Texture(int width, int height) :width(width), height(height) {

	}

	void load() {
		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	~Texture() {
		glDeleteTextures(1, &handle);
	}
};