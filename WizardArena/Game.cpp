#include "Game.hpp"

Game::Game(): window("Chapter 7", sf::Vector2u(800, 600)), stateManager(&context), 
				entityManager(&systemManager, &textureManager)
{
	clock.restart();
	srand(time(nullptr));

	systemManager.setEntityManager(&entityManager);
	context.window = &window;
	context.eventManager = window.getEventManager();
	context.textureManager = &textureManager;
	context.entityManager = &entityManager;
	context.systemManager = &systemManager;

	stateManager.switchTo(StateType::Intro);
}

Game::~Game(){ }

sf::Time Game::getElapsed(){ return clock.getElapsedTime(); }
void Game::restartClock(){ elapsed = clock.restart(); }
Window* Game::getWindow(){ return &window; }

void Game::update(){
	window.update();
	stateManager.update(elapsed);
}

void Game::render(){
	window.beginDraw();
	// Render here.
	stateManager.draw();

	//DEBUG
	if (context.debugOverlay.debug()) {
		context.debugOverlay.draw(window.getRenderWindow());
	}
	window.endDraw();
}

void Game::lateUpdate(){
	stateManager.processRequests();
	restartClock();
}