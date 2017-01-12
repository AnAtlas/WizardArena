#include "Game.hpp"


int main()
{
	Game game;

	while (!game.getWindow()->isDone()) {
		game.handleInput();
		game.update();
		game.render();
		sf::sleep(sf::seconds(0.2));
		game.restartClock();
	}

	return 0;
}