#include "StateManager.hpp"

StateManager::StateManager(std::shared_ptr<SharedContext> shared) : sharedContext(shared) {
	registerState<State_MainMenu>(StateType::MainMenu);
}

StateManager::~StateManager() {
	for (auto itr : states) {
		itr.second->onDestroy();
	}
	states.clear();
}

void StateManager::draw() {
	if (states.empty())
		return;
	if (states.back().second->isTransparent() && states.size() > 1) {
		auto itr = states.end();
		while (itr != states.begin()) {
			if (itr != states.end()) {
				if (!itr->second->isTransparent())
					break;
			}
			--itr;
		}
		for (; itr != states.end(); ++itr) {
			itr->second->draw();
		}
	}
	else {
		states.back().second->draw();
	}
}

void StateManager::update(const sf::Time time) {
	if (states.empty())
		return;

	if (states.back().second->isTranscendent() && states.size() > 1) {
		auto itr = states.end();
		while (itr != states.begin()) {
			if (itr != states.end()) {
				if (!itr->second->isTranscendent())
					break;
			}
			--itr;
		}
		for (; itr != states.end(); ++itr) {
			itr->second->update(time);
		}
	}
	else {
		states.back().second->update(time);
	}
}

std::shared_ptr<SharedContext> StateManager::getContext() {
	return sharedContext;
}

bool StateManager::hasState(const StateType type) {
	for (auto itr = states.begin(); itr != states.end(); ++itr) {
		if (itr->first == type) {
			auto removed = std::find(toRemove.begin(), toRemove.end(), type);
			if (removed == toRemove.end())
				return true;
			return false;
		}
	}
	return false;
}

void StateManager::remove(const StateType type) {
	toRemove.push_back(type);
}

void StateManager::processRequests() {
	while (toRemove.begin() != toRemove.end()) {
		removeState(*toRemove.begin());
		toRemove.erase(toRemove.begin());
	}
}

void StateManager::switchTo(const StateType type) {
	sharedContext->eventManager->setCurrentState(type);
	for (auto itr = states.begin(); itr != states.end(); ++itr) {
		if (itr->first == type) {
			states.back().second->deactivate();
			StateType tempType = itr->first;
			std::shared_ptr<BaseState> tempState = itr->second;
			states.erase(itr);
			states.emplace_back(tempType, tempState);
			tempState->activate();
			return; 
		}
	}

	// State with type wasn't found.
	if (states.empty()) {
		states.back().second->deactivate();
	}
	createState(type);
	states.back().second->activate();
}

void StateManager::createState(const StateType type) {
	auto newState = stateFactory.find(type);
	if (newState == stateFactory.end()) {
		return;
	}
	std::shared_ptr<BaseState> state = newState->second;
	states.emplace_back(type, state);

	state->onCreate();
}

void StateManager::removeState(const StateType type) {
	for (auto itr = states.begin(); itr != states.end(); ++itr) {
		if (itr->first == type) {
			itr->second->onDestroy();
			states.erase(itr);
			return;
		}
	}
}