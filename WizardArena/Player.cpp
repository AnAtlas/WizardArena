#include "Player.hpp"
#include "EntityManager.hpp"
#include "StateManager.hpp"

Player::Player(EntityManager* entityMgr)
	:Character(entityMgr)
{
	load("Player.char");
	type = EntityType::Player;

	EventManager* events = entityManager->getContext()->eventManager;
	events->addCallback<Player>(StateType::Game, "Player_MoveLeft", &Player::react, this);
	events->addCallback<Player>(StateType::Game, "Player_MoveRight", &Player::react, this);
	events->addCallback<Player>(StateType::Game, "Player_Jump", &Player::react, this);
	events->addCallback<Player>(StateType::Game, "Player_Attack", &Player::react, this);
}

Player::~Player() {
	EventManager* events = entityManager->getContext()->eventManager;
	events->removeCallback(StateType::Game, "Player_MoveLeft");
	events->removeCallback(StateType::Game, "Player_MoveRight");
	events->removeCallback(StateType::Game, "Player_Jump");
	events->removeCallback(StateType::Game, "Player_Attack");
}

void Player::react(EventDetails* details) {
	if (details->name == "Player_MoveLeft")
		Character::move(Direction::Left);
	else if (details->name == "Player_MoveRight")
		Character::move(Direction::Right);
	else if (details->name == "Player_Jump")
		Character::jump();
	else if (details->name == "Player_Attack")
		Character::attack();
}
void Player::onEntityCollision(EntityBase* collider, bool attack) {
	if (state == EntityState::Dying)
		return;
	if (attack) {
		if (state != EntityState::Attacking)
			return;
		if (!spriteSheet.getCurrentAnim()->isInAction())
			return;
		if (collider->getType() != EntityType::Enemy && collider->getType() != EntityType::Player)
			return;
		Character* opponent = (Character*)collider;
		opponent->getHurt(1);
		if (position.x > opponent->getPosition().x)
			opponent->addVelocity(-32, 0);
		else
			opponent->addVelocity(32, 0);
	}
	else {
		//Other Behavior
	}
}