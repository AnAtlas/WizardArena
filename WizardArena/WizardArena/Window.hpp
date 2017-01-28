#pragma once
#include <string>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "EventManager.hpp"

class Window{
public:
	Window();
	Window(const std::string& title, const sf::Vector2u& size);
	~Window();

	void beginDraw();
	void endDraw();

	void update();

	bool isDone();
	bool isFullscreen();
	bool isFocused();

	void toggleFullscreen(EventDetails* details);
	void close(EventDetails* details = nullptr);

	sf::RenderWindow* getRenderWindow();
	EventManager* getEventManager();
	sf::Vector2u getWindowSize();
	sf::FloatRect getViewSpace();

private:
	void setup(const std::string& title, const sf::Vector2u& size);
	void create();

	sf::RenderWindow window;
	EventManager eventManager;
	sf::Vector2u windowSize;
	std::string windowTitle;
	bool done;
	bool fullscreen;
	bool isFocus;
};