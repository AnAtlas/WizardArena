#pragma once
#include "Window.hpp"
#include "EventManager.hpp"
#include "StateManager.hpp"
#include "TextureManager.hpp"
#include "EntityManager.hpp"
#include <iostream>

class Game{
public:
	Game();
	~Game();

	void update();
	void render();
	void lateUpdate();

	sf::Time getElapsed();

	Window* getWindow();
private:
	SharedContext context;
	Window window;
	StateManager stateManager;
	EntityManager entityManager;
	TextureManager textureManager;
	sf::Clock clock;
	sf::Time elapsed;
	void restartClock();
};