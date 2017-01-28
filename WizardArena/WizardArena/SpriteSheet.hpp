#pragma once
#include <unordered_map>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Directions.hpp"
#include "Anim_Directional.hpp"

class TextureManager;
using Animations = std::unordered_map<std::string, Anim_Base*>;

class SpriteSheet {

private:
	std::string texture;
	sf::Sprite sprite;
	sf::Vector2u spriteSize;
	sf::Vector2f spriteScale;
	sf::Vector2f sheetPadding;
	sf::Vector2f spriteSpacing;

	Direction direction;
	std::string animType;
	Animations animations;
	Anim_Base* animationCurrent;
	TextureManager* textureManager;

public:
	SpriteSheet(TextureManager* texManager);
	~SpriteSheet();

	void cropSprite(const sf::IntRect& rect);

	bool loadSheet(const std::string& file);
	void setSpriteSize(const sf::Vector2u& size);
	sf::Vector2u getSpriteSize();
	void setSheetPadding(const sf::Vector2f& padding);
	void setSpriteSpacing(const sf::Vector2f& spacing);
	const sf::Vector2f& getSheetPadding();
	const sf::Vector2f& getSpriteSpacing();

	void setSpritePosition(const sf::Vector2f& pos);
	void setDirection(const Direction& dir);
	Direction getDirection();
	void releaseSheet();

	Anim_Base* getCurrentAnim();
	bool setAnimation(const std::string& name, const bool& play = false, const bool& loop = false);

	void update(const float& dT);
	void draw(sf::RenderWindow* window);
};