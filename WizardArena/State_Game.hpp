#pragma once
#include "BaseState.hpp"
#include "Map.hpp"
#include "EventManager.hpp"

class State_Game : public BaseState{
public:
	State_Game(StateManager* stateManager);
	~State_Game();

	void onCreate();
	void onDestroy();

	void activate();
	void deactivate();
	void toggleOverlay(EventDetails* details);
	void update(const sf::Time& l_time);
	void draw();
	void playerMove(EventDetails* details);
	void mainMenu(EventDetails* l_details);
	void pause(EventDetails* l_details);
private:
	Map* gameMap;
	void updateCamera();
	int player;
};