#pragma once

#include <vector>
#include "Node.h"
#include "Geometry.h"
#include "Material.h"

class Mesh : public Node {
public:
	Geometry * geometry;
	Material * material;

	Mesh(Geometry *geometry, Material *material);

	void render(Camera *camera);

	~Mesh();
};