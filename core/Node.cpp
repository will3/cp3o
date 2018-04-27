#include "Node.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

void Node::add(Node * node)
{
	if (node->parent != 0) {
		node->parent->remove(node);
	}

	nodes.insert(node);
	node->parent = this;
	if (scene != 0) {
		node->scene = scene;
	}
}

void Node::remove(Node * node)
{
	nodes.erase(node);
	node->parent = 0;
}

void Node::update_matrix()
{
	matrix = glm::translate(position) * glm::toMat4(rotation) * glm::scale(scale);
}

void Node::remove_self()
{
	parent->remove(this);
}
