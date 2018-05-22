#pragma once
#include "Entity.h"
#include <map>
#include <functional>
#include <vector>

typedef std::function<void()> runnerTaskType;

class Runner
{
private:
	std::map<int, Entity *> map;
	std::vector<runnerTaskType> tasks;

public:
	Runner();
	~Runner();
	int add(Entity *entity);
	void remove(int id);
	bool has(int id);
	bool get(int id);
	void update();
	float delta = 0.0f;
	void dispatchMain(runnerTaskType task);
};

