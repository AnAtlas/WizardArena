#include "State_Intro.hpp"
#include "StateManager.hpp"

State_Intro::State_Intro(StateManager* stateManager)
	: BaseState(stateManager), timeLength(2){}

State_Intro::~State_Intro(){}

void State_Intro::onCreate(){
	timePassed = 0.0f;

	sf::Vector2u windowSize = stateMgr->getContext()->window->getRenderWindow()->getSize();

	introTexture.loadFromFile("intro.png");
	introSprite.setTexture(introTexture);
	introSprite.setOrigin(introTexture.getSize().x / 2.0f,
		introTexture.getSize().y);

	introSprite.setPosition(windowSize.x / 2.0f, 0);

	font.loadFromFile("arial.ttf");
	text.setFont(font);
	text.setString({ "Press SPACE to continue" });
	text.setCharacterSize(15);
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	text.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	EventManager* evMgr = stateMgr->getContext()->eventManager;
	evMgr->addCallback(StateType::Intro,"Intro_Continue",&State_Intro::Continue,this);
}

void State_Intro::onDestroy(){
	EventManager* evMgr = stateMgr->getContext()->eventManager;
	evMgr->removeCallback(StateType::Intro,"Intro_Continue");
}

void State_Intro::update(const sf::Time& l_time){
	if(timePassed < timeLength){ // Less than five seconds.
		timePassed += l_time.asSeconds();
		introSprite.setPosition(
			introSprite.getPosition().x,
			300 * (timePassed/timeLength));
	}
}

void State_Intro::draw(){
	sf::RenderWindow* window = stateMgr->getContext()->window->getRenderWindow();

	window->draw(introSprite);
	if(timePassed >= timeLength){
		window->draw(text);
	}
}

void State_Intro::Continue(EventDetails* details){
	if(timePassed >= timeLength){
		stateMgr->switchTo(StateType::MainMenu);
		stateMgr->remove(StateType::Intro);
	}
}

void State_Intro::activate(){}
void State_Intro::deactivate(){}