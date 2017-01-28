#include "EntityManager.hpp"
#include "SharedContext.hpp"

EntityManager::EntityManager(SystemManager* sysMgr, TextureManager* textureMgr)
	:idCounter(0), systems(sysMgr), textureManager(textureMgr)
{
	addComponentType<C_Position>(Component::Position);
	addComponentType<C_SpriteSheet>(Component::SpriteSheet);
	addComponentType<C_State>(Component::State);
	addComponentType<C_Movable>(Component::Movable);
	addComponentType<C_Controller>(Component::Controller);
	addComponentType<C_Collidable>(Component::Collidable);
}

EntityManager::~EntityManager() {
	purge();
}

int EntityManager::addEntity(const Bitmask& mask) {
	unsigned int entity = idCounter;
	if (!entities.emplace(entity, EntityData(0, ComponentContainer())).second)
		return -1;
	++idCounter;
	for (unsigned int i = 0; i < N_COMPONENT_TYPES; ++i) {
		if (mask.getBit(i))
			addComponent(entity, (Component)i);
	}
	// Notifying the system manager of a modified entity
	systems->entityModified(entity, mask);
	systems->addEvent(entity, (EventID)EntityEvent::Spawned);
	return entity;
}

int EntityManager::addEntity(const std::string& entityFile) {
	int entityId = -1;

	std::ifstream file;
	file.open(Utils::GetWorkingDirectory() + "media/Entities/" + entityFile + ".entity");
	if (!file.is_open()) {
		std::cerr << "! Failed to load entity: " << entityFile << std::endl;
		return -1;
	}
	std::string line;
	while (std::getline(file, line)) {
		if (line[0] == '|')
			continue;
		std::stringstream keystream(line);
		std::string type;
		keystream >> type;
		if (type == "Name") {

		}
		else if (type == "Attributes") {
			if (entityId != -1)
				continue;
			Bitset set = 0;
			Bitmask mask;
			keystream >> set;
			mask.setMask(set);
			entityId = addEntity(mask);
			if (entityId == -1)
				return -1;
		}
		else if (type == "Component") {
			if (entityId == -1)
				continue;
			unsigned int c_id = 0;
			keystream >> c_id;
			C_Base* component = getComponent<C_Base>(entityId, (Component)c_id);
			if (!component)
				continue;
			keystream >> *component;

			if (component->getType() == Component::SpriteSheet) {
				C_SpriteSheet* sheet = (C_SpriteSheet*)component;
				sheet->create(textureManager);
			}
		}
	}
	file.close();
	return entityId;
}

bool EntityManager::removeEntity(const EntityId& id) {
	auto itr = entities.find(id);
	if (itr == entities.end())
		return false;
	//Removing all components
	while (itr->second.second.begin() != itr->second.second.end()) {
		delete itr->second.second.back();
		itr->second.second.pop_back();
	}
	entities.erase(itr);
	systems->removeEntity(id);
	return true;
}

bool EntityManager::addComponent(const EntityId& entity, const Component& component) {
	auto itr = entities.find(entity);
	if (itr == entities.end())
		return false;
	if (itr->second.first.getBit((unsigned int)component))
		return false;
	//Component doesn't exist
	auto itr2 = cFactory.find(component);
	if (itr2 == cFactory.end())
		return false;
	//Component type does exist
	C_Base* comp = itr2->second();
	itr->second.second.emplace_back(comp);
	itr->second.first.turnOnBit((unsigned int)component);
	//Notify system manager of modified entity
	systems->entityModified(entity, itr->second.first);
	return true;
}

bool EntityManager::removeComponent(const EntityId& entity, const Component& lComponent) {
	auto itr = entities.find(entity);
	if (itr == entities.end())
		return false;
	//Found entity
	if (!itr->second.first.getBit((unsigned int)lComponent))
		return false;
	//Component exists
	auto& container = itr->second.second;
	auto component = std::find_if(container.begin(), container.end(),
		[lComponent](C_Base* c) {
		return c->getType() == lComponent;
	});
	if (component == container.end())
		return false;
	delete(*component);
	container.erase(component);
	itr->second.first.clearBit((unsigned int)lComponent);

	systems->entityModified(entity, itr->second.first);
	return true;
}

bool EntityManager::hasComponent(const EntityId& entity, const Component& component) {
	auto itr = entities.find(entity);
	if (itr == entities.end())
		return false;
	return itr->second.first.getBit((unsigned int)component);
}

void EntityManager::purge() {
	systems->purgeEntities();
	if (entities.size() > 0) {
		for (auto& entity : entities) {
			for (auto &component : entity.second.second) {
				delete component;
			}
			entity.second.second.clear();
			entity.second.first.clear();
		}
		entities.clear();
	}
	
	idCounter = 0;
}