#include "../core/StandardGeometry.h"
#include <vector>
#include "glm/glm.hpp"

class BoxGeometry : public StandardGeometry {
public:
	float size;

	BoxGeometry(float size) : size(size) {
		// helper variables
		build_plane(0, false);
		build_plane(0, true);
		build_plane(1, false);
		build_plane(1, true);
		build_plane(2, false);
		build_plane(2, true);
	}

	void build_plane(int d, bool front) {
		glm::vec3 v0 = glm::vec3(-0.5, -0.5, -0.5);
		glm::vec3 v1 = glm::vec3(-0.5, -0.5, -0.5);
		glm::vec3 v2 = glm::vec3(-0.5, -0.5, -0.5);
		glm::vec3 v3 = glm::vec3(-0.5, -0.5, -0.5);

		int u = (d + 1) % 3;
		int v = (d + 2) % 3;

		v1[u] += 1.0f;

		v2[u] += 1.0f;
		v2[v] += 1.0f;

		v3[v] += 1.0f;

		if (front) {
			v0[d] += 1.0f;
			v1[d] += 1.0f;
			v2[d] += 1.0f;
			v3[d] += 1.0f;
		}

		glm::vec3 a = v1 - v0;
		glm::vec3 b = v2 - v0;
		glm::vec3 normal = front ? glm::cross(b, a) : glm::cross(a, b);
		
		glm::ivec3 color = glm::ivec3(255, 255, 255);

		unsigned int index = getIndex();

		v0 *= size;
		v1 *= size;
		v2 *= size;
		v3 *= size;

		push_vertice(v0, normal, color);
		push_vertice(v1, normal, color);
		push_vertice(v2, normal, color);
		push_vertice(v3, normal, color);

		if (front) {
			get_indices()->push_back(index, index + 1, index + 2);
			get_indices()->push_back(index + 2, index + 3, index);
		}
		else {
			get_indices()->push_back(index, index + 3, index + 2);
			get_indices()->push_back(index + 2, index + 1, index);
		}
	}
};
