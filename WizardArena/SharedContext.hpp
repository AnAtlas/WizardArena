#pragma once
#include "Window.hpp"
#include "EventManager.hpp"
#include "TextureManager.hpp"

class Map;
struct SharedContext{
	SharedContext() :
		window(nullptr),
		eventManager(nullptr),
		textureManager(nullptr) {}
	Window* window;
	EventManager* eventManager;
	TextureManager* textureManager;
	Map* gameMap;
};