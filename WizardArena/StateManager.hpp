#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include "BaseState.hpp"
#include "State_MainMenu.hpp"

using TypeContainer = std::vector<StateType>;
using StateContainer = std::vector<std::pair<StateType, std::shared_ptr<BaseState>>>;
using StateFactory = std::unordered_map<StateType, std::function<std::shared_ptr<BaseState>>(void)>;

class StateManager {
private:
	//Methods
	void createState(const StateType type);
	void removeState(const StateType type);

	template<class T>
	void registerState(const StateType type) {
		stateFactory[type] = [this]()->BaseState* {
			return new T(this);
		}
	}

	//Members
	std::shared_ptr<SharedContext> sharedContext;
	StateContainer states;
	TypeContainer toRemove;
	StateFactory stateFactory;

public:
	StateManager(std::shared_ptr<SharedContext> shared);
	~StateManager();

	void update(const sf::Time time);
	void draw();

	void processRequests();

	std::shared_ptr<SharedContext> getContext();
	bool hasState(const StateType type);

	void switchTo(const StateType type);
	void remove(const StateType type);
};