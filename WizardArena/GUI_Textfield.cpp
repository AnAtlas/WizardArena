#include "GUI_Textfield.hpp"
#include "Utilities.hpp"

GUI_Textfield::GUI_Textfield(const std::string& name, GUI_Interface* owner)
	:GUI_Element(name, GUI_ElementType::Textfield, owner)
{

}

GUI_Textfield::~GUI_Textfield() {

}

void GUI_Textfield::readIn(std::stringstream& stream) {
	std::string content;
	Utils::ReadQuotedString(stream, content);
	visual.text.setString(content);
}

void GUI_Textfield::onClick(const sf::Vector2f& mousePos) {
	setState(GUI_ElementState::Clicked);
}

void GUI_Textfield::onRelease() {

}

void GUI_Textfield::onHover(const sf::Vector2f& mousePos) {
	setState(GUI_ElementState::Focused);
}

void GUI_Textfield::onLeave() {
	setState(GUI_ElementState::Neutral);
}

void GUI_Textfield::draw(sf::RenderTarget* target) {
	target->draw(visual.backgroundSolid);
	if (style[state].glyph != "")
		target->draw(visual.glyph);
	target->draw(visual.text);
}