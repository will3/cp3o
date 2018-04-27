#pragma once
#include "StandardGeometry.h"
#include "glm\glm.hpp"

#define M_PI           3.14159265358979323846

class SphereGeometry : public StandardGeometry {

public:
	float radius;
	int widthSegments;
	int heightSegments;
	float phiStart;
	float phiLength;
	float thetaStart;
	float thetaLength;

	SphereGeometry(float radius, int widthSegments = 8, int heightSegments = 6, float phiStart = 0, float phiLength = M_PI * 2, float thetaStart = 0, float thetaLength = M_PI)
		: radius(radius), widthSegments(widthSegments), heightSegments(heightSegments), phiStart(phiStart), phiLength(phiLength), thetaStart(thetaStart), thetaLength(thetaLength) { 
		
		float thetaEnd = thetaStart + thetaLength;

		int ix, iy;

		float index = 0;
		std::vector<std::vector<float>> grid;

		glm::vec3 vertex;
		glm::vec3 normal;

		// buffers

		//var indices = [];
		//var vertices = [];
		//var normals = [];
		//var uvs = [];

		// generate vertices, normals and uvs

		for (iy = 0; iy <= heightSegments; iy++) {

			std::vector<float> verticesRow;

			float v = iy / (float)heightSegments;

			for (ix = 0; ix <= widthSegments; ix++) {

				float u = ix / (float)widthSegments;

				// vertex

				vertex.x = -radius * cos(phiStart + u * phiLength) * sin(thetaStart + v * thetaLength);
				vertex.y = radius * cos(thetaStart + v * thetaLength);
				vertex.z = radius * sin(phiStart + u * phiLength) * sin(thetaStart + v * thetaLength);

				get_positions()->push_back(vertex.x);
				get_positions()->push_back(vertex.y);
				get_positions()->push_back(vertex.z);

				// normal

				glm::vec3 normal = glm::vec3(vertex.x, vertex.y, vertex.z);
				normal = glm::normalize(normal);
				get_normals()->push_back(-normal.x);
				get_normals()->push_back(-normal.y);
				get_normals()->push_back(-normal.z);

				get_colors()->push_back(255,255,255);

				// uv

				//uvs.push(u, 1 - v);

				verticesRow.push_back(index++);

			}

			grid.push_back(verticesRow);

		}

		// indices

		for (iy = 0; iy < heightSegments; iy++) {

			for (ix = 0; ix < widthSegments; ix++) {

				float a = grid[iy][ix + 1];
				float b = grid[iy][ix];
				float c = grid[iy + 1][ix];
				float d = grid[iy + 1][ix + 1];

				if (iy != 0 || thetaStart > 0) {
					get_indices()->push_back(a);
					get_indices()->push_back(b);
					get_indices()->push_back(d);
				}
				if (iy != heightSegments - 1 || thetaEnd < M_PI) {
					get_indices()->push_back(b);
					get_indices()->push_back(c);
					get_indices()->push_back(d);
				}

			}

		}

		// build geometry

		//this.setIndex(indices);
		//this.addAttribute('position', new Float32BufferAttribute(vertices, 3));
		//this.addAttribute('normal', new Float32BufferAttribute(normals, 3));
		//this.addAttribute('uv', new Float32BufferAttribute(uvs, 2));

	}


};