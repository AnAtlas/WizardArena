#pragma once
#include "ResourceManager.hpp"
#include <SFML/Graphics.hpp>

class TextureManager : public ResourceManager<TextureManager, sf::Texture> {

public:
	TextureManager() : ResourceManager("textures.cfg") {

	}

	sf::Texture* load(const std::string& path) {
		sf::Texture* texture = new sf::Texture();
		if (!texture->loadFromFile(Utils::GetWorkingDirectory() + path)) {
			delete texture;
			texture = nullptr;
			std::cerr << "! Failed to laod texture: " << path << std::endl;
		}
		return texture;
	}
};