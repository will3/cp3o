#pragma once

class Runner;
class Entity
{
public:
	Entity();
	~Entity();
	int id = 0;
	bool started = false;
	bool removed = false;
	virtual void update();
	virtual void start();
	virtual void remove();
	Runner *runner;
};

