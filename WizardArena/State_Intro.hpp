#pragma once
#include "BaseState.hpp"
#include "EventManager.hpp"

class State_Intro : public BaseState{
public:
	State_Intro(StateManager* stateManager);
	~State_Intro();

	void onCreate();
	void onDestroy();

	void activate();
	void deactivate();

	void update(const sf::Time& time);
	void draw();

	void Continue(EventDetails* details);
private:
	sf::Texture introTexture;
	sf::Sprite introSprite;
	sf::Font font;
	sf::Text text;

	float timeLength;
	float timePassed;
};