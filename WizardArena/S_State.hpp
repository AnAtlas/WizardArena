#pragma once
#include "S_Base.hpp"
#include "C_State.hpp"

class S_State : public S_Base {
private:
	void changeState(const EntityId& entity, const EntityState& stat, const bool& force);

public:
	S_State(SystemManager* systemMgr);
	~S_State();

	void update(float dT);
	void handleEvent(const EntityId& entity, const EntityEvent& event);
	void notify(const Message& message);
};