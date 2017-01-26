#pragma once
#include "C_Base.hpp"

enum class EntityState { Idle, Walking, Attacking, Hurt, Dying };

class C_State : public C_Base {
private:
	EntityState state;

public:
	C_State() : C_Base(Component::State) {}

	void readIn(std::stringstream& stream) {
		unsigned int st = 0;
		stream >> st;
		state = (EntityState)st;
	}

	EntityState getState() {
		return state;
	}

	void setState(const EntityState& stat) {
		state = stat;
	}
};