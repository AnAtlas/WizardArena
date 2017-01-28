#include "S_Collision.hpp"
#include "SystemManager.hpp"


S_Collision::S_Collision(SystemManager* systemMgr)
	:S_Base(System::Collision, systemMgr)
{
	Bitmask req;
	req.turnOnBit((unsigned int)Component::Position);
	req.turnOnBit((unsigned int)Component::Collidable);
	requiredComponents.push_back(req);
	req.clear();

	gameMap = nullptr;
}

void S_Collision::setMap(Map* map) {
	gameMap = map;
}

void S_Collision::update(float dT) {
	if (!gameMap)
		return;
	EntityManager* ents = systemManager->getEntityManager();
	for (auto &entity : entities) {
		C_Position* position = ents->getComponent<C_Position>(entity, Component::Position);
		C_Collidable* collidable = ents->getComponent<C_Collidable>(entity, Component::Collidable);
		collidable->setPosition(position->getPosition());
		collidable->resetCollisionFlags();
		checkOutOfBounds(position, collidable);
		mapCollisions(entity, position, collidable);
	}
	entityCollisions();
}