#include "Game.hpp"

Game::Game(): window("Chapter 7", sf::Vector2u(800, 600)), stateManager(&context), entityManager(&context, 100)
{
	clock.restart();
	srand(time(nullptr));

	context.window = &window;
	context.eventManager = window.getEventManager();
	context.textureManager = &textureManager;
	context.entityManager = &entityManager;

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