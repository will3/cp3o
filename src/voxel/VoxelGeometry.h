#pragma once

#include "Geometry.h"
#include <array>
#include "ArrayBuffer.h"
#include "ElementBuffer.h"

class VoxelGeometry : public Geometry {
public:
	ArrayBuffer<GLint, GL_INT> *positions = new ArrayBuffer<GLint, GL_INT>(0, 3);
	ArrayBuffer<GLint, GL_INT> *colors = new ArrayBuffer<GLint, GL_INT>(1, 3);
	ArrayBuffer<GLint, GL_INT> *lightings = new ArrayBuffer<GLint, GL_INT>(2, 1);
	ElementBuffer *indices = new ElementBuffer();

	void push_vertice(float x, float y, float z, int r, int g, int b, int lighting) {
		positions->push_back(x);
		positions->push_back(y);
		positions->push_back(z);

		colors->push_back(r);
		colors->push_back(g);
		colors->push_back(b);

		lightings->push_back(lighting);
	}

	void load() {
		positions->load();
		colors->load();
		lightings->load();
		indices->load();
	}

	void unload() {
		positions->unload();
		colors->unload();
		lightings->unload();
		indices->unload();
	}

	void bind()
	{
		positions->bind();
		colors->bind();
		lightings->bind();
		indices->bind();
	}

	void unbind()
	{
		positions->unbind();
		colors->unbind();
		lightings->unbind();
		indices->unbind();
	}

	int get_indices_count() {
		return indices->size();
	}

	~VoxelGeometry() {
		unload();
		delete positions;
		delete colors;
		delete lightings;
		delete indices;
	}
};
