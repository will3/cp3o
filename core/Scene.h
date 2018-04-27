#pragma once

#include <vector>
#include "Node.h"
#include <unordered_set>
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include <set>
#include "Material.h"

class ShadowMap;

class Scene : public Node {
public:
	std::set<DirectionalLight *> directional_lights;
	std::set<AmbientLight *> ambient_lights;
	Material *override_material = 0;
	ShadowMap *shadowMap = 0;

	void add(Node *node) override {
		Node::add(node);

		node->scene = this;

		DirectionalLight *directional_light = dynamic_cast<DirectionalLight *>(node);
		if (directional_light != NULL) {
			directional_lights.insert(directional_light);
		}
	
		AmbientLight *ambientLight = dynamic_cast<AmbientLight *>(node);
		if (ambientLight != NULL) {
			ambient_lights.insert(ambientLight);
		}
	}
};