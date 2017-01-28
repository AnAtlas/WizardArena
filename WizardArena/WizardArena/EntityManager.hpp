#pragma once
#include "Bitmask.hpp"
#include "C_Position.hpp"
#include "C_SpriteSheet.hpp"
//#include "C_State.hpp"
//#include "C_Movable.hpp"
//#include "C_Collidable.hpp"
#include "TextureManager.hpp"
#include <unordered_map>
#include <functional>
#include <vector>

using EntityId = unsigned int;

using ComponentContainer = std::vector<C_Base*>;
using EntityData = std::pair<Bitmask, ComponentContainer>;
using EntityContainer = std::unordered_map<EntityId, EntityData>;
using ComponentFactory = std::unordered_map<Component, std::function<C_Base*(void)>>;


class SystemManager;
class EntityManager {
private:
	template<class T>
	void addComponentType(const Component& id) {
		cFactory[id] = []()->C_Base* {
			return new T();
		};
	}

	unsigned int idCounter;
	EntityContainer entities;
	ComponentFactory cFactory;

	SystemManager* systems;
	TextureManager* textureManager;
public:
	EntityManager(SystemManager* sysMgr, TextureManager* textureMgr);
	~EntityManager();

	int addEntity(const Bitmask& mask);
	int addEntity(const std::string& entityFile);
	bool removeEntity(const EntityId& id);

	bool addComponent(const EntityId& entity, const Component& component);

	template<class T>
	T* getComponent(const EntityId& entity, const Component& component){
		auto itr = entities.find(entity);
		if (itr == entities.end())
			return nullptr;
		//Found the entity
		if (!itr->second.first.getBit((unsigned int)component))
			return nullptr;
		//Component exists
		auto& container = itr->second.second;
		auto aComponent = std::find_if(container.begin(), container.end(), [&component](C_Base* c) {
			return c->getType() == component;
		});
		return (aComponent != container.end() ? dynamic_cast<T*>(*aComponent) : nullptr);
	}

	bool removeComponent(const EntityId& entity, const Component& component);
	bool hasComponent(const EntityId& entity, const Component& component);

	void purge();
};