#pragma once
#include "GUI_Element.hpp"

class GUI_Label : public GUI_Element
{
public:
	GUI_Label(const std::string& name, GUI_Interface* owner);
	~GUI_Label();

	void readIn(std::stringstream& stream);
	void onClick(const sf::Vector2f& mousePos);
	void onRelease();
	void onHover(const sf::Vector2f& mousePos);
	void onLeave();

	void update(float dT);
	void draw(sf::RenderTarget* target)
};