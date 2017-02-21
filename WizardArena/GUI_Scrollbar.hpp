#pragma once
#include "GUI_Element.hpp"

enum class SliderType { Horizontal, Vertical };

class GUI_Scrollbar : public GUI_Element
{
private:
	SliderType sliderType;
	sf::RectangleShape slider;
	sf::Vector2f moveMouseLast;
	int percentage;

public:
	GUI_Scrollbar(const std::string& name, GUI_Interface* owner);
	~GUI_Scrollbar();

	void readIn(std::stringstream& stream);
	void onClick(const sf::Vector2f& mousePos);
	void onRelease();
	void onHover(const sf::Vector2f& mousePos);
	void onLeave();

	void setPosition(const sf::Vector2f& pos);
	void applyStyle();
	void updateStyle(const GUI_ElementState& state, const GUI_Style& style);

	void update(float dT);
	void draw(sf::RenderTarget* target);
};