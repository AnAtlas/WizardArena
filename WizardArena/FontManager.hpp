#pragma once
#include "ResourceManager.hpp"
#include <SFML/Graphics/Text.hpp>

class FontManager : public ResourceManager<FontManager, sf::Font> {

public:
	FontManager() : ResourceManager("fonts.cfg") {

	}
	sf::Font* load(const std::string& path) {
		sf::Font* font = new sf::Font();
		if (!font->loadFromFile(Utils::GetWorkingDirectory() + path)) {
			delete font;
			font = nullptr;
			std::cerr << "! Failed to load font: " << path << std::endl;
		}
		return font;
	}
};