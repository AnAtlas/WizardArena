#pragma once
#include "Character.hpp"

class Enemy : public Character {
private:
	sf::Vector2f destination;
	bool hasDestination;

public:
	Enemy(EntityManager* entityMgr);
	~Enemy();

	void onEntityCollision(EntityBase* collider, bool attack);
	void update(float dT);
};