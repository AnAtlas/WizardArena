#pragma once
#include <unordered_map>
#include <functional>
#include "Player.hpp"
#include "Enemy.hpp"

using EntityContainer = std::unordered_map<unsigned int, EntityBase*>;
using EntityFactory = std::unordered_map<EntityType, std::function<EntityBase*(void)>>;
using EnemyTypes = std::unordered_map<std::string, std::string>;

struct SharedContext;

class EntityManager {

private:
	template<class T>
	void registerEntity(const EntityType& type) {
		entityFactory[type] = [this]() -> EntityBase*
		{
			return new T(this);
		};
	}

	void processRemovals();
	void loadEnemyTypes(const std::string& name);
	void entityCollisionCheck();

	EntityContainer entities;
	EnemyTypes enemyTypes;
	EntityFactory entityFactory;
	SharedContext* context;
	unsigned int idCounter;
	unsigned int maxEntities;

	std::vector<unsigned int> entitiesToRemove;

public:
	EntityManager(SharedContext* context, unsigned int maxEntities);
	~EntityManager();

	int add(const EntityType& type, const std::string& name = "");
	EntityBase* find(unsigned int id);
	EntityBase* find(const std::string& name);
	void remove(unsigned int id);

	void update(float dT);
	void draw();

	void purge();

	SharedContext* getContext();
};