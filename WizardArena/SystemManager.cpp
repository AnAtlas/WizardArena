#include "SystemManager.hpp"
#include "EntityManager.hpp"

SystemManager::SystemManager() 
	: entityManager(nullptr) 
{
	systems[System::State] = new S_State(this);
	systems[System::Control] = new S_Controller(this);
	systems[System::Movement] = new S_Movement(this);
	//systems[System::Collision] = new S_Collision(this);
	systems[System::SheetAnimation] = new S_SheetAnimation(this);
	systems[System::Renderer] = new S_Renderer(this);
}

SystemManager::~SystemManager() {
	purgeSystems();
}

void SystemManager::setEntityManager(EntityManager* entityMgr) {
	if (!entityManager)
		entityManager = entityMgr;
}

EntityManager* SystemManager::getEntityManager() {
	return entityManager;
}

MessageHandler* SystemManager::getMessageHandler() {
	return &messages;
}

void SystemManager::addEvent(const EntityId& entity, const EventID& event) {
	events[entity].addEvent(event);
}

void SystemManager::entityModified(const EntityId& entity, const Bitmask& bits) {
	for (auto &itr : systems) {
		S_Base* system = itr.second;
		if (system->fitsRequirements(bits)) {
			if (!system->hasEntity(entity)) {
				system->addEntity(entity);
			}
		}
		else {
			if (system->hasEntity(entity)) {
				system->removeEntity(entity);
			}
		}
	}
}

void SystemManager::removeEntity(const EntityId& entity) {
	for (auto &system : systems) {
		system.second->removeEntity(entity);
	}
}

void SystemManager::purgeEntities() {
	if (systems.size() <= 0)
		return;
	for (auto &system : systems) {
		system.second->purge();
	}
}

void SystemManager::purgeSystems() {
	if (systems.size() > 0) {
		for (auto &system : systems) {
			delete system.second;
		}
	}
	systems.clear();
}

void SystemManager::update(float dT) {
	for (auto &itr : systems) {
		itr.second->update(dT);
	}
	handleEvents();
}



void SystemManager::handleEvents() {
	for (auto &event : events) {
		EventID id = 0;
		while (event.second.processEvents(id)) {
			for (auto &system : systems) {
				if (system.second->hasEntity(event.first))
					system.second->handleEvent(event.first, (EntityEvent)id);
			}
		}
	}
}

void SystemManager::draw(Window* window, unsigned int elevation) {
	auto itr = systems.find(System::Renderer);
	if (itr == systems.end())
		return;
	S_Renderer* system = (S_Renderer*)itr->second;
	system->render(window, elevation);
}