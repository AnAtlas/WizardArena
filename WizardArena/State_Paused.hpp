#pragma once
#include "BaseState.hpp"
#include "EventManager.hpp"

class State_Paused : public BaseState{
public:
	State_Paused(StateManager* stateManager);
	~State_Paused();

	void onCreate();
	void onDestroy();

	void activate();
	void deactivate();

	void update(const sf::Time& time);
	void draw();

	void unpause(EventDetails* details);
private:
	sf::Font font;
	sf::Text text;
	sf::RectangleShape rect;
};