#pragma once
#include <SFML/Graphics.hpp>
class StateManager;

class BaseState{
	friend class StateManager;
public:
	BaseState(StateManager* stateManager):stateMgr(stateManager),
		transparent(false), transcendent(false){}
	virtual ~BaseState(){}

	virtual void onCreate() = 0;
	virtual void onDestroy() = 0;

	virtual void activate() = 0;
	virtual void deactivate() = 0;

	virtual void update(const sf::Time& time) = 0;
	virtual void draw() = 0;

	void setTransparent(const bool& l_transparent){ transparent = l_transparent; }
	bool isTransparent()const{ return transparent; }
	void setTranscendent(const bool& l_transcendence){ transcendent = l_transcendence; }
	bool isTranscendent()const{ return transcendent; }
	StateManager* getStateManager(){ return stateMgr; }
	sf::View& getView() { return view; }

protected:
	StateManager* stateMgr;
	sf::View view;
	bool transparent;
	bool transcendent;
};