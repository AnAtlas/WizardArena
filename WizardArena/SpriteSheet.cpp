#include "SpriteSheet.hpp"
#include "TextureManager.hpp"
#include <fstream>
#include "Utilities.hpp"

SpriteSheet::SpriteSheet(TextureManager* textMgr) :textureManager(textMgr), animationCurrent(nullptr),
	spriteScale(1.f, 1.f), direction(Direction::Right) 
{
	
}

SpriteSheet::~SpriteSheet() {
	releaseSheet();
}

bool SpriteSheet::loadSheet(const std::string& file) {
	std::ifstream sheet;
	sheet.open(Utils::GetWorkingDirectory() + file);
	if (sheet.is_open()) {
		releaseSheet(); //release current sheet resources
		std::string line;
		while (std::getline(sheet, line)) {
			if (line[0] == '|')
				continue;
			std::stringstream keystream(line);
			std::string type;
			keystream >> type;
			
			if (type == "Texture") {
				if (texture != "") {
					std::cerr << "! Duplicate texture entries in: " << file << std::endl;
					continue;
				}
				std::string texture;
				keystream >> texture;
				if (!textureManager->requireResource(texture)) {
					std::cerr << "! Could not set up texture: " << texture << std::endl;
					continue;
				}
				texture = texture;
				sprite.setTexture(*textureManager->getResource(texture));
			}
			else if (type == "Size") {
				keystream >> spriteSize.x >> spriteSize.y;
				setSpriteSize(spriteSize);
			}
			else if (type == "Scale") {
				keystream >> spriteScale.x >> spriteScale.y;
				sprite.setScale(spriteScale);
			}
			else if (type == "AnimationType") {
				keystream >> animType;
			}
			else if (type == "Animation") {
				std::string name; 
				keystream >> name;
				if (animations.find(name) != animations.end()) {
					std::cerr << "! Duplicate animation(" << name << ") in " << file << std::endl;
					continue;
				}
				Anim_Base* anim = nullptr;
				if (animType == "Directional") {
					anim = new Anim_Directional();
				}
				else {
					std::cerr << "! Unknown animation type: " << animType << std::endl;
					continue;
				}
				keystream >> *anim;
				anim->setSpriteSheet(this);
				anim->setName(name);
				anim->reset();
				animations.emplace(name, anim);

				if (animationCurrent)
					continue;
				animationCurrent = anim;
				animationCurrent->play();
			}

		}
		sheet.close();
		return true;
	}
	std::cerr << "! Failed loading spritesheet : " << file << std::endl;
	return false;
}

void SpriteSheet::releaseSheet() {
	textureManager->releaseResource(texture);
	animationCurrent = nullptr;
	while (animations.begin() != animations.end()) {
		delete animations.begin()->second;
		animations.erase(animations.begin());
	}
}

void SpriteSheet::setSpriteSize(const sf::Vector2i& size) {
	spriteSize = size;
	sprite.setOrigin(spriteSize.x / 2, spriteSize.y);
}

void SpriteSheet::setSpritePosition(const sf::Vector2f& pos) {
	sprite.setPosition(pos);
}

void SpriteSheet::setDirection(const Direction& dir) {
	if (dir == direction)
		return;
	direction = dir;
	animationCurrent->cropSprite();
}

void SpriteSheet::cropSprite(const sf::IntRect& rect) {
	sprite.setTextureRect(rect);
}

Anim_Base* SpriteSheet::getCurrentAnim() {
	return animationCurrent;
}

bool SpriteSheet::setAnimation(const std::string& name, const bool& play, const bool& loop) {
	auto itr = animations.find(name);
	if (itr == animations.end())
		return false;
	if (itr->second == animationCurrent)
		return false;
	if (animationCurrent)
		animationCurrent->stop();
	animationCurrent = itr->second;
	animationCurrent->setLooping(loop);
	if (play)
		animationCurrent->play();
	animationCurrent->cropSprite();
	return true;
}

sf::Vector2i SpriteSheet::getSpriteSize() {
	return spriteSize;
}

Direction SpriteSheet::getDirection() {
	return direction;
}

void SpriteSheet::update(const float& dT) {
	animationCurrent->update(dT);
}

void SpriteSheet::draw(sf::RenderWindow* window) {
	window->draw(sprite);
}