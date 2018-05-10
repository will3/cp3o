#pragma once
#include <vector>
#include <array>
#include <glm/glm.hpp>

#define max_light_value 15

class MaskValue {
public:
	int v = 0;
	int ao0;
	int ao1;
	int ao2;
	int ao3;
	int lighting = max_light_value;
    glm::vec3 color;

	bool has_ao() {
		return  ao0 || ao1 || ao2 || ao3;
	}
	MaskValue(int v, int ao0, int ao1, int ao2, int ao3, int light) : v(v), ao0(ao0), ao1(ao1), ao2(ao2), ao3(ao3), lighting(light) { };
	MaskValue(int v) : v(v) { };
	MaskValue() {};
};

class Mask {
public:
	std::vector<MaskValue> data;
	int i = 0;
	int d = 0;
    bool front = false;

	Mask(int i, int d, int front);
	~Mask();

	bool is_empty(int j, int k);

	MaskValue get(int j, int k);

	void set(int j, int k, MaskValue& v);
};
