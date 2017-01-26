#pragma once
#include "C_Base.hpp"
#include "Directions.hpp"
#include <SFML/System/Vector2.hpp>

class C_Movable : public C_Base {
private:
	sf::Vector2f velocity;
	float velocityMax;
	sf::Vector2f speed;
	sf::Vector2f acceleration;
	Direction direction;

public:
	C_Movable() : C_Base(Component::Movable), velocityMax(0.f), direction((Direction)0) {}

	void readIn(std::stringstream& stream) {
		stream >> velocityMax >> speed.x >> speed.y;

		unsigned int dir = 0;
		stream >> dir;
		direction = (Direction)dir;
	}

	void addVelocity(const sf::Vector2f& vec) {
		velocity += vec;
		if (std::abs(velocity.x) > velocityMax) {
			velocity.x = velocityMax * (velocity.x / std::abs(velocity.x));
		}

		if (std::abs(velocity.y) > velocityMax) {
			velocity.y = velocityMax * (velocity.y / std::abs(velocity.y));
		}
	}

	void applyFriction(const sf::Vector2f& vec) {
		if (velocity.x != 0 && vec.x != 0) {
			if (std::abs(velocity.x) - std::abs(vec.x) < 0)
				velocity.x = 0;
			else
				velocity.x += (velocity.x > 0 ? vec.x * -1 : vec.x);
		}

		if (velocity.y != 0 && vec.y != 0) {
			if (std::abs(velocity.y) - std::abs(vec.y) < 0)
				velocity.y = 0;
			else
				velocity.y += (velocity.y > 0 ? vec.y * -1 : vec.y);
		}
	}

	void accelerate(const sf::Vector2f& vec) {
		acceleration += vec;
	}

	void accelerate(float x, float y) {
		acceleration += sf::Vector2f(x, y);
	}

	void move(const Direction& dir) {
		if (dir == Direction::Up)
			acceleration.y -= speed.y;
		else if (dir == Direction::Down)
			acceleration.y += speed.y;
		else if (dir == Direction::Left)
			acceleration.x -= speed.x;
		else if (dir == Direction::Right)
			acceleration.x += speed.x;
	}

	const sf::Vector2f& getVelocity() { return velocity; }
	float getMaxVelocity() { return velocityMax; }
	const sf::Vector2f& getSpeed() { return speed; }
	const sf::Vector2f& getAcceleration() { return acceleration; }
	Direction getDirection() { return direction; }

	void setVelocity(const sf::Vector2f& vec) {velocity = vec; }
	void setMaxVelocity(float vel) { velocityMax = vel; }
	void setSpeed(const sf::Vector2f& vec) { speed = vec; }
	void setAcceleration(const sf::Vector2f& vec) { acceleration = vec; }
	void setDirection(const Direction& dir) { direction = dir; }
};