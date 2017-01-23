#pragma once
#include <iostream>
#include <sstream>
#include "ECS_Types.hpp"

class C_Base {
protected:
	Component type;

public:
	C_Base(const Component& type) : type(type) {}
	virtual ~C_Base() {}

	Component getType() { return type; }

	friend std::stringstream& operator >> (std::stringstream& stream, C_Base& b)
	{
		b.readIn(stream);
		return stream;
	}
	virtual void readIn(std::stringstream& stream) = 0;
};