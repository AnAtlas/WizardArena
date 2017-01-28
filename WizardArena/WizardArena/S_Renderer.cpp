#include "S_Renderer.hpp"
#include "SystemManager.hpp"

S_Renderer::S_Renderer(SystemManager* systemMgr)
	:S_Base(System::Renderer, systemMgr)
{
	Bitmask req;
	req.turnOnBit((unsigned int)Component::Position);
	req.turnOnBit((unsigned int)Component::SpriteSheet);
	requiredComponents.push_back(req);
	req.clear();

	systemManager->getMessageHandler()->subscribe(EntityMessage::Direction_Changed, this);
}

S_Renderer::~S_Renderer() {}

void S_Renderer::update(float dT) {
	EntityManager* lEntities = systemManager->getEntityManager();
	for (auto &entity : entities) {
		C_Position* position = lEntities->getComponent<C_Position>(entity, Component::Position);
		C_Drawable* drawable = nullptr;
		if (lEntities->hasComponent(entity, Component::SpriteSheet)) {
			drawable = lEntities->getComponent<C_Drawable>(entity, Component::SpriteSheet);
		}
		else {
			continue;
		}
		drawable->updatePosition(position->getPosition());
	}
}

void S_Renderer::handleEvent(const EntityId& entity, const EntityEvent& event) {
	if (event == EntityEvent::Moving_Left ||
		event == EntityEvent::Moving_Up ||
		event == EntityEvent::Moving_Down ||
		event == EntityEvent::Elevation_Change ||
		event == EntityEvent::Spawned)
		sortDrawables();
}

void S_Renderer::notify(const Message& message) {
	if (hasEntity(message.receiver)) {
		EntityMessage m = (EntityMessage)message.type;
		switch (m) {
		case EntityMessage::Direction_Changed:
			setSheetDirection(message.receiver, (Direction)message.integer);
			break;
		}
	}
}

void S_Renderer::render(Window* window, unsigned int layer) {
	EntityManager* lEntities = systemManager->getEntityManager();
	for (auto &entity : entities) {
		C_Position* position = lEntities->getComponent<C_Position>(entity, Component::Position);
		if (position->getElevation() < layer)
			continue;
		if (position->getElevation() > layer)
			break;
		C_Drawable* drawable = nullptr;
		if (!lEntities->hasComponent(entity, Component::SpriteSheet))
			continue;
		drawable = lEntities->getComponent<C_Drawable>(entity, Component::SpriteSheet);
		sf::FloatRect drawableBounds;
		drawableBounds.left = position->getPosition().x - (drawable->getSize().x / 2);
		drawableBounds.top = position->getPosition().y - drawable->getSize().y;
		drawableBounds.width = drawable->getSize().x;
		drawableBounds.height = drawable->getSize().y;
		if (!window->getViewSpace().intersects(drawableBounds))
			continue;
		drawable->draw(window->getRenderWindow());
	}
}

void S_Renderer::setSheetDirection(const EntityId& entity, const Direction& dir) {
	EntityManager* ents = systemManager->getEntityManager();
	if (!ents->hasComponent(entity, Component::SpriteSheet))
		return;
	C_SpriteSheet* sheet = ents->getComponent<C_SpriteSheet>(entity, Component::SpriteSheet);
	sheet->getSpriteSheet()->setDirection(dir);
}

void S_Renderer::sortDrawables() {
	EntityManager* mgr = systemManager->getEntityManager();
	std::sort(entities.begin(), entities.end(),
		[mgr](unsigned int l1, unsigned int l2)
	{
		auto pos1 = mgr->getComponent<C_Position>(l1, Component::Position);
		auto pos2 = mgr->getComponent<C_Position>(l2, Component::Position);
		if (pos1->getElevation() == pos2->getElevation()) {
			return pos1->getPosition().y < pos2->getPosition().y;
		}
		return pos1->getElevation() < pos2->getElevation();
	});
}

