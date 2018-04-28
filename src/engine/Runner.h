#pragma once
#include "Entity.h"
#include <map>

class Runner
{
private:
	std::map<int, Entity *> map;

public:
	Runner();
	~Runner();
	int add(Entity *entity);
	void remove(int id);
	bool has(int id);
	bool get(int id);
	void update();
	float delta = 0.0f;
};

