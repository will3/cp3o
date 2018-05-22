#include "Mask.h"
#include "Mesh.h"
#include "Chunk.h"

Mask::~Mask()
{
}

bool Mask::is_empty(int j, int k) {
	int index = j * size + k;
	return data[index].v == 0;
}

MaskValue Mask::get(int j, int k) {
	int index = j * size + k;
	return data[index];
}

void Mask::set(int j, int k, MaskValue & v) {
	int index = j * size + k;
	data[index] = v;
}

void Mask::clear() {
	for (int i = 0; i < size * size; i++) {
		data[i].v = 0;
	}
}
