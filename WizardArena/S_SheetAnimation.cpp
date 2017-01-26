#include "S_SheetAnimation.hpp"
#include "SystemManager.hpp"

S_SheetAnimation::S_SheetAnimation(SystemManager* systemMgr)
	: S_Base(System::SheetAnimation, systemMgr)
{
	Bitmask req;
	req.turnOnBit((unsigned int)Component::SpriteSheet);
	req.turnOnBit((unsigned int)Component::State);
	requiredComponents.push_back(req);

	systemManager->getMessageHandler()->subscribe(EntityMessage::State_Changed, this);
}

S_SheetAnimation::~S_SheetAnimation() {

}

void S_SheetAnimation::update(float dT) {
	EntityManager* ents = systemManager->getEntityManager();
	for (auto &entity : entities) {
		C_SpriteSheet* sheet = ents->getComponent<C_SpriteSheet>(entity, Component::SpriteSheet);
		C_State* state = ents->getComponent<C_State>(entity, Component::State);

		sheet->getSpriteSheet()->update(dT);

		const std::string& animName = sheet->getSpriteSheet()->getCurrentAnim()->getName();

		if (animName == "Attack") {
			if (!sheet->getSpriteSheet()->getCurrentAnim()->isPlaying()) {
				Message msg((MessageType)EntityMessage::Switch_State);
				msg.receiver = entity;
				msg.integer = (int)EntityState::Idle;
				systemManager->getMessageHandler()->dispatch(msg);
			}
			else if (sheet->getSpriteSheet()->getCurrentAnim()->isInAction()) {
				Message msg((MessageType)EntityMessage::Attack_Action);
				msg.sender = entity;
				systemManager->getMessageHandler()->dispatch(msg);
			}
		}
		else if (animName == "Death" && !sheet->getSpriteSheet()->getCurrentAnim()->isPlaying()) {
			Message msg((MessageType)EntityMessage::Dead);
			msg.receiver = entity;
			systemManager->getMessageHandler()->dispatch(msg);
		}
	}
}

void S_SheetAnimation::notify(const Message& message) {
	if (hasEntity(message.receiver)) {
		EntityMessage m = (EntityMessage)message.type;
		switch (m) {
		case EntityMessage::State_Changed:
		{
			EntityState s = (EntityState)message.integer;
			switch (s) {
			case EntityState::Idle:
				changeAnimation(message.receiver, "Idle", true, true);
				break;
			case EntityState::Walking:
				changeAnimation(message.receiver, "Walk", true, true);
				break;
			case EntityState::Attacking:
				changeAnimation(message.receiver, "Attack", true, false);
				break;
			case EntityState::Dying:
				changeAnimation(message.receiver, "Death", true, false);
				break;
			}
		}break;
		}
	}
}

void S_SheetAnimation::handleEvent(const EntityId& entity, const EntityEvent& event) {

}

void S_SheetAnimation::changeAnimation(const EntityId& entity, const std::string& anim, bool play, bool loop) {
	C_SpriteSheet* sheet = systemManager->getEntityManager()->getComponent<C_SpriteSheet>(entity, Component::SpriteSheet);
	sheet->getSpriteSheet()->setAnimation(anim, play, loop);
}