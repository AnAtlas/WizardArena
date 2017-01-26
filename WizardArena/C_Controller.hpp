#pragma once
#include "C_Base.hpp"

class C_Controller : public C_Base {
public:
	C_Controller() : C_Base(Component::Controller) {}

	void readIn(std::stringstream& stream) {}
};