#pragma once
#include <vector>
#include <GL/glew.h>

class ElementBuffer {
private:
	std::vector<unsigned int> data;
public:
	GLuint handle = 0;

	int size() {
		return data.size();
	}

	void load() {
		glGenBuffers(1, &handle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned int), &data[0], GL_STATIC_DRAW);
	}

	void unload() {
		glDeleteBuffers(1, &handle);
	}

	void bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
	}

	void unbind() {
	}

	void push_back(unsigned int value) {
		data.push_back(value);
	}

	void push_back(unsigned int v0, unsigned int v1, unsigned int v2) {
		data.push_back(v0);
		data.push_back(v1);
		data.push_back(v2);
	}
};