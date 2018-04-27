#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Material.h"
#include "Scene.h"

Mesh::Mesh(Geometry * geometry, Material * material) : geometry(geometry), material(material)
{
}

void Mesh::render(Camera * camera) {
	if (!geometry->loaded) {
		geometry->load();
		geometry->loaded = true;
	}

	Material *material_to_use = scene->override_material == 0 ? 
		material : 
		scene->override_material;

	material_to_use->scene = scene;
	material_to_use->currentRenderer = currentRenderer;
	material_to_use->currentNode = this;
	material_to_use->currentCamera = camera;

	if (!material_to_use->loaded) {
		material_to_use->load();
		material_to_use->loaded = true;
	}

	material_to_use->bind();

	geometry->bind();

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		geometry->get_indices_count(),    // count
		GL_UNSIGNED_INT,   // type
		(void*)0           // element array buffer offset
	);

	geometry->unbind();
}

Mesh::~Mesh() {
}
