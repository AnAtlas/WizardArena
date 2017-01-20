#include "EntityBase.hpp"
#include "Map.hpp"

bool sortCollisions(const CollisionElement& e1, const CollisionElement& e2) {
	return e1.area > e2.area;
}

EntityBase::EntityBase(EntityManager* entityMgr)
	:entityManager(entityMgr), name("BaseEntity"), type(EntityType::Base),
	referenceTile(nullptr), state(EntityState::Idle), id(0), collidingOnX(false), collidingOnY(false)
{

}

EntityBase::~EntityBase() {

}

void EntityBase::setPosition(float& x, float& y) {
	position = sf::Vector2f(x, y);
	updateAABB();
}

void EntityBase::setPosition(const sf::Vector2f& pos) {
	position = pos;
	updateAABB();
}

void EntityBase::setSize(float& x, float& y) {
	size = sf::Vector2f(x, y);
	updateAABB();
}

void EntityBase::setAcceleration(float x, float y) {
	acceleration = sf::Vector2f(x, y);
}

void EntityBase::setState(const EntityState& state) {
	if (this->state == EntityState::Dying)
		return;
	this->state = state;
}

const sf::Vector2f& EntityBase::getSize() { return size; }
std::string EntityBase::getName() { return name; }
EntityState EntityBase::getState() { return state; }
unsigned int EntityBase::getId() { return id; }
EntityType EntityBase::getType() { return type; }
const sf::Vector2f& EntityBase::getPosition() { return position; }

void EntityBase::move(float x, float y) {
	positionOld = position;
	position += sf::Vector2f(x, y);
	sf::Vector2u mapSize = entityManager->getContext()->gameMap->getMapSize();
	if (position.x < 0)
		position.x = 0;
	else if (position.x > (mapSize.x + 1) * Sheet::TileSize) {
		position.x = (mapSize.x + 1) * Sheet::TileSize;
	}

	if (position.y < 0) 
		position.y = 0;
	else if (position.y > (mapSize.y + 1) * Sheet::TileSize) {
		position.y = (mapSize.y + 1) * Sheet::TileSize;
		setState(EntityState::Dying);
	}
	updateAABB();
}

void EntityBase::addVelocity(float x, float y) {
	velocity += sf::Vector2f(x, y);
	if (abs(velocity.x) > maxVelocity.x) {
		if (velocity.x < 0)
			velocity.x = -maxVelocity.x;
		else
			velocity.x = maxVelocity.x;
	}
	if (abs(velocity.y) > maxVelocity.y) {
		if (velocity.y < 0)
			velocity.y = -maxVelocity.y;
		else
			velocity.y = maxVelocity.y;
	}
}

void EntityBase::accelerate(float x, float y) {
	acceleration += sf::Vector2f(x, y);
}

void EntityBase::applyFriction(float x, float y) {
	if (velocity.x != 0) {
		if (abs(velocity.x) - abs(x) < 0)
			velocity.x = 0;
		else {
			if (velocity.x < 0)
				velocity.x += x;
			else
				velocity.x -= x;
		}
	}
	if (velocity.y != 0) {
		if (abs(velocity.y) - abs(y) < 0)
			velocity.y = 0;
		else {
			if (velocity.y < 0)
				velocity.y += y;
			else
				velocity.y -= y;
		}
	}
}

void EntityBase::update(float dT) {
	Map* map = entityManager->getContext()->gameMap;
	float gravity = map->getGravity();
	accelerate(0, gravity);
	addVelocity(acceleration.x * dT, acceleration.y * dT);
	setAcceleration(0.0f, 0.0f);

	sf::Vector2f frictionValue;
	if (referenceTile) {
		frictionValue = referenceTile->friction;
		if (referenceTile->deadly)
			setState(EntityState::Dying);
	}
	else if (map->getDefaultTile()) {
		frictionValue = map->getDefaultTile()->friction;
	}
	else
		frictionValue = friction;

	float frictionX = (speed.x * frictionValue.x) * dT;
	float frictionY = (speed.y * frictionValue.y) * dT;
	applyFriction(frictionX, frictionY);
	sf::Vector2f deltaPos = velocity * dT;
	move(deltaPos.x, deltaPos.y);
	collidingOnX = false;
	collidingOnY = false;
	checkCollisions();
	resolveCollisions();
}

void EntityBase::updateAABB() {
	AABB = sf::FloatRect(position.x - (size.x / 2), position.y - size.y, size.x, size.y);
}

void EntityBase::checkCollisions() {
	Map* gameMap = entityManager->getContext()->gameMap;
	unsigned int tileSize = gameMap->getTileSize();
	int fromX = floor(AABB.left / tileSize);
	int toX = floor(AABB.left + AABB.width) / tileSize;
	int fromY = floor(AABB.top / tileSize);
	int toY = floor(AABB.top + AABB.height) / tileSize;

	for (int x = fromX; x <= toX; ++x) {
		for (int y = fromY; y <= toY; ++y) {
			Tile* tile = gameMap->getTile(x, y);
			if (!tile)
				continue;
			sf::FloatRect tileBounds(x * tileSize, y * tileSize, tileSize, tileSize);
			sf::FloatRect intersection;
			AABB.intersects(tileBounds, intersection);
			float area = intersection.width * intersection.height;

			CollisionElement e(area, tile->properties, tileBounds);
			collisions.emplace_back(e);
			if (tile->warp && type == EntityType::Player) {
				gameMap->loadNext();
			}
		}
	}
}

void EntityBase::resolveCollisions() {
	if (!collisions.empty()) {
		std::sort(collisions.begin(), collisions.end(), sortCollisions);
		Map* gameMap = entityManager->getContext()->gameMap;
		unsigned int tileSize = gameMap->getTileSize();
		for (auto &itr : collisions) {
			if (!AABB.intersects(itr.tileBounds))
				continue;
			//Debug
			if (entityManager->getContext()->debugOverlay.debug()) {
				sf::Vector2f tempPos(itr.tileBounds.left, itr.tileBounds.top);
				sf::RectangleShape* rect = new sf::RectangleShape(sf::Vector2f(tileSize, tileSize));
				rect->setPosition(tempPos);
				rect->setFillColor(sf::Color(255, 255, 0, 150));
				entityManager->getContext()->debugOverlay.add(rect);
			}//end debug
			float xDiff = (AABB.left + (AABB.width / 2)) - (itr.tileBounds.left + (itr.tileBounds.width / 2));
			float yDiff = (AABB.top + (AABB.height / 2)) - (itr.tileBounds.top + (itr.tileBounds.height / 2));
			float resolve = 0;
			if (abs(xDiff) > abs(yDiff)) {
				if (xDiff > 0) {
					resolve = (itr.tileBounds.left + tileSize) - AABB.left;
				}
				else {
					resolve = -((itr.tileBounds.left + AABB.width) - itr.tileBounds.left);
				}
				move(resolve, 0);
				velocity.x = 0;
				collidingOnX = true;
			}
			else {
				if (yDiff > 0) {
					resolve = (itr.tileBounds.top + tileSize) - AABB.top;
				}
				else {
					resolve = -((AABB.top + AABB.height) - itr.tileBounds.top);
				}
				move(0, resolve);
				velocity.y = 0;
				if (collidingOnY)
					continue;
				referenceTile = itr.tile;
				collidingOnY = true;
			}
		}
		collisions.clear();
	}
	if (!collidingOnY)
		referenceTile = nullptr;
}