#pragma once
#include "BaseState.hpp"
#include "EventManager.hpp"

class State_Game : public BaseState{
public:
	State_Game(StateManager* stateManager);
	~State_Game();

	void onCreate();
	void onDestroy();

	void activate();
	void deactivate();

	void update(const sf::Time& l_time);
	void draw();

	void mainMenu(EventDetails* l_details);
	void pause(EventDetails* l_details);
private:
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f increment;
};