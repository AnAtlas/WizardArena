#pragma once
#include "S_Base.hpp"

class S_SheetAnimation : public S_Base {
private:
	void changeAnimation(const EntityId& entity, const std::string& anim, bool play, bool loop);

public:
	S_SheetAnimation(SystemManager* systemMgr);
	~S_SheetAnimation();

	void update(float dT);
	void notify(const Message& message);
	void handleEvent(const EntityId& entity, const EntityEvent& event);
};