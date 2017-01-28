#pragma once
#include "C_Base.hpp"
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\RenderWindow.hpp>

class C_Drawable : public C_Base {
public:
	C_Drawable(const Component& type) : C_Base(type) {}
	virtual ~C_Drawable() {}

	virtual void updatePosition(const sf::Vector2f& vec) = 0;
	virtual const sf::Vector2u& getSize() = 0;
	virtual void draw(sf::RenderWindow* window) = 0;
};