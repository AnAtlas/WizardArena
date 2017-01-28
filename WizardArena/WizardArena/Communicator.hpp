#pragma once
#include <vector>
#include "Observer.hpp"
#include <algorithm>
using ObserverContainer = std::vector<Observer*>;

class Communicator {
private:
	ObserverContainer observers;

public:
	~Communicator() {
		observers.clear();
	}

	bool addObserver(Observer* observer) {
		if (hasObserver(observer))
			return false;
		observers.emplace_back(observer);
		return true;
	}

	bool removeObserver(Observer* lObserver) {
		auto observer = std::find_if(observers.begin(),
			observers.end(), [&lObserver](Observer* o) {
			return o == lObserver;
		});
		if (observer == observers.end())
			return false;
		observers.erase(observer);
		return true;
	}

	bool hasObserver(const Observer* observer) {
		return (std::find_if(observers.begin(), observers.end(),
			[observer](Observer* o) {
			return o == observer;
		}) != observers.end());
	}

	void broadcast(const Message& msg) {
		for (auto& itr : observers) {
			itr->notify(msg);
		}
	}
};