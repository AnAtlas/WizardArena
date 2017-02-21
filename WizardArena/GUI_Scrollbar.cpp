#include "GUI_Scrollbar.hpp"
#include "GUI_Interface.hpp"
#include "GUI_Manager.hpp"
#include "SharedContext.hpp"

GUI_Scrollbar::GUI_Scrollbar(const std::string& name, GUI_Interface* owner)
	:GUI_Element(name, GUI_ElementType::Scrollbar, owner)
{
	control = true;
}

void GUI_Scrollbar::readIn(std::stringstream& stream) {
	std::string type;
	stream >> type;
	if (type == "Horizontal")
		sliderType = SliderType::Horizontal;
	else
		sliderType = SliderType::Vertical;

	if (sliderType == SliderType::Horizontal)
		slider.setPosition(0, getPosition().y);
	else
		slider.setPosition(getPosition().x, 0);
}

void GUI_Scrollbar::onClick(const sf::Vector2f& mousePos) {
	if (!slider.getGlobalBounds().contains(mousePos - owner->getPosition()))
		return;
	setState(GUI_ElementState::Clicked);
	moveMouseLast = mousePos;
}

void GUI_Scrollbar::onRelease() {
	setState(GUI_ElementState::Neutral);
}

void GUI_Scrollbar::onHover(const sf::Vector2f& mousePos) {
	setState(GUI_ElementState::Focused);
}

void GUI_Scrollbar::onLeave() {
	setState(GUI_ElementState::Neutral);
}

void GUI_Scrollbar::updateStyle(const GUI_ElementState& state, const GUI_Style& style) {
	GUI_Element::updateStyle(state, style);
	if (sliderType == SliderType::Horizontal)
		this->style[state].size.x = owner->getSize().x;
	else
		this->style[state].size.y = owner->getSize().y;
}

void GUI_Scrollbar::applyStyle() {
	GUI_Element::applyStyle();
	slider.setFillColor(style[state].elementColor);
	bool horizontal = sliderType == SliderType::Horizontal;
	auto &bgSolid = visual.backgroundSolid;
	setPosition((horizontal ? sf::Vector2f(0, owner->getSize().y - bgSolid.getSize().y) : sf::Vector2f(owner->getSize().x - bgSolid.getSize().x, 0)));
	bgSolid.setSize((horizontal ? sf::Vector2f(owner->getSize().x, style[state].size.y) : sf::Vector2f(style[state].size.x, owner->getSize().y)));
	slider.setPosition((horizontal ? slider.getPosition().x : getPosition().x), (horizontal ? getPosition().y : slider.getPosition().y));
	float sizeFactor = (horizontal ? owner->getContentSize().x / owner->getSize().x : owner->getContentSize().y / owner->getSize().y);
	if (sizeFactor < 1.0f)
		sizeFactor = 1.0f;
	float sliderSize = (horizontal ? owner->getSize().x : owner->getSize().y) / sizeFactor;
	slider.setSize((horizontal ? sf::Vector2f(sliderSize, bgSolid.getSize().y) : sf::Vector2f(bgSolid.getSize().x, sliderSize)));
	bgSolid.setPosition(getPosition());
}

void GUI_Scrollbar::update(float dT) {
	//Mouse drag code
	if (getState() != GUI_ElementState::Clicked)
		return;
	SharedContext* context = owner->getManager()->getContext();
	sf::Vector2f mousePos = sf::Vector2f(context->eventManager->getMousePos(context->window->getRenderWindow()));
	if (moveMouseLast == mousePos)
		return;
	sf::Vector2f dif = mousePos - moveMouseLast;
	moveMouseLast = mousePos;

	bool horiz = sliderType == SliderType::Horizontal;
	slider.move((horiz ? dif.x : 0), (horiz ? 0 : dif.y));
	if (horiz && slider.getPosition().x < 0)
		slider.setPosition(0, slider.getPosition().y);
	else if (slider.getPosition().y < 0)
		slider.setPosition(slider.getPosition().x, 0);

	if (horiz && slider.getPosition().x + slider.getSize().x > owner->getSize().x)
		slider.setPosition(owner->getSize().x - slider.getSize().x, slider.getPosition().y);
	else if (slider.getPosition().y + slider.getSize().y > owner->getSize().y)
		slider.setPosition(slider.getPosition().x, owner->getSize().y - slider.getSize().y);

	float workArea = (horiz ? owner->getSize().x - slider.getSize().x : owner->getSize().y - slider.getSize().y);
	int percentage = ((horiz ? slider.getPosition().x : slider.getPosition().y) / workArea) * 100;
	if (horiz)
		owner->updateScrollHorizontal(percentage);
	else
		owner->updateScrollVertical(percentage);
	setRedraw(true);
}

void GUI_Scrollbar::draw(sf::RenderTarget* target) {
	target->draw(visual.backgroundSolid);
	target->draw(slider);
}

void GUI_Scrollbar::setPosition(const sf::Vector2f& pos) {
	GUI_Element::setPosition(pos);
	if (sliderType == SliderType::Horizontal)
		position.x = 0;
	else
		position.y = 0;
}


