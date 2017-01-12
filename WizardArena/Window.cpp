#include "Window.hpp"


Window::Window() {
	setup("Window", sf::Vector2u(640, 480));
}

Window::Window(const std::string title, const sf::Vector2u size) {
	setup(title, size);
}

Window::~Window() {
	destroy();
}

void Window::setup(const std::string title, const sf::Vector2u size) {
	windowTitle = title;
	windowSize = size;
	fullscreen = false;
	done = false;
	create();
}

void Window::create() {
	auto style = (fullscreen ? sf::Style::Fullscreen : sf::Style::Default);
	window.create({ windowSize.x, windowSize.y, 32 }, windowTitle, style);
}

void Window::destroy() {
	window.close();
}

void Window::update() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			done = true;
		}
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F5)
			toggleFullscreen();
	}
}

void Window::toggleFullscreen() {
	fullscreen = !fullscreen;
	destroy();
	create();
}

void Window::beginDraw() {
	window.clear(sf::Color::Black);
}

void Window::endDraw() {
	window.display();
}

bool Window::isDone() {
	return done;
}

bool Window::isFullscreen() {
	return fullscreen;
}

sf::Vector2u Window::getWindowSize() {
	return windowSize;
}

void Window::draw(sf::Drawable& drawable) {
	window.draw(drawable);
}