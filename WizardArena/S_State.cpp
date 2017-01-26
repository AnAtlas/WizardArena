#include "S_State.hpp"
#include "SystemManager.hpp"

S_State::S_State(SystemManager* systemMgr)
	: S_Base(System::State, systemMgr)
{
	Bitmask req;
	req.turnOnBit((unsigned int)Component::State);
	requiredComponents.push_back(req);

	systemManager->getMessageHandler()->subscribe(EntityMessage::State_Changed, this);
	systemManager->getMessageHandler()->subscribe(EntityMessage::Move, this);
}

S_State::~S_State() {

}

void S_State::update(float dT) {
	EntityManager* ents = systemManager->getEntityManager();
	for (auto &entity : entities) {
		C_State* aState = ents->getComponent<C_State>(entity, Component::State);
		if (aState->getState() == EntityState::Walking) {
			Message msg((MessageType)EntityMessage::Is_Moving);
			msg.receiver = entity;
			systemManager->getMessageHandler()->dispatch(msg);
		}
	}
}

void S_State::handleEvent(const EntityId& entity, const EntityEvent& event) {
	switch (event) {
	case EntityEvent::Became_Idle:
		changeState(entity, EntityState::Idle, false);
		break;
	}
}

void S_State::notify(const Message& message) {
	if (!hasEntity(message.receiver))
		return;
	EntityMessage m = (EntityMessage)message.type;
	switch (m) {
	case EntityMessage::Move: {
		C_State* s = systemManager->getEntityManager()->getComponent<C_State>(message.receiver, Component::State);
		if (s->getState() == EntityState::Dying)
			return;
		EntityEvent e;
		if (message.integer == (int)Direction::Up)
			e = EntityEvent::Moving_Up;
		else if (message.integer == (int)Direction::Down)
			e = EntityEvent::Moving_Down;
		else if (message.integer == (int)Direction::Left)
			e = EntityEvent::Moving_Left;
		else if (message.integer == (int)Direction::Right)
			e = EntityEvent::Moving_Right;

		systemManager->addEvent(message.receiver, (EventID)e);
		changeState(message.receiver, EntityState::Walking, false);
	} break;

	case EntityMessage::Switch_State:
		changeState(message.receiver, (EntityState)message.integer, false);
		break;
	}
}

void S_State::changeState(const EntityId& entity, const EntityState& state, const bool& force) {
	EntityManager* ents = systemManager->getEntityManager();
	C_State* s = ents->getComponent<C_State>(entity, Component::State);
	if (!force && s->getState() == EntityState::Dying)
		return;
	s->setState(state);
	Message msg((MessageType)EntityMessage::State_Changed);
	msg.receiver = entity;
	msg.integer = (int)state;
	systemManager->getMessageHandler()->dispatch(msg);
}