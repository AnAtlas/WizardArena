#include "State_Game.hpp"
#include "StateManager.hpp"

State_Game::State_Game(StateManager* stateManager)
	: BaseState(stateManager){}

State_Game::~State_Game(){}

void State_Game::onCreate(){
	texture.loadFromFile("Mushroom.png");
	sprite.setTexture(texture);
	sprite.setPosition(0,0);
	increment = sf::Vector2f(400.0f,400.0f);

	EventManager* evMgr = stateMgr->getContext()->eventManager;
	evMgr->addCallback(StateType::Game,"Key_Escape",&State_Game::mainMenu,this);
	evMgr->addCallback(StateType::Game,"Key_P",&State_Game::pause,this);
}

void State_Game::onDestroy(){
	EventManager* evMgr = stateMgr->getContext()->eventManager;
	evMgr->removeCallback(StateType::Game,"Key_Escape");
	evMgr->removeCallback(StateType::Game,"Key_P");
}

void State_Game::update(const sf::Time& time){
	sf::Vector2u l_windSize = stateMgr->getContext()->window->getWindowSize();
	sf::Vector2u l_textSize = texture.getSize();

	if((sprite.getPosition().x > l_windSize.x - l_textSize.x && increment.x > 0) ||
		(sprite.getPosition().x < 0 && increment.x < 0))
	{
			increment.x = -increment.x;
	}

	if((sprite.getPosition().y > l_windSize.y - l_textSize.y && increment.y > 0) ||
		(sprite.getPosition().y < 0 && increment.y < 0))
	{
			increment.y = -increment.y;
	}

	sprite.setPosition(sprite.getPosition().x + (increment.x * time.asSeconds()), 
		sprite.getPosition().y + (increment.y * time.asSeconds()));
}

void State_Game::draw(){
	stateMgr->getContext()->window->getRenderWindow()->draw(sprite);
}

void State_Game::mainMenu(EventDetails* l_details){
	stateMgr->switchTo(StateType::MainMenu); 
}

void State_Game::pause(EventDetails* l_details){
	stateMgr->switchTo(StateType::Paused); 
}

void State_Game::activate(){}
void State_Game::deactivate(){}