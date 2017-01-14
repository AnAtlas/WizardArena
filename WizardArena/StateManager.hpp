#pragma once
#include <vector>
#include <unordered_map>
#include "State_Intro.hpp"
#include "State_MainMenu.hpp"
#include "State_Game.hpp"
#include "State_Paused.hpp"
#include "SharedContext.hpp"

enum class StateType{ Intro = 1, MainMenu, Game, Paused, GameOver, Credits };

// State container.
using StateContainer = std::vector<std::pair<StateType, BaseState*>>;
// Type container.
using TypeContainer = std::vector<StateType>;
// State factory.
using StateFactory = std::unordered_map<StateType, std::function<BaseState*(void)>>;

class StateManager{
public:
	StateManager(SharedContext* l_shared);
	~StateManager();

	void update(const sf::Time& l_time);
	void draw();

	void processRequests();

	SharedContext* getContext();
	bool hasState(const StateType& l_type);

	void switchTo(const StateType& l_type);
	void remove(const StateType& l_type);
private:
	// Methods.
	void createState(const StateType& l_type);
	void removeState(const StateType& l_type);

	template<class T>
	void registerState(const StateType& l_type){
		stateFactory[l_type] = [this]() -> BaseState*
		{
			return new T(this);
		};
	}

	// Members.
	SharedContext* shared;
	StateContainer states;
	TypeContainer toRemove;
	StateFactory stateFactory;
};