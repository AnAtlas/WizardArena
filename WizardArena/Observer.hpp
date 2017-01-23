#pragma once
#include "Message.hpp"

class Observer {
public:
	virtual ~Observer() {}
	virtual void notify(const Message& message) = 0;
};