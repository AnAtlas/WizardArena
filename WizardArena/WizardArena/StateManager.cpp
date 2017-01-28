#include "StateManager.hpp"

StateManager::StateManager(SharedContext* l_shared)
	: shared(l_shared)
{
	registerState<State_Intro>(StateType::Intro);
	registerState<State_MainMenu>(StateType::MainMenu);
	registerState<State_Game>(StateType::Game);
	registerState<State_Paused>(StateType::Paused);
}

StateManager::~StateManager(){
	for (auto &itr : states){
		itr.second->onDestroy();
		delete itr.second;
	}
}

void StateManager::update(const sf::Time& l_time){
	if (states.empty()){ return; }
	if (states.back().second->isTranscendent() && states.size() > 1){
		auto itr = states.end();
		while (itr != states.begin()){
			if (itr != states.end()){
				if (!itr->second->isTranscendent()){
					break;
				}
			}
			--itr;
		}
		for (; itr != states.end(); ++itr){
			itr->second->update(l_time);
		}
	} else {
		states.back().second->update(l_time);
	}
}

void StateManager::draw(){
	if (states.empty()){ return; }
	if (states.back().second->isTransparent() && states.size() > 1){
		auto itr = states.end();
		while (itr != states.begin()){
			if (itr != states.end()){
				if (!itr->second->isTransparent()){
					break;
				}
			}
			--itr;
		}
		for (; itr != states.end(); ++itr){
			shared->window->getRenderWindow()->setView(itr->second->getView());
			itr->second->draw();
		}
	} else {
		states.back().second->draw();
	}
}

SharedContext* StateManager::getContext(){ return shared; }

bool StateManager::hasState(const StateType& l_type){
	for (auto itr = states.begin();
		itr != states.end(); ++itr)
	{
		if (itr->first == l_type){
			auto removed = std::find(toRemove.begin(), toRemove.end(), l_type);
			if (removed == toRemove.end()){ return true; }
			return false;
		}
	}
	return false;
}

void StateManager::processRequests(){
	while (toRemove.begin() != toRemove.end()){
		removeState(*toRemove.begin());
		toRemove.erase(toRemove.begin());
	}
}

void StateManager::switchTo(const StateType& l_type){
	shared->eventManager->setCurrentState(l_type);
	for (auto itr = states.begin(); itr != states.end(); ++itr){
		if (itr->first == l_type){
			states.back().second->deactivate();
			StateType tmp_type = itr->first;
			BaseState* tmp_state = itr->second;
			states.erase(itr);
			states.emplace_back(tmp_type, tmp_state);
			tmp_state->activate();
			shared->window->getRenderWindow()->setView(tmp_state->getView());
			return;
		}
	}

	// State with l_type wasn't found.
	if (!states.empty()){ 
		states.back().second->deactivate(); 
	}
	createState(l_type);
	states.back().second->activate();
	shared->window->getRenderWindow()->setView(states.back().second->getView());
}

void StateManager::remove(const StateType& l_type){
	toRemove.push_back(l_type);
}

// Private methods.

void StateManager::createState(const StateType& l_type){
	auto newState = stateFactory.find(l_type);
	if (newState == stateFactory.end()){ return; }
	BaseState* state = newState->second();
	state->view = shared->window->getRenderWindow()->getDefaultView();
	states.emplace_back(l_type, state);
	state->onCreate();
}

void StateManager::removeState(const StateType& l_type){
	for (auto itr = states.begin();
		itr != states.end(); ++itr)
	{
		if (itr->first == l_type){
			itr->second->onDestroy();
			delete itr->second;
			states.erase(itr);
			return;
		}
	}
}