#include "S_Base.hpp"
#include "SystemManager.hpp"

S_Base::S_Base(const System& id, SystemManager* systemMgr)
	: id(id), systemManager(systemMgr)
{

}

S_Base::~S_Base() {
	purge();
}

bool S_Base::addEntity(const EntityId& entity) {
	if (hasEntity(entity))
		return false;
	entities.emplace_back(entity);
	return true;
}

bool S_Base::hasEntity(const EntityId& entity) {
	return std::find(entities.begin(), entities.end(), entity) != entities.end();
}

bool S_Base::removeEntity(const EntityId& lEntity) {
	auto entity = std::find_if(entities.begin(), entities.end(),
		[&lEntity](EntityId& id) {
		return id = lEntity;
	});
	if (entity == entities.end())
		return false;
	entities.erase(entity);
	return true;
}

bool S_Base::fitsRequirements(const Bitmask& bits) {
	return std::find_if(requiredComponents.begin(),
		requiredComponents.end(), [&bits](Bitmask& b) {
		return b.matches(bits, b.getMask());
	}) != requiredComponents.end();
}

void S_Base::purge() {
	entities.clear();
}