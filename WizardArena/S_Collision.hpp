#pragma once
#include "S_Base.hpp"

struct TileInfo;
class Map;

struct CollisionElement {
	CollisionElement(float area, TileInfo* info, const sf::FloatRect& bounds)
		: area(area), tile(info), tileBounds(bounds) {}

	float area;
	TileInfo* tile;
	sf::FloatRect tileBounds;
};

using Collisions = std::vector<CollisionElement>;

class S_Collision : public S_Base {
private:
	void checkOutOfBounds(C_Position* pos, C_Collidable* col);
	void mapCollisions(const EntityId& entity, C_Position* pos, C_Collidable* col);
	void entityCollisions();

	Map* gameMap;

public:
	S_Collision(SystemManager* systemMgr);
	~S_Collision();

	void setMap(Map* map);
	void update(float dT);
	void handleEvent(const EntityId& entity, const EntityEvent& event);
	void notify(const Message& message);
};