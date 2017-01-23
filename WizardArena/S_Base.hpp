#pragma once
#include <vector>
#include "Bitmask.hpp"
#include "EntityManager.hpp"
#include "EventQueue.hpp"
#include "EntityEvents.hpp"
#include "ECS_Types.hpp"
#include "Observer.hpp"

using EntityList = std::vector<EntityId>;
using Requirements = std::vector<Bitmask>;

class SystemManager;
class S_Base : public Observer{
protected:
	System id;
	Requirements requiredComponents;
	EntityList entities;

	SystemManager* systemManager;

public:
	S_Base(const System& id, SystemManager* systemMgr);
	virtual ~S_Base();

	bool addEntity(const EntityId& entity);
	bool hasEntity(const EntityId& entity);
	bool removeEntity(const EntityId& entity);

	System getId();

	bool fitsRequirements(const Bitmask& bits);
	void purge();

	virtual void update(float dT) = 0;
	virtual void handleEvent(const EntityId& entity, const EntityEvent& event) = 0;
};