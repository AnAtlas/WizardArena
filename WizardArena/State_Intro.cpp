#include "State_Intro.hpp"
#include "StateManager.hpp"

State_Intro::State_Intro(StateManager* stateManager)
	: BaseState(stateManager){}

State_Intro::~State_Intro(){}

void State_Intro::onCreate(){

	sf::Vector2u windowSize = stateManager->getContext()->window->getRenderWindow()->getSize();

	TextureManager* textureMgr = stateManager->getContext()->textureManager;
	textureMgr->requireResource("Intro");
	introSprite.setTexture(*textureMgr->getResource("Intro"));
	introSprite.setOrigin(textureMgr->getResource("Intro")->getSize().x / 2.0f,
							textureMgr->getResource("Intro")->getSize().y / 2.0f);

	introSprite.setPosition(windowSize.x / 2.0f, windowSize.y / 2.0f);

	font.loadFromFile(Utils::GetWorkingDirectory() + "media/Fonts/arial.ttf");
	text.setFont(font);
	text.setString({ "Press SPACE to continue" });
	text.setCharacterSize(15);
	text.setFillColor(sf::Color::White);
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
	text.setPosition(introSprite.getPosition().x, introSprite.getPosition().y + textureMgr->getResource("Intro")->getSize().y / 1.5f);

	EventManager* evMgr = stateManager->getContext()->eventManager;
	evMgr->addCallback(StateType::Intro,"Intro_Continue",&State_Intro::Continue,this);
}

void State_Intro::onDestroy(){
	TextureManager* texMgr = stateManager->getContext()->textureManager;
	texMgr->releaseResource("Intro");

	EventManager* evMgr = stateManager->getContext()->eventManager;
	evMgr->removeCallback(StateType::Intro,"Intro_Continue");
}

void State_Intro::update(const sf::Time& l_time){
}

void State_Intro::draw(){
	sf::RenderWindow* window = stateManager->getContext()->window->getRenderWindow();

	window->draw(introSprite);
	window->draw(text);
}

void State_Intro::Continue(EventDetails* details){
	stateManager->switchTo(StateType::MainMenu);
	stateManager->remove(StateType::Intro);
}

void State_Intro::activate(){}
void State_Intro::deactivate(){}