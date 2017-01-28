#pragma once
#include <unordered_map>
#include "EntityMessages.hpp"
#include "Communicator.hpp"

using Subscriptions = std::unordered_map<EntityMessage, Communicator>;

class MessageHandler {
private:
	Subscriptions communicators;

public:
	bool subscribe(const EntityMessage& type, Observer* observer) {
		return communicators[type].addObserver(observer);
	}

	bool unsubscribe(const EntityMessage& type, Observer* observer) {
		return communicators[type].removeObserver(observer);
	}

	void dispatch(const Message& msg) {
		auto itr = communicators.find((EntityMessage)msg.type);
		if (itr == communicators.end())
			return;
		itr->second.broadcast(msg);
	}
};