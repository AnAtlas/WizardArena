#pragma once
#include <memory>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


class StateManager;
class EventManager;

enum class StateType {
	MainMenu = 1, PlayerTurn, OtherPlayerTurn, Spectating, Paused
};

struct SharedContext {
	SharedContext() : window(nullptr), eventManager(nullptr) {}
	std::shared_ptr<sf::RenderWindow> window;
	std::shared_ptr<EventManager> eventManager;
};
class BaseState {
	friend class StateManager;

protected:
	std::shared_ptr<StateManager> stateManager;
	bool transparent;
	bool transcendent;

public:
	BaseState(std::shared_ptr<StateManager> stateManager)
		:stateManager(stateManager), transparent(false), transcendent(false) {}
	virtual ~BaseState();

	virtual void onCreate() = 0;
	virtual void onDestroy() = 0;
	virtual void activate() = 0; 
	virtual void deactivate() = 0;

	virtual void update(const sf::Time time) = 0;
	virtual void draw() = 0;

	void setTransparent(const bool transparent) {
		this->transparent = transparent;
	}
	bool isTransparent() const { return transparent; }
	
	void setTranscendent(const bool transcendence) {
		this->transcendent = transcendence;
	}
	bool isTranscendent() const { return transcendent; }

	std::shared_ptr<StateManager> getStateManager() { return stateManager; }
};