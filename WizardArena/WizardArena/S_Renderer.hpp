#pragma once
#include "S_Base.hpp"
#include "Window.hpp"

class S_Renderer : public S_Base {
private:
	void setSheetDirection(const EntityId& entity, const Direction& dir);
	void sortDrawables();

public:
	S_Renderer(SystemManager* systemMgr);
	~S_Renderer();

	void update(float dT);
	void handleEvent(const EntityId& entity, const EntityEvent& event);
	void notify(const Message& message);
	void render(Window* window, unsigned int layer);
};