#pragma once
#include "C_Drawable.hpp"
#include "SpriteSheet.hpp"

class C_SpriteSheet : public C_Drawable {
private:
	SpriteSheet* spriteSheet;
	std::string sheetName;

public:
	C_SpriteSheet() : C_Drawable(Component::SpriteSheet), spriteSheet(nullptr) {}
	~C_SpriteSheet() { if (spriteSheet) { delete spriteSheet; } }

	void readIn(std::stringstream& stream) {
		stream >> sheetName;
	}

	void create(TextureManager* textureMgr, const std::string& name = "") {
		if (spriteSheet)
			return;
		spriteSheet = new SpriteSheet(textureMgr);
		spriteSheet->loadSheet("media/Spritesheets/" + (name != "" ? name : sheetName) + ".sheet");
	}

	SpriteSheet* getSpriteSheet() {
		return spriteSheet;
	}

	void updatePosition(const sf::Vector2f& vec) {
		spriteSheet->setSpritePosition(vec);
	}

	const sf::Vector2u& getSize() {
		return spriteSheet->getSpriteSize();
	}

	void draw(sf::RenderWindow* window) {
		if (!spriteSheet)
			return;
		spriteSheet->draw(window);
	}
};