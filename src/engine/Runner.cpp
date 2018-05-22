#include "Runner.h"
#include <vector>
#include "GLFW/glfw3.h"
#include <math.h>
#include <mutex>

static std::mutex tasksLock;
static int next_id = 0;
int get_next_id() { return ++next_id; }

Runner::Runner()
{
}


Runner::~Runner()
{
}

int Runner::add(Entity *entity) {
	if (entity->id == 0) {
		entity->id = get_next_id();
	}

	entity->runner = this;

	map[entity->id] = entity;
	return entity->id;
}

void Runner::remove(int id) {
	map[id]->removed = true;
}

void Runner::update() {
	tasksLock.lock();
	for (runnerTaskType task : tasks) {
		task();
	}
	tasks.clear();
	tasksLock.unlock();

	static double lastTime = glfwGetTime();

	double currentTime = glfwGetTime();

	delta = fmin(1 / 60.0f, float(currentTime - lastTime));

	for (auto kv : map) {
		Entity *entity = kv.second;
		if (entity->started) {
			continue;
		}

		entity->start();
		entity->started = true;
	}

	std::vector<int> to_remove;

	for (auto kv : map) {
		kv.second->update();

		if (kv.second->removed) {
			to_remove.push_back(kv.first);
		}
	}

	for (auto key : to_remove) {
		map[key]->remove();
		map.erase(key);
	}

	lastTime = glfwGetTime();
}

void Runner::dispatchMain(runnerTaskType task) {
	tasksLock.lock();
	tasks.push_back(task);
	tasksLock.unlock();
}

bool Runner::has(int id) {
	return map.count(id) > 0;
}

bool Runner::get(int id) {
	return map[id];
}
