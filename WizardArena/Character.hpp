#pragma once
#include "EntityBase.hpp"
#include "SpriteSheet.hpp"

class Character : public EntityBase {
	friend class EntityManager;

protected:
	void updateAttackAABB();
	void animate();
	SpriteSheet spriteSheet;
	float jumpVelocity;
	int hitpoints;
	sf::FloatRect attackAABB;
	sf::Vector2f attackAABBoffset;

public:
	Character(EntityManager* entityMgr);
	virtual ~Character();
	void move(const Direction& dir);
	void jump();
	void attack();
	void getHurt(const int& damage);
	void load(const std::string& path);

	virtual void onEntityCollision(EntityBase* collider, bool attack) = 0;
	virtual void update(float dT);
	void draw(sf::RenderWindow* window);
};