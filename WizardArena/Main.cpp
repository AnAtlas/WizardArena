#include "Game.hpp"

void main(int argc, void** argv[]){
	// Program entry point.
	Game game;
	while(!game.getWindow()->isDone()){
		game.update();
		game.render();
		game.lateUpdate();
	}
}