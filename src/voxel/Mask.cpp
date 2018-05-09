#include "Mask.h"
#include "Mesh.h"
#include "Chunk.h"

Mask::Mask(int i, int d, int front) : i(i), d(d), front(front) {
	data.resize(CHUNK_SIZE * CHUNK_SIZE);
}

Mask::~Mask()
{
}

bool Mask::is_empty(int j, int k) {
	int index = j * CHUNK_SIZE + k;
	return data[index].v == 0;
}

MaskValue Mask::get(int j, int k) {
	int index = j * CHUNK_SIZE + k;
	return data[index];
}

void Mask::set(int j, int k, MaskValue & v) {
	int index = j * CHUNK_SIZE + k;
	data[index] = v;
}
