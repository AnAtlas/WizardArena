#pragma once
#include "Window.hpp"
#include "EventManager.hpp"

struct SharedContext{
	SharedContext()
		:window(nullptr),
		eventManager(nullptr){}
	Window* window;
	EventManager* eventManager;
};