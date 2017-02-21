#include "Game.hpp"

Game::Game(): window("Chapter 7", sf::Vector2u(800, 600)), stateManager(&context), 
				entityManager(&systemManager, &textureManager), guiManager(window.getEventManager(), &context)
{
	clock.restart();
	srand(time(nullptr));

	systemManager.setEntityManager(&entityManager);
	context.window = &window;
	context.eventManager = window.getEventManager();
	context.textureManager = &textureManager;
	context.entityManager = &entityManager;
	context.systemManager = &systemManager;
	context.guiManager = &guiManager;
	stateManager.switchTo(StateType::Intro);
	fontManager.requireResource("Main");
}

Game::~Game(){ 
	fontManager.releaseResource("Main");
}

sf::Time Game::getElapsed(){ return clock.getElapsedTime(); }
void Game::restartClock(){ elapsed = clock.restart(); }
Window* Game::getWindow(){ return &window; }

void Game::update(){
	window.update();
	stateManager.update(elapsed);
	context.guiManager->update(elapsed.asSeconds());
	GUI_Event guiEvent;
	while (context.guiManager->pollEvent(guiEvent))
		window.getEventManager()->handleEvent(guiEvent);
}

void Game::render(){
	window.beginDraw();
	// Render here.
	stateManager.draw();

	sf::View currentView = window.getRenderWindow()->getView();
	window.getRenderWindow()->setView(window.getRenderWindow()->getDefaultView());
	context.guiManager->render(window.getRenderWindow());
	window.getRenderWindow()->setView(currentView);

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