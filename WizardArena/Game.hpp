#pragma once
#include "StateManager.hpp"
#include "Window.hpp"

class Game {
private:
	StateManager stateManager;
	std::shared_ptr<Window> window;
	sf::Clock clock;
	sf::Time elapsed;
public:
	Game();
	~Game();

	void handleInput();
	void update();
	void render();
	std::shared_ptr<Window> getWindow();
	sf::Time getElapsed();
	void restartClock();
};