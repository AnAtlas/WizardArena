#pragma once
#include <vector>
#include "SFML/Graphics.hpp"

class DebugOverlay {
private:
	std::vector<sf::Drawable*> drawables;
	bool debugging;

public:
	DebugOverlay() {
		debugging = false;
	}

	void add(sf::Drawable* drawable) {
		drawables.push_back(drawable);
	}

	void draw(sf::RenderWindow* window) {
		while (drawables.begin() != drawables.end()) {
			window->draw(*drawables.back());
			delete drawables.back();
			drawables.pop_back();
		}
	}

	bool debug() {
		return debugging;
	}

	void setDebug(const bool& val) {
		debugging = val;
	}
};