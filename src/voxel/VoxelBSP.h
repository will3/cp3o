#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <map>

class VoxelChunk {
public:
	VoxelChunk * parent = 0;
	std::map<int, VoxelChunk *> children;
	int size = 32;
	int level = 0;
	glm::ivec3 origin = glm::ivec3(0, 0, 0);
	std::vector<int> data;
	int childSegments = 2;
	int childIndex;

	int get(int i, int j, int k) {
		if (level == 0) {
			if (data.size() == 0) {
				return 0;
			}
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
			if (!hasChild(childIndex)) {
				return 0;
			}
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
			if (!hasChild(childIndex)) {
				return 0;
			}
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
			int childIndex = getChildIndex(i, j, k);
			makeChild(childIndex);
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

	void makeChild(int index) {
		if (hasChild(index)) {
			return;
		}

		int nextLevel = level - 1;
		int halfSize = size / 2;

		glm::ivec3 childOrigin = origin;

		if ((index & 4) == 4) {
			childOrigin[0] += halfSize;
		}
		if ((index & 2) == 2) { 
			childOrigin[1] += halfSize; 
		}
		if ((index & 1) == 1) { 
			childOrigin[2] += halfSize;
		}

		children[index] = new VoxelChunk(nextLevel, halfSize, childOrigin);
		children[index]->parent = this;
		children[index]->childIndex = index;
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
		return children[index] != 0;
	}

	void remove(int i, int j, int k) {
		VoxelChunk *chunk = getChunk(i, j, k);
		if (chunk != 0) {
			chunk->parent->removeChild(chunk);

			while ((chunk = chunk->parent) != 0) {
				if (chunk->children.size() == 0) {
					chunk->parent->removeChild(chunk);
				}
				else {
					break;
				}
			}
		}
	}

	void removeChild(VoxelChunk *chunk) {
		children.erase(chunk->childIndex);
		delete chunk;
	}

	VoxelChunk(int level, int size, glm::ivec3 origin) : level(level), size(size), origin(origin) {};
	VoxelChunk() {};

	~VoxelChunk() {
		for (auto kv : children) {
			VoxelChunk *chunk = kv.second;
			delete chunk;
		}
	};

	VoxelChunk(int size) : size(size) {}
};

class VoxelBSP {
private:
	VoxelChunk * data = new VoxelChunk();

	void expandWithOffset(int offsetI, int offsetJ, int offsetK) {
		glm::ivec3 origin = data->origin;
		int size = data->size;
		int level = data->level;

		glm::ivec3 nextOrigin = glm::ivec3(origin[0] + offsetI * size, origin[1] + offsetJ * size, origin[2] + offsetK * size);
		VoxelChunk *parent = new VoxelChunk(level + 1, size * 2, nextOrigin);

		int childIndex = parent->getChildIndex(origin[0], origin[1], origin[2]);
		parent->children[childIndex] = data;
		data->parent = parent;
		data->childIndex = childIndex;

		data = parent;
	}

public:
	int size = 32;
	void set(int i, int j, int k, int v) {
		expand(i, j, k);
		data->set(i, j, k, v);
	};

	void expand(int i, int j, int k) {
		while (data->isOutOfBounds(i, j, k)) {
			int iDiff = i - data->origin[0];
			int jDiff = j - data->origin[1];
			int kDiff = k - data->origin[2];
			int offsetI = iDiff < 0 ? -1 : 0;
			int offsetJ = jDiff < 0 ? -1 : 0;
			int offsetK = kDiff < 0 ? -1 : 0;

			expandWithOffset(offsetI, offsetJ, offsetK);
		}
	}

	int get(int i, int j, int k) {
		if (data->isOutOfBounds(i, j, k)) {
			return 0;
		}
		return data->get(i, j, k);
	};

	~VoxelBSP() { delete data; }

	VoxelChunk * getChunk(int i, int j, int k) {
		return data->getChunk(i, j, k);
	}

	VoxelBSP(int size = 32) : size(size) {
		data = new VoxelChunk(size);
	};

	void remove(int i, int j, int k) {
		data->remove(i, j, k);
	}
};