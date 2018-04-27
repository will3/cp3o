#pragma once
#include <GL\glew.h>
#include <vector>

class Geometry
{
public:
	bool loaded = false;
	virtual int get_indices_count() { return 0; };

	virtual void load() {};
	virtual void unload() {};
	virtual void bind() {};
	virtual void unbind() {};

	virtual ~Geometry() {};
};