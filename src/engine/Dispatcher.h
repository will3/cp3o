#pragma once
#include "Worker.h"
#include <mutex>

class Dispatcher {
private:
	std::map<int, Worker *> map;
	std::vector<int> keys;
	int counter = 0;
	std::mutex mutex;

public:
	void add(Worker *worker) {
		mutex.lock();

		worker->id = ++counter;
		map[worker->id] = worker;
		keys.push_back(worker->id);

		mutex.unlock();
	};

	void update() {
		mutex.lock();

		for (auto key : keys) {
			Worker *worker = map[key];
			worker->run();
		}

		for(auto key : keys) {
			delete map[key];
			map.erase(key);
		}

		mutex.unlock();

		keys.clear();
	};
};