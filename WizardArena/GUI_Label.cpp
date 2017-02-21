#include "GUI_Label.hpp"
#include "Utilities.hpp"

GUI_Label::GUI_Label(const std::string& name, GUI_Interface* ownder)
	: GUI_Element(name, GUI_ElementType::Label, owner)
{

}

GUI_Label::~GUI_Label() {

}

void GUI_Label::readIn(std::stringstream& stream) {
	std::string content;
	Utils::ReadQuotedString(stream, content);
	visual.text.setString(content);
}

void GUI_Label::onClick(const sf::Vector2f& mousePos) {
	setState(GUI_ElementState::Clicked);
}

void GUI_Label::onRelease() {
	setState(GUI_ElementState::Neutral);
}

void GUI_Label::onHover(const sf::Vector2f& mousePos) {
	setState(GUI_ElementState::Focused);
}

void GUI_Label::onLeave() {
	setState(GUI_ElementState::Neutral);
}

void GUI_Label::update(float dT) {

}

void GUI_Label::draw(sf::RenderTarget* target) {
	target->draw(visual.backgroundSolid);
	if (style[state].glyph != "")
		target->draw(visual.glyph);
	target->draw(visual.text);
}
