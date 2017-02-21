#pragma once
#include "GUI_Element.hpp"

class GUI_Textfield : public GUI_Element
{
public:
	GUI_Textfield(const std::string& name, GUI_Interface* owner);
	~GUI_Textfield();

	void readIn(std::stringstream& stream);
	void onClick(const sf::Vector2f& mousePos);
	void onRelease();
	void onHover(const sf::Vector2f& mousePos);
	void onLeave();

	void update(float dT);
	void draw(sf::RenderTarget* target);
};