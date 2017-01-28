#pragma once
#include "C_Base.hpp"
#include <SFML/Graphics/Rect.hpp>

enum class Origin{Top_Left, Abs_Center, Mid_Bottom};

class C_Collidable : public C_Base {
private:
	sf::FloatRect boundingBox;
	sf::Vector2f offset;
	Origin origin;

	bool collidingOnX;
	bool collidingOnY;

public:
	C_Collidable() : C_Base(Component::Collidable),
		origin(Origin::Mid_Bottom), collidingOnX(false),collidingOnY(false)
	{		
	}
	virtual ~C_Collidable() {}
	
	void readIn(std::stringstream& stream) {
		unsigned int orig = 0;
		stream >> boundingBox.width >> boundingBox.height >> offset.x >> offset.y >> orig;
		origin = (Origin)orig;
	}

	void collideOnX() {
		collidingOnX = true;
	}

	void collideOnY() {
		collidingOnY = true;
	}

	void resetCollisionFlags() {
		collidingOnX = false;
		collidingOnY = false;
	}

	void setSize(const sf::Vector2f& vec) {
		boundingBox.width = vec.x;
		boundingBox.height = vec.y;
	}

	void setPosition(const sf::Vector2f& vec) {
		switch (origin) {
		case(Origin::Top_Left):
			boundingBox.left = vec.x + offset.x;
			boundingBox.top = vec.y + offset.y;
			break;
		case(Origin::Abs_Center):
			boundingBox.left = vec.x - (boundingBox.width / 2) + offset.x;
			boundingBox.top = vec.y - (boundingBox.height / 2) + offset.y;
			break;
		case(Origin::Mid_Bottom):
			boundingBox.left = vec.x - (boundingBox.width / 2) + offset.x;
			boundingBox.top = vec.y - boundingBox.height + offset.y;
			break;
		}
	}
};