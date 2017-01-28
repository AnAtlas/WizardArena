#include "State_Paused.hpp"
#include "StateManager.hpp"

State_Paused::State_Paused(StateManager* stateManager)
	: BaseState(stateManager){}

State_Paused::~State_Paused(){}

void State_Paused::onCreate(){
	setTransparent(true); // Set our transparency flag.
	font.loadFromFile("arial.ttf");
	text.setFont(font);
	text.setString(sf::String("PAUSED"));
	text.setCharacterSize(14);
	text.setStyle(sf::Text::Bold);

	sf::Vector2u windowSize = stateManager->getContext()->window->getRenderWindow()->getSize();

	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	rect.setSize(sf::Vector2f(windowSize));
	rect.setPosition(0,0);
	rect.setFillColor(sf::Color(0,0,0,150));

	EventManager* evMgr = stateManager->getContext()->eventManager;
	evMgr->addCallback(StateType::Paused,"Key_P",&State_Paused::unpause,this);
}

void State_Paused::onDestroy(){
	EventManager* evMgr = stateManager->getContext()->eventManager;
	evMgr->removeCallback(StateType::Paused,"Key_P");
}

void State_Paused::draw(){
	sf::RenderWindow* wind = stateManager->getContext()->window->getRenderWindow();
	wind->draw(rect);
	wind->draw(text);
}

void State_Paused::unpause(EventDetails* details){
	stateManager->switchTo(StateType::Game);
}

void State_Paused::activate(){}
void State_Paused::deactivate(){}
void State_Paused::update(const sf::Time& time){}