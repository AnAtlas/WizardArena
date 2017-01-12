#include "Game.hpp"

Game::Game() : window("Chapter 5", sf::Vector2u(800, 600)), stateManager(context) {

}

Game::~Game() {

}

void Game::render() {
	window->beginDraw();
	//Draw stuff between these
	window->endDraw();
}

sf::Time Game::getElapsed() {
	return elapsed;
}

void Game::restartClock() {
	elapsed = clock.restart();
}