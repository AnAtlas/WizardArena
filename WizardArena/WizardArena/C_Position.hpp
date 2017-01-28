#pragma once
#include "C_Base.hpp"
#include <SFML/System/Vector2.hpp>

class C_Position : public C_Base {
private:
	sf::Vector2f position;
	sf::Vector2f positionOld;
	unsigned int elevation;

public:
	C_Position() : C_Base(Component::Position), elevation(0) {}
	~C_Position() {}

	void readIn(std::stringstream& stream) {
		stream >> position.x >> position.y >> elevation;
	}

	const sf::Vector2f& getPosition() { return position; }
	const sf::Vector2f& getOldPosition() { return positionOld; }
	unsigned int getElevation() { return elevation; }

	void setPosition(float x, float y) {
		positionOld = position;
		position = sf::Vector2f(x, y);
	}

	void setPosition(const sf::Vector2f& vec) {
		positionOld = position;
		position = vec;
	}

	void setElevation(unsigned int elev) {
		elevation = elev;
	}

	void moveBy(float x, float y) {
		positionOld = position;
		position += sf::Vector2f(x, y);
	}

	void moveBy(const sf::Vector2f& vec) {
		positionOld = position;
		position += vec;
	}
};