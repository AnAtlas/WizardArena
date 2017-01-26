#include "S_Movement.hpp"
#include "SystemManager.hpp"
#include "Map.hpp"

S_Movement::S_Movement(SystemManager* systemMgr)
	: S_Base(System::Movement, systemMgr)
{
	Bitmask req;
	req.turnOnBit((unsigned int)Component::Position);
	req.turnOnBit((unsigned int)Component::Movable);
	requiredComponents.push_back(req);
	req.clear();

	systemManager->getMessageHandler()->subscribe(EntityMessage::Is_Moving, this);

	gameMap = nullptr;
}

S_Movement::~S_Movement() {

}

void S_Movement::update(float dT) {
	if (!gameMap)
		return;
	EntityManager* ents = systemManager->getEntityManager();
	for (auto &entity : entities) {
		C_Position* position = ents->getComponent<C_Position>(entity, Component::Position);
		C_Movable* movable = ents->getComponent<C_Movable>(entity, Component::Movable);
		movementStep(dT, movable, position);
		position->moveBy(movable->getVelocity() * dT);
	}
}

void S_Movement::movementStep(float dT, C_Movable* movable, C_Position* position) {
	sf::Vector2f coefficient = getTileFriction(position->getElevation(),
		floor(position->getPosition().x / Sheet::TileSize),
		floor(position->getPosition().y / Sheet::TileSize));

	sf::Vector2f friction(movable->getSpeed().x * coefficient.x,
		movable->getSpeed().y * coefficient.y);

	movable->addVelocity(movable->getAcceleration() * dT);
	movable->setAcceleration(sf::Vector2f(0.0f, 0.0f));
	movable->applyFriction(friction * dT);

	float magnitude = sqrt((movable->getVelocity().x * movable->getVelocity().x) +
		(movable->getVelocity().y * movable->getVelocity().y));

	if (magnitude <= movable->getMaxVelocity())
		return;
	float maxV = movable->getMaxVelocity();
	movable->setVelocity(sf::Vector2f((movable->getVelocity().x / magnitude) * maxV,
		(movable->getVelocity().y / magnitude) * maxV));
}

void S_Movement::handleEvent(const EntityId& entity, const EntityEvent& event) {
	switch (event) {
	case EntityEvent::Colliding_X:
		stopEntity(entity, Axis::x);
		break;
	case EntityEvent::Colliding_Y:
		stopEntity(entity, Axis::y);
		break;
	case EntityEvent::Moving_Left:
		setDirection(entity, Direction::Left);
		break;
	case EntityEvent::Moving_Right:
		setDirection(entity, Direction::Right);
		break;
	case EntityEvent::Moving_Up:
	{
		C_Movable* mov = systemManager->getEntityManager()->getComponent<C_Movable>(entity, Component::Movable);
		if (mov->getVelocity().x == 0)
			setDirection(entity, Direction::Up);
	}break;
	case EntityEvent::Moving_Down:
	{
		C_Movable* mov = systemManager->getEntityManager()->getComponent<C_Movable>(entity, Component::Movable);
		if (mov->getVelocity().x == 0)
			setDirection(entity, Direction::Down);
	}break;
	}
}

void S_Movement::notify(const Message& message) {
	EntityManager* eMgr = systemManager->getEntityManager();
	EntityMessage m = (EntityMessage)message.type;
	switch (m) {
	case EntityMessage::Is_Moving:
	{
		if (!hasEntity(message.receiver))
			return;
		C_Movable* movable = eMgr->getComponent<C_Movable>(message.receiver, Component::Movable);
		if (movable->getVelocity() != sf::Vector2f(0.0f, 0.0f))
			return;
		systemManager->addEvent(message.receiver, (EventID)EntityEvent::Became_Idle);
	}break;
	}
}

void S_Movement::stopEntity(const EntityId& entity, const Axis& axis) {
	C_Movable* movable = systemManager->getEntityManager()->getComponent<C_Movable>(entity, Component::Movable);
	if (axis == Axis::x)
		movable->setVelocity(sf::Vector2f(0.0f, movable->getVelocity().y));
	else if (axis == Axis::y)
		movable->setVelocity(sf::Vector2f(movable->getVelocity().x, 0.0f));
}

void S_Movement::setDirection(const EntityId& entity, const Direction& dir) {
	C_Movable* movable = systemManager->getEntityManager()->getComponent<C_Movable>(entity, Component::Movable);
	movable->setDirection(dir);

	Message msg((MessageType)EntityMessage::Direction_Changed);
	msg.receiver = entity;
	msg.integer = (int)dir;
	systemManager->getMessageHandler()->dispatch(msg);
}

void S_Movement::setMap(Map* map) {
	gameMap = map;
}

const sf::Vector2f& S_Movement::getTileFriction(unsigned int elevation, unsigned int x, unsigned int y) {
	Tile* t = nullptr;
	while (!t && elevation >= 0) {
		t = gameMap->getTile(x, y, elevation);
		--elevation;
	}

	return (t ? t->properties->friction : gameMap->getDefaultTile()->friction);
}