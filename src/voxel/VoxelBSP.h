#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <map>

class VoxelChunk {
public:
	std::vector<VoxelChunk *> children;
	int size = 32;
	int level = 0;
	glm::ivec3 origin = glm::ivec3(0, 0, 0);
	std::vector<int> data;
	int childSegments = 2;

	int get(int i, int j, int k) {
		if (level == 0) {
			int index = getIndex(i, j, k);
			if (index < 0 || index >= data.size()) {
				throw std::runtime_error("out of index");
			}
			return data[index];
		}
		else {
			if (children.size() == 0) {
				return 0;
			}

			int childIndex = getChildIndex(i, j, k);
			VoxelChunk *child = children[childIndex];
			return child->get(i, j, k);
		}
	}

	int getLocal(int i, int j, int k) {
		int index = i * size * size + j * size + k;
		return data[index];
	}

	VoxelChunk * getChunk(int i, int j, int k) {
		if (level == 0) {
			return this;
		}
		else {
			if (children.size() == 0) {
				return 0;
			}

			int childIndex = getChildIndex(i, j, k);
			VoxelChunk *child = children[childIndex];
			return child->getChunk(i, j, k);
		}
	}

	void set(int i, int j, int k, int v) {
		if (level == 0) {
			if (data.size() == 0) {
				data.resize(size * size * size);
			}
			int index = getIndex(i, j, k);
			if (index < 0 || index > data.size()) {
				return;
			}
			data[index] = v;
		}
		else {
			if (children.size() == 0) {
				makeChildren();
			}

			int childIndex = getChildIndex(i, j, k);
			children[childIndex]->set(i, j, k, v);
		}
	}

	int getIndex(int& i, int& j, int& k) {
		int iLocal = i - origin[0];
		int jLocal = j - origin[1];
		int kLocal = k - origin[2];
		return iLocal * size * size + jLocal * size + kLocal;
	}

	int getChildIndex(int i, int j, int k) {
		int halfSize = size / 2;
		int iDiff = (i - origin[0]) >= halfSize ? 4 : 0;
		int jDiff = (j - origin[1]) >= halfSize ? 2 : 0;
		int kDiff = (k - origin[2]) >= halfSize ? 1 : 0;

		return iDiff + jDiff + kDiff;
	}

	void makeChildren(VoxelChunk *existingChildren = 0) {
		children.resize(8);
		int childIndex = existingChildren == 0 ? -1 : 
			getChildIndex(existingChildren->origin[0], existingChildren->origin[1], existingChildren->origin[2]);
		int nextLevel = level - 1;
		int halfSize = size / 2;

		if (childIndex != 0) children[0] = new VoxelChunk(nextLevel, halfSize, glm::ivec3(origin[0], origin[1], origin[2]));
		if (childIndex != 1) children[1] = new VoxelChunk(nextLevel, halfSize, glm::ivec3(origin[0], origin[1], origin[2] + halfSize));
		if (childIndex != 2) children[2] = new VoxelChunk(nextLevel, halfSize, glm::ivec3(origin[0], origin[1] + halfSize, origin[2]));
		if (childIndex != 3) children[3] = new VoxelChunk(nextLevel, halfSize, glm::ivec3(origin[0], origin[1] + halfSize, origin[2] + halfSize));
		if (childIndex != 4) children[4] = new VoxelChunk(nextLevel, halfSize, glm::ivec3(origin[0] + halfSize, origin[1], origin[2]));
		if (childIndex != 5) children[5] = new VoxelChunk(nextLevel, halfSize, glm::ivec3(origin[0] + halfSize, origin[1], origin[2] + halfSize));
		if (childIndex != 6) children[6] = new VoxelChunk(nextLevel, halfSize, glm::ivec3(origin[0] + halfSize, origin[1] + halfSize, origin[2]));
		if (childIndex != 7) children[7] = new VoxelChunk(nextLevel, halfSize, glm::ivec3(origin[0] + halfSize, origin[1] + halfSize, origin[2] + halfSize));
		
		if (existingChildren != 0) {
			children[childIndex] = existingChildren;
		}
	}

	bool isOutOfBounds(int i, int j, int k) {
		int iDiff = i - origin[0];
		if (iDiff < 0 || iDiff >= size) {
			return true;
		}
		int jDiff = j - origin[1];
		if (jDiff < 0 || jDiff >= size) {
			return true;
		}
		int kDiff = k - origin[2];
		if (kDiff < 0 || kDiff >= size) {
			return true;
		}

		return false;
	}

	bool hasChild(int index) {
		return children[index] == 0;
	}

	VoxelChunk(int level, int size, glm::ivec3 origin) : level(level), size(size), origin(origin) {};
	VoxelChunk() {};

	~VoxelChunk() {
		for (VoxelChunk *chunk : children) {
			delete chunk;
		}
	};

	VoxelChunk(int size) : size(size) {}
};

class VoxelBSP {
private:
	VoxelChunk * data = new VoxelChunk();
public:
	void set(int i, int j, int k, int v) {
		while (data->isOutOfBounds(i, j, k)) {
			int iDiff = i - data->origin[0];
			int jDiff = j - data->origin[1];
			int kDiff = k - data->origin[2];
			int offsetI = iDiff < 0 ? -1 : 0;
			int offsetJ = jDiff < 0 ? -1 : 0;
			int offsetK = kDiff < 0 ? -1 : 0;

			expand(offsetI, offsetJ, offsetK);
		}

		data->set(i, j, k, v);
	};

	int get(int i, int j, int k) {
		if (data->isOutOfBounds(i, j, k)) {
			return 0;
		}
		return data->get(i, j, k);
	};

	~VoxelBSP() { delete data; }

	void expand(int offsetI, int offsetJ, int offsetK) {
		glm::ivec3 origin = data->origin;
		int size = data->size;
		int level = data->level;

		glm::ivec3 nextOrigin = glm::ivec3(origin[0] + offsetI * size, origin[1] + offsetJ * size, origin[2] + offsetK * size);
		VoxelChunk *parent = new VoxelChunk(level + 1, size * 2, nextOrigin);
		parent->makeChildren(data);
		data = parent;
	}

	VoxelChunk * getChunk(int i, int j, int k) {
		return data->getChunk(i, j, k);
	}

	VoxelBSP(int size = 32) {
		data = new VoxelChunk(size);
	};
};