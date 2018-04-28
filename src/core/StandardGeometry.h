#pragma once
#include "Geometry.h"
#include "ArrayBuffer.h"
#include "ElementBuffer.h"

class StandardGeometry : public Geometry {
private:
	ArrayBuffer<GLfloat, GL_FLOAT> *positions = new ArrayBuffer<GLfloat, GL_FLOAT>(0, 3);
	ArrayBuffer<GLfloat, GL_FLOAT> *normals = new ArrayBuffer<GLfloat, GL_FLOAT>(1, 3);
	ArrayBuffer<GLint, GL_INT> *colors = new ArrayBuffer<GLint, GL_INT>(2, 3);
	ElementBuffer *indices = new ElementBuffer();

public:
	~StandardGeometry() override {
		delete positions;
		delete normals;
		delete colors;
		delete indices;
	}

	void push_vertice(glm::vec3 &position, glm::vec3 &normal, glm::ivec3 &color) {
		positions->push_back(position[0]);
		positions->push_back(position[1]);
		positions->push_back(position[2]);

		normals->push_back(normal[0]);
		normals->push_back(normal[1]);
		normals->push_back(normal[2]);

		colors->push_back(color[0]);
		colors->push_back(color[1]);
		colors->push_back(color[2]);
	}

	unsigned int getIndex() {
		return positions->size() / 3;
	}

	ArrayBuffer<GLfloat, GL_FLOAT>* get_positions() {
		return positions;
	}

	ArrayBuffer<GLfloat, GL_FLOAT>* get_normals() {
		return normals;
	}

	ArrayBuffer<GLint, GL_INT>* get_colors() {
		return colors;
	}

	ElementBuffer* get_indices() {
		return indices;
	}

	int get_indices_count() override { 
		return indices->size();
	};

	void load() override {
		positions->load();
		normals->load();
		colors->load();
		indices->load();
	};

	void unload() override {
		positions->unload();
		normals->unload();
		colors->unload();
		indices->unload();
	};

	void bind() override {
		positions->bind();
		normals->bind();
		colors->bind();
		indices->bind();
	};

	void unbind() override {
		positions->unbind();
		normals->unbind();
		colors->unbind();
		indices->unbind();
	};
};