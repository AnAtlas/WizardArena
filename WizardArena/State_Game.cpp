#include "State_Game.hpp"
#include "StateManager.hpp"

State_Game::State_Game(StateManager* stateManager)
	: BaseState(stateManager){}

State_Game::~State_Game(){}

void State_Game::onCreate(){
	EventManager* evMgr = stateMgr->getContext()->eventManager;

	evMgr->addCallback(StateType::Game,"Key_Escape",&State_Game::mainMenu,this);
	evMgr->addCallback(StateType::Game,"Key_P",&State_Game::pause,this);
	evMgr->addCallback(StateType::Game, "Key_O", &State_Game::toggleOverlay, this);

	sf::Vector2u size = stateMgr->getContext()->window->getWindowSize();
	view.setSize(size.x, size.y);
	view.setCenter(size.x / 2, size.y / 2);
	view.zoom(0.6f);
	stateMgr->getContext()->window->getRenderWindow()->setView(view);

	gameMap = new Map(stateMgr->getContext(), this);
	gameMap->loadMap("media/Maps/map1.map");
}

void State_Game::onDestroy(){
	EventManager* evMgr = stateMgr->getContext()->eventManager;
	evMgr->removeCallback(StateType::Game,"Key_Escape");
	evMgr->removeCallback(StateType::Game,"Key_P");
	evMgr->removeCallback(StateType::Game, "Key_O");

	delete gameMap;
	gameMap = nullptr;
}

void State_Game::update(const sf::Time& time){
	SharedContext* context = stateMgr->getContext();
	EntityBase* player = context->entityManager->find("Player");
	if (!player) {
		std::cout << "Respawning player..." << std::endl;
		context->entityManager->add(EntityType::Player, "Player");
		player = context->entityManager->find("Player");
		player->setPosition(gameMap->getPlayerStart());
	}
	else {
		view.setCenter(player->getPosition());
		context->window->getRenderWindow()->setView(view);
	}

	sf::FloatRect viewSpace = context->window->getViewSpace();
	if (viewSpace.left <= 0) {
		view.setCenter(viewSpace.width / 2, view.getCenter().y);
		context->window->getRenderWindow()->setView(view);
	}
	else if (viewSpace.left + viewSpace.width > (gameMap->getMapSize().x + 1) * Sheet::TileSize) {
		view.setCenter(((gameMap->getMapSize().x + 1) * Sheet::TileSize) - (viewSpace.width / 2), view.getCenter().y);
		context->window->getRenderWindow()->setView(view);
	}
	gameMap->update(time.asSeconds());
	stateMgr->getContext()->entityManager->update(time.asSeconds());
}

void State_Game::draw(){
	gameMap->draw();
	stateMgr->getContext()->entityManager->draw();
}

void State_Game::mainMenu(EventDetails* l_details){
	stateMgr->switchTo(StateType::MainMenu); 
}

void State_Game::pause(EventDetails* l_details){
	stateMgr->switchTo(StateType::Paused); 
}

void State_Game::activate(){}
void State_Game::deactivate(){}

void State_Game::toggleOverlay(EventDetails* details) {
	stateMgr->getContext()->debugOverlay.setDebug(!stateMgr->getContext()->debugOverlay.debug());
}