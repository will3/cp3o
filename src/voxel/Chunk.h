#pragma once
#include <vector>
#include "Mask.h"
#include "Mesh.h"
#include <unordered_map>
#include "types.h"
#include <glm/glm.hpp>

template<typename T>
class Chunk
{
private:
    std::vector<T> data;
    
    Coord3 origin;
    Coord3 offset;
    int get_index(int i, int j, int k) {
        return i * size * size + j * size + k;
    }
public:
	int size = 32;
    Chunk(Coord3 origin = Coord3(), int size = 32) {
        this->origin = origin;
        this->offset = origin * size;
		this->size = size;
        
        data.resize(size * size * size);
    }
    ~Chunk() {}
    Coord3& get_origin() { return origin; }
    Coord3& get_offset() { return offset; }
    bool dirty;
    
    glm::vec3 position;
    
    T get(Coord3 coord) {
        int index = coord.i * size * size + coord.j * size + coord.k;
		if (index < 0 || index >= data.size()) {
			throw std::runtime_error("out of index");
		}
        return data[index];
    }
    
    void set(Coord3 coord, T v) {
        int index = get_index(coord.i, coord.j, coord.k);
        data[index] = v;
        dirty = true;
    }
};

