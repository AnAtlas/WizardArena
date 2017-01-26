#pragma once
#include"S_Base.hpp"

class S_Controller : public S_Base {
private:
	void moveEntity(const EntityId& entity, const Direction& dir);

public:
	S_Controller(SystemManager* systemMgr);
	~S_Controller();

	void update(float dT);
	void handleEvent(const EntityId& entity, const EntityEvent& event);
	void notify(const Message& message);
};