#include "State_Game.hpp"
#include "StateManager.hpp"

State_Game::State_Game(StateManager* stateManager)
	: BaseState(stateManager){}

State_Game::~State_Game(){}

void State_Game::onCreate(){
	EventManager* evMgr = stateManager->getContext()->eventManager;

	evMgr->addCallback(StateType::Game,"Key_Escape",&State_Game::mainMenu,this);
	evMgr->addCallback(StateType::Game,"Key_P",&State_Game::pause,this);
	evMgr->addCallback(StateType::Game, "Key_O", &State_Game::toggleOverlay, this);
	evMgr->addCallback(StateType::Game, "Player_MoveLeft", &State_Game::playerMove, this);
	evMgr->addCallback(StateType::Game, "Player_MoveRight", &State_Game::playerMove, this);
	evMgr->addCallback(StateType::Game, "Player_MoveUp", &State_Game::playerMove, this);
	evMgr->addCallback(StateType::Game, "Player_MoveDown", &State_Game::playerMove, this);

	sf::Vector2u size = stateManager->getContext()->window->getWindowSize();
	view.setSize(size.x, size.y);
	view.setCenter(size.x / 2, size.y / 2);
	view.zoom(0.6f);
	stateManager->getContext()->window->getRenderWindow()->setView(view);

	gameMap = new Map(stateManager->getContext(), this);
	gameMap->loadMap("media/Maps/map1.map");
	S_Movement* ass = stateManager->getContext()->systemManager->getSystem<S_Movement>(System::Movement);
	ass->setMap(gameMap);
	//stateManager->getContext()->systemManager->getSystem<S_Movement>(System::Movement)->setMap(gameMap);
	player = gameMap->getPlayerId();
}

void State_Game::onDestroy(){
	EventManager* evMgr = stateManager->getContext()->eventManager;
	evMgr->removeCallback(StateType::Game,"Key_Escape");
	evMgr->removeCallback(StateType::Game,"Key_P");
	evMgr->removeCallback(StateType::Game, "Key_O");

	delete gameMap;
	gameMap = nullptr;
}

void State_Game::update(const sf::Time& time){
	SharedContext* context = stateManager->getContext();
	updateCamera();
	gameMap->update(time.asSeconds());
	context->systemManager->update(time.asSeconds());
}

void State_Game::updateCamera() {
	if (player == -1)
		return;
	SharedContext* context = stateManager->getContext();
	C_Position* pos = stateManager->getContext()->entityManager->getComponent<C_Position>(player, Component::Position);

	view.setCenter(pos->getPosition());
	context->window->getRenderWindow()->setView(view);

	sf::FloatRect viewSpace = context->window->getViewSpace();

	if (viewSpace.left <= 0) {
		view.setCenter(viewSpace.width / 2, view.getCenter().y);
		context->window->getRenderWindow()->setView(view);
	}
	else if (viewSpace.left + viewSpace.width > (gameMap->getMapSize().x) * Sheet::TileSize) {
		view.setCenter(((gameMap->getMapSize().x) * Sheet::TileSize) -
			(viewSpace.width / 2), view.getCenter().y);
		context->window->getRenderWindow()->setView(view);
	}

	if (viewSpace.top <= 0) {
		view.setCenter(view.getCenter().x, viewSpace.height / 2);
		context->window->getRenderWindow()->setView(view);
	}
	else if (viewSpace.top + viewSpace.height > (gameMap->getMapSize().y) * Sheet::TileSize) {
		view.setCenter(view.getCenter().x, ((gameMap->getMapSize().y) * Sheet::TileSize) - (viewSpace.height / 2));
		context->window->getRenderWindow()->setView(view);
	}

}
void State_Game::draw(){
	for (unsigned int i = 0; i < Sheet::NumLayers; ++i) {
		gameMap->draw(i);
		stateManager->getContext()->systemManager->draw(stateManager->getContext()->window, i);
	}
}

void State_Game::playerMove(EventDetails* details) {
	Message msg((MessageType)EntityMessage::Move);
	if (details->name == "Player_MoveLeft")
		msg.integer = (int)Direction::Left;
	else if (details->name == "Player_MoveRight")
		msg.integer = (int)Direction::Right;
	else if (details->name == "Player_MoveUp")
		msg.integer = (int)Direction::Up;
	else if (details->name == "Player_MoveDown")
		msg.integer = (int)Direction::Down;
	msg.receiver = player;
	stateManager->getContext()->systemManager->getMessageHandler()->dispatch(msg);
}

void State_Game::mainMenu(EventDetails* l_details){
	stateManager->switchTo(StateType::MainMenu); 
}

void State_Game::pause(EventDetails* l_details){
	stateManager->switchTo(StateType::Paused); 
}

void State_Game::activate(){}
void State_Game::deactivate(){}

void State_Game::toggleOverlay(EventDetails* details) {
	stateManager->getContext()->debugOverlay.setDebug(!stateManager->getContext()->debugOverlay.debug());
}