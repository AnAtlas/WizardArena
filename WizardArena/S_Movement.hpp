#pragma once
#include "S_Base.hpp"

enum class Axis{x,y};
class Map;

class S_Movement : public S_Base {
private:
	Map* gameMap;
	void stopEntity(const EntityId& entity, const Axis& axis);
	void setDirection(const EntityId& entity, const Direction& dir);
	const sf::Vector2f& getTileFriction(unsigned int elevation, unsigned int x, unsigned int y);
	void movementStep(float dT, C_Movable* movable, C_Position* position);

public:
	S_Movement(SystemManager* systemMgr);
	~S_Movement();

	void update(float dT);
	void handleEvent(const EntityId& entity, const EntityEvent& event);
	void notify(const Message& message);

	void setMap(Map* gameMap);
};