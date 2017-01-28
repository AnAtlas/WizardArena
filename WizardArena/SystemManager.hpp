#pragma once
#include <unordered_map>
#include "S_Renderer.hpp"
#include "S_Movement.hpp"
#include "S_Controller.hpp"
#include "S_SheetAnimation.hpp"
#include "S_State.hpp"
#include "S_Collision.hpp"
#include "EventQueue.hpp"
#include "MessageHandler.hpp"
#include "Window.hpp"
#include "DebugOverlay.hpp"

using SystemContainer = std::unordered_map<System, S_Base*>;
using EntityEventContainer = std::unordered_map<EntityId, EventQueue>;

class EntityManager;
class SystemManager {
private:
	SystemContainer systems;
	EntityManager* entityManager;
	EntityEventContainer events;
	MessageHandler messages;

public:
	SystemManager();
	~SystemManager();

	void setEntityManager(EntityManager* entityMgr);
	EntityManager* getEntityManager();
	MessageHandler* getMessageHandler();

	template<class T>
	T* getSystem(const System& system) {
		auto itr = systems.find(system);
		return (itr != systems.end() ? dynamic_cast<T*>(itr->second) : nullptr);
	}

	void addEvent(const EntityId& entity, const EventID& event);

	void update(float dT);
	void handleEvents();
	void draw(Window* window, unsigned int elevation);

	void entityModified(const EntityId& entity, const Bitmask& bits);
	void removeEntity(const EntityId& entity);

	void purgeEntities();
	void purgeSystems();
};