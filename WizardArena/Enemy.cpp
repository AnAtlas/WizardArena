#include "Enemy.hpp"

Enemy::Enemy(EntityManager* entityMgr)
	: Character(entityMgr), hasDestination(false)
{
	type == EntityType::Enemy;
}

Enemy::~Enemy() {

}

void Enemy::onEntityCollision(EntityBase* collider, bool attack) {
	if (state == EntityState::Dying)
		return;
	if (attack)
		return;
	if (collider->getType() != EntityType::Player)
		return;
	Character* player = (Character*)collider;
	setState(EntityState::Attacking);
	player->getHurt(1);
	if (position.x > player->getPosition().x) {
		player->addVelocity(-speed.x, 0);
		spriteSheet.setDirection(Direction::Left);
	}
	else {
		player->addVelocity(speed.x, 0);
		spriteSheet.setDirection(Direction::Right);
	}
}

void Enemy::update(float dT) {
	Character::update(dT);

	if (hasDestination) {
		if (abs(destination.x - position.x) < 16) {
			hasDestination = false;
			return;
		}
		if (destination.x - position.x > 0)
			move(Direction::Right);
		else
			move(Direction::Left);
		if (collidingOnX)
			hasDestination = false;
		return;
	}
	int random = rand() % 1000 + 1;
	if (random != 1000)
		return;
	int newX = rand() % 65 + 0;
	if (rand() % 2)
		newX = -newX;
	destination.x = position.x + newX;
	if (destination.x < 0)
		destination.x = 0;
	hasDestination = true;
}