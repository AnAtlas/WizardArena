#pragma once
#include "BaseState.hpp"
#include "EventManager.hpp"
/*
This state puts all the main menu elements on the screen
*/
class State_MainMenu : public BaseState {
private:


public:
	State_MainMenu(std::shared_ptr<StateManager> stateManager);
	~State_MainMenu();

	void onCreate();
	void onDestroy();

	void activate();
	void deactivate();

	void update(const sf::Time time);
	void draw();

	void mouseClick(std::shared_ptr<EventDetails> details);
};