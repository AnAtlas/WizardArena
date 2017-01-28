#include "S_Collision.hpp"
#include "SystemManager.hpp"
#include "Map.hpp"

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

S_Collision::~S_Collision() {

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

void S_Collision::handleEvent(const EntityId& entity, const EntityEvent& event) {

}

void S_Collision::notify(const Message& message) {

}

void S_Collision::checkOutOfBounds(C_Position* pos, C_Collidable* col) {
	unsigned int tileSize = gameMap->getTileSize();

	if (pos->getPosition().x < 0) {
		pos->setPosition(0.0f, pos->getPosition().y);
		col->setPosition(pos->getPosition());
	}
	else if (pos->getPosition().x > gameMap->getMapSize().x * tileSize) {
		pos->setPosition(gameMap->getMapSize().x * tileSize, pos->getPosition().y);
		col->setPosition(pos->getPosition());
	}
	
	if (pos->getPosition().y < 0) {
		pos->setPosition(pos->getPosition().x, 0.0f);
		col->setPosition(pos->getPosition());
	}
	else if (pos->getPosition().y > gameMap->getMapSize().y * tileSize) {
		pos->setPosition(pos->getPosition().x, gameMap->getMapSize().y * tileSize);
		col->setPosition(pos->getPosition());
	}
}

void S_Collision::mapCollisions(const EntityId& entity, C_Position* pos, C_Collidable* col) {
	unsigned int tileSize = gameMap->getTileSize();
	Collisions c;

	sf::FloatRect entBoundingBox = col->getCollidable();
	int fromX = floor(entBoundingBox.left / tileSize);
	int toX = floor((entBoundingBox.left + entBoundingBox.width) / tileSize);
	int fromY = floor(entBoundingBox.top / tileSize);
	int toY = floor((entBoundingBox.top + entBoundingBox.height) / tileSize);

	for (int x = fromX; x <= toX; ++x) {
		for (int y = fromY; y <= toY; ++y) {
			for (int l = 0; l < Sheet::NumLayers; ++l) {
				Tile* t = gameMap->getTile(x, y, l);
				if (!t)
					continue;
				if (!t->solid)
					continue;
				sf::FloatRect tileBoundingBox(x * tileSize, y* tileSize, tileSize, tileSize);
				sf::FloatRect intersection;
				entBoundingBox.intersects(tileBoundingBox, intersection);
				float s = intersection.width * intersection.height;
				c.emplace_back(s, t->properties, tileBoundingBox);
			}
		}
	}

	if (c.empty())
		return;
	std::sort(c.begin(), c.end(),
		[](CollisionElement& l1, CollisionElement& l2) {
		return l1.area > l2.area;
	});

	for (auto &collision : c) {
		entBoundingBox = col->getCollidable();
		if (!entBoundingBox.intersects(collision.tileBounds))
			continue;
		float xDiff = (entBoundingBox.left + (entBoundingBox.width / 2)) - (collision.tileBounds.left + (collision.tileBounds.width / 2));
		float yDiff = (entBoundingBox.top + (entBoundingBox.height / 2)) - (collision.tileBounds.top + (collision.tileBounds.height / 2));
		float resolve = 0;
		if (std::abs(xDiff) > std::abs(yDiff)) {
			if (xDiff > 0)
				resolve = (collision.tileBounds.left + tileSize) - entBoundingBox.left;
			else
				resolve = -((entBoundingBox.left + entBoundingBox.width) - collision.tileBounds.left);
			pos->moveBy(resolve, 0);
			col->setPosition(pos->getPosition());
			systemManager->addEvent(entity, (EventID)EntityEvent::Colliding_X);
			col->collideOnX();
		}
		else {
			if (yDiff > 0)
				resolve = (collision.tileBounds.top + tileSize) - entBoundingBox.top;
			else
				resolve = -((entBoundingBox.top + entBoundingBox.height) - collision.tileBounds.top);
			pos->moveBy(0, resolve);
			col->setPosition(pos->getPosition());
			systemManager->addEvent(entity, (EventID)EntityEvent::Colliding_Y);
			col->collideOnY();
		}
	}
}

void S_Collision::entityCollisions() {
	EntityManager* ents = systemManager->getEntityManager();
	for (auto itr = entities.begin(); itr != entities.end(); ++itr) {
		for (auto itr2 = std::next(itr); itr2 != entities.end(); ++itr2) {
			C_Collidable* collidable1 = ents->getComponent<C_Collidable>(*itr, Component::Collidable);
			C_Collidable* collidable2 = ents->getComponent<C_Collidable>(*itr2, Component::Collidable);
			if (collidable1->getCollidable().intersects(collidable2->getCollidable())) {
				//Entity on Entity Collisions
			}
		}
	}
}