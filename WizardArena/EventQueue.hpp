#pragma once
#include <queue>

using EventID = unsigned int;

class EventQueue {
private:
	std::queue<EventID> queue;

public:
	void addEvent(const EventID& event) {
		queue.push(event);
	}

	bool processEvents(EventID& id) {
		if (queue.empty())
			return false;
		id = queue.front();
		queue.pop();
		return true;
	}

	void clear() {
		while (!queue.empty()) {
			queue.pop();
		}
	}
};