#pragma once;
#include "Geometry.h"
#include <array>
#include "ArrayBuffer.h"
#include "ElementBuffer.h"

struct VoxelVertice {
	std::array<GLint, 3> position;
	std::array<GLint, 3> color;
	GLint lighting = max_light_value;
};

class VoxelGeometry : public Geometry {
private:
	ArrayBuffer<GLint, GL_INT> *positions = new ArrayBuffer<GLint, GL_INT>(0, 3);
	ArrayBuffer<GLint, GL_INT> *colors = new ArrayBuffer<GLint, GL_INT>(1, 3);
	ArrayBuffer<GLint, GL_INT> *lighting = new ArrayBuffer<GLint, GL_INT>(2, 1);
	ElementBuffer *indices = new ElementBuffer();
public:
	void push_vertice(float x, float y, float z, int r, int g, int b, int light) {
		positions->push_back(x);
		positions->push_back(y);
		positions->push_back(z);

		colors->push_back(r);
		colors->push_back(g);
		colors->push_back(b);

		lighting->push_back(light);
	}

	ArrayBuffer<GLint, GL_INT>* get_positions() {
		return positions;
	}

	ArrayBuffer<GLint, GL_INT>* get_colors() {
		return colors;
	}

	ArrayBuffer<GLint, GL_INT>* get_lighting() {
		return lighting;
	}

	ElementBuffer* get_indices() {
		return indices;
	}

	void load() {
		positions->load();
		colors->load();
		lighting->load();
		indices->load();
	}

	void unload() {
		positions->unload();
		colors->unload();
		lighting->unload();
		indices->unload();
	}

	void bind()
	{
		positions->bind();
		colors->bind();
		lighting->bind();
		indices->bind();
	}

	void unbind()
	{
		positions->unbind();
		colors->unbind();
		lighting->unbind();
		indices->unbind();
	}

	int get_indices_count() {
		return indices->size();
	}

	~VoxelGeometry() {
		delete positions;
		delete colors;
		delete lighting;
		delete indices;
	}
};