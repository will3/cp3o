#pragma once

#include "Chunk.h"
#include <map>
#include <unordered_map>
#include <string>
#include <unordered_set>
#include <functional>

template<typename T>
class Chunks
{
private:
    Coord3 get_origin(Coord3 coord) {
        return Coord3(
                      fast_floor(coord.i, size),
                      fast_floor(coord.j, size),
                      fast_floor(coord.k, size));
    }
    std::unordered_set<Coord3> coords;
    std::unordered_map<Coord3, Chunk<T> *> map;
    int fast_floor(int x, int y) {
        return x / y - (x % y < 0);
    }
public:
	int size = 32;
    Chunks(int size = 32) : size(size) {};
    ~Chunks() {};
    T get(Coord3 coord) {
        Coord3 origin = get_origin(coord);

        if (!has_chunk(origin)) {
            return T();
        }

        Chunk<T> *chunk = map[origin];
        return chunk->get(coord - chunk->get_offset());
    }
    void set(Coord3 coord, T v) {
        Coord3 origin = get_origin(coord);
        
        get_or_create_chunk(origin)->set(coord - origin * size, v);
    }
    Chunk<T>* get_or_create_chunk(Coord3 origin) {
        if (!has_chunk(origin)) {
            map[origin] = new Chunk<T>(origin);
            coords.insert(origin);
        }
        
        return map[origin];
    }
    void delete_chunk(Coord3 origin) {
        Chunk<T> *chunk = map[origin];
        map.erase(origin);
        coords.erase(origin);
        delete chunk;
    }
    bool has_chunk(Coord3 origin) {
        return coords.find(origin) != coords.end();
    }
    Chunk<T> *get_chunk(Coord3 origin) {
        return map[origin];
    }
    std::unordered_set<Coord3>& get_coords() {
        return coords;
    }
	void remove(Coord3 origin) {
		delete map[origin];
		map.erase(origin);
		coords.erase(origin);
	}
};

