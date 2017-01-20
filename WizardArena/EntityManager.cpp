#include "EntityManager.hpp"
#include "SharedContext.hpp"

EntityManager::EntityManager(SharedContext* context, unsigned int maxEntities)
	:context(context), maxEntities(maxEntities), idCounter(0)
{
	loadEnemyTypes("EnemyList.list");
	registerEntity<Player>(EntityType::Player);
	registerEntity<Enemy>(EntityType::Enemy);
}

EntityManager::~EntityManager() {
	purge();
}

int EntityManager::add(const EntityType& type, const std::string& name) {
	auto itr = entityFactory.find(type);
	if (itr == entityFactory.end())
		return -1;
	EntityBase* entity = itr->second();
	entity->id = idCounter;
	if (name != "")
		entity->name = name;
	entities.emplace(idCounter, entity);
	if (type == EntityType::Enemy) {
		auto itr = enemyTypes.find(name);
		if (itr != enemyTypes.end()) {
			Enemy* enemy = (Enemy*)entity;
			enemy->load(itr->second);
		}
	}
	++idCounter;
	return idCounter - 1;
}

EntityBase* EntityManager::find(const std::string& name) {
	for (auto &itr : entities) {
		if (itr.second->getName() == name)
			return itr.second;
	}
	return nullptr;
}

EntityBase* EntityManager::find(unsigned int id) {
	auto itr = entities.find(id);
	if (itr == entities.end())
		return nullptr;
	return itr->second;
}

void EntityManager::remove(unsigned int id) {
	entitiesToRemove.emplace_back(id);
}

void EntityManager::update(float dT) {
	for (auto &itr : entities)
		itr.second->update(dT);
	entityCollisionCheck();
	processRemovals();
}

void EntityManager::draw() {
	sf::RenderWindow* win = context->window->getRenderWindow();
	sf::FloatRect viewSpace = context->window->getViewSpace();

	for (auto &itr : entities) {
		if (!viewSpace.intersects(itr.second->AABB))
			continue;
		itr.second->draw(win);
	}
}

void EntityManager::loadEnemyTypes(const std::string& name) {
	std::ifstream file;
	file.open(Utils::GetWorkingDirectory() + std::string("media/Characters/") + name);
	if (!file.is_open()) {
		std::cerr << "! Failed loading file: " << name << std::endl;
		return;
	}
	std::string line;
	while (std::getline(file, line)) {
		if (line[0] == '|')
			continue;
		std::stringstream keystream(line);
		std::string name;
		std::string charFile;
		keystream >> name >> charFile;
		enemyTypes.emplace(name, charFile);
	}
	file.close();
}

void EntityManager::entityCollisionCheck() {
	if (entities.empty())
		return;
	for (auto itr = entities.begin(); std::next(itr) != entities.end(); ++itr) {
		for (auto itr2 = std::next(itr); itr2 != entities.end(); ++itr2) {
			if (itr->first == itr2->first)
				continue;
			//Regular AABB bounding box collision
			if (itr->second->AABB.intersects(itr2->second->AABB)) {
				itr->second->onEntityCollision(itr2->second, false);
				itr2->second->onEntityCollision(itr->second, false);
			}

			EntityType t1 = itr->second->getType();
			EntityType t2 = itr2->second->getType();
			if (t1 == EntityType::Player || t1 == EntityType::Enemy) {
				Character* c1 = (Character*)itr->second;
				if (c1->attackAABB.intersects(itr2->second->AABB))
					c1->onEntityCollision(itr2->second, true);
			}

			if (t2 == EntityType::Player || t2 == EntityType::Enemy) {
				Character* c2 = (Character*)itr2->second;
				if (c2->attackAABB.intersects(itr->second->AABB))
					c2->onEntityCollision(itr->second, true);
			}
		}
	}
}

void EntityManager::processRemovals() {
	while (entitiesToRemove.begin() != entitiesToRemove.end()) {
		unsigned int id = entitiesToRemove.back();
		auto itr = entities.find(id);
		if (itr != entities.end()) {
			std::cout << "Discarding entity: " << itr->second->getId() << std::endl;
			delete itr->second;
			entities.erase(itr);
		}
		entitiesToRemove.pop_back();
	}
}

void EntityManager::purge() {
	for (auto &itr : entities)
		delete itr.second;
	entities.clear();
	idCounter = 0;
}

SharedContext* EntityManager::getContext() { return context; }