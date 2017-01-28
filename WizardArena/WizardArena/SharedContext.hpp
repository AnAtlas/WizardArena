#pragma once
#include "Window.hpp"
#include "EventManager.hpp"
#include "TextureManager.hpp"
#include "EntityManager.hpp"
#include "DebugOverlay.hpp"
#include "SystemManager.hpp"

class Map;
struct SharedContext{
	SharedContext() :
		window(nullptr),
		eventManager(nullptr),
		textureManager(nullptr),
		entityManager(nullptr),
		systemManager(nullptr),
		gameMap(nullptr) {}
	Window* window;
	EventManager* eventManager;
	TextureManager* textureManager;
	EntityManager* entityManager;
	SystemManager* systemManager;
	Map* gameMap;
	DebugOverlay debugOverlay;
};