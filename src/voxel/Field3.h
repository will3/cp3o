#pragma once
#include <limits>

template <typename T>
class Field3 {
private:
	std::vector<T> data;
public:
	int size;

	Field3(int size) {
		this->size = size;
		data.resize(size * size * size);
	}
	T get(int i, int j, int k) {
		if (i < 0 || i >= size ||
			j < 0 || j >= size ||
			k < 0 || k >= size) {
			throw std::exception("out of bounds");
		}
		int index = i * size * size + j * size + k;
		return data[index];
	}

	void set(int i, int j, int k, T v) {
		int index = i * size * size + j * size + k;
		data[index] = v;
	}

	T sample(float i, float j, float k, float scale) {
		return sample(i / scale, j / scale, k / scale);
	}

	T sample(float i, float j, float k) {
		int min_i = floor(i);
		int min_j = floor(j);
		int min_k = floor(k);
	
		int max_i = min_i + 1;
		int max_j = min_j + 1;
		int max_k = min_k + 1;

		float ri = 1 - (i - min_i);
		float rj = 1 - (j - min_j);
		float rk = 1 - (k - min_k);
		float ri2 = 1 - ri;
		float rj2 = 1 - rj;
		float rk2 = 1 - rk;

		float a = get(min_i, min_j, min_k);
		float b = get(max_i, min_j, min_k);
		float c = get(min_i, max_j, min_k);
		float d = get(max_i, max_j, min_k);
		float e = get(min_i, min_j, max_k);
		float f = get(max_i, min_j, max_k);
		float g = get(min_i, max_j, max_k);
		float h = get(max_i, max_j, max_k);

		float v = 
			((a * ri + b * ri2) * rj + (c * ri + d * ri2) * rj2) * rk +
			((e * ri + f * ri2) * rj + (g * ri + h * ri2) * rj2) * rk2;

		return v;
	}

	std::array<T, 3> sampleGradient(float i, float j, float k) {
		int min_i = floor(i);
		int min_j = floor(j);
		int min_k = floor(k);

		int max_i = min_i + 1;
		int max_j = min_j + 1;
		int max_k = min_k + 1;

		float ri = 1 - (i - min_i);
		float rj = 1 - (j - min_j);
		float rk = 1 - (k - min_k);
		float ri2 = 1 - ri;
		float rj2 = 1 - rj;
		float rk2 = 1 - rk;

		float a = get(min_i, min_j, min_k);
		float b = get(max_i, min_j, min_k);
		float c = get(min_i, max_j, min_k);
		float d = get(max_i, max_j, min_k);
		float e = get(min_i, min_j, max_k);
		float f = get(max_i, min_j, max_k);
		float g = get(min_i, max_j, max_k);
		float h = get(max_i, max_j, max_k);

		vec3 coordA = vec3(0, 0, 0);
		vec3 coordB = vec3(1, 0, 0);
		vec3 coordC = vec3(0, 1, 0);
		vec3 coordD = vec3(1, 1, 0);
		vec3 coordE = vec3(0, 0, 1);
		vec3 coordF = vec3(1, 0, 1);
		vec3 coordG = vec3(0, 1, 1);
		vec3 coordH = vec3(1, 1, 1);
	}

	bool fast_equal(int i, float f) {
		return abs(i - f) < 0.0001;
	}
};