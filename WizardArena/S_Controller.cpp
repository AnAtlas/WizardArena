#include "S_Controller.hpp"
#include "SystemManager.hpp"

S_Controller::S_Controller(SystemManager* systemMgr)
	: S_Base(System::Control, systemMgr)
{
	Bitmask req;
	req.turnOnBit((unsigned int)Component::Position);
	req.turnOnBit((unsigned int)Component::Movable);
	req.turnOnBit((unsigned int)Component::Controller);
	requiredComponents.push_back(req);
	req.clear();
}

S_Controller::~S_Controller() {

}

void S_Controller::update(float dT) {

}

void S_Controller::notify(const Message& l_message) {

}

void S_Controller::handleEvent(const EntityId& entity, const EntityEvent& event) {
	switch (event) {
	case EntityEvent::Moving_Left:
		moveEntity(entity, Direction::Left); break;
	case EntityEvent::Moving_Right:
		moveEntity(entity, Direction::Right); break;
	case EntityEvent::Moving_Up:
		moveEntity(entity, Direction::Up); break;
	case EntityEvent::Moving_Down:
		moveEntity(entity, Direction::Down); break;
	}
}

void S_Controller::moveEntity(const EntityId& entity, const Direction& dir) {
	C_Movable* mov = systemManager->getEntityManager()->getComponent<C_Movable>(entity, Component::Movable);
	mov->move(dir);
}