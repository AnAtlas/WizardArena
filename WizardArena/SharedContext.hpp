#pragma once
#include "Window.hpp"
#include "EventManager.hpp"
#include "TextureManager.hpp"
#include "EntityManager.hpp"
#include "DebugOverlay.hpp"
#include "SystemManager.hpp"
#include "FontManager.hpp"
#include "GUI_Manager.hpp"


class Map;
struct SharedContext{
	SharedContext() :
		window(nullptr),
		eventManager(nullptr),
		fontManager(nullptr),
		guiManager(nullptr),
		textureManager(nullptr),
		entityManager(nullptr),
		systemManager(nullptr),
		gameMap(nullptr) {}
	Window* window;
	EventManager* eventManager;
	TextureManager* textureManager;
	EntityManager* entityManager;
	SystemManager* systemManager;
	FontManager* fontManager;
	GUI_Manager* guiManager;
	Map* gameMap;
	DebugOverlay debugOverlay;
};