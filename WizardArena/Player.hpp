#pragma once
#include "Character.hpp"
#include "EventManager.hpp"

class Player : public Character {
public:
	Player(EntityManager* entityMgr);
	~Player();

	void onEntityCollision(EntityBase* collider, bool attack);
	void react(EventDetails* details);
};