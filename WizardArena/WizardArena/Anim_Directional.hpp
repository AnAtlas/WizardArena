#pragma once
#include "Anim_Base.hpp"

class Anim_Directional : public Anim_Base {
protected:
	void frameStep();
	void cropSprite();
	void readIn(std::stringstream& stream);
};