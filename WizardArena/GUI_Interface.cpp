#include "GUI_Interface.hpp"
#include "GUI_Manager.hpp"
#include "SharedContext.hpp"
#include "GUI_Event.hpp"

GUI_Interface::GUI_Interface(const std::string& name, GUI_Manager* guiMgr)
	: GUI_Element(name, GUI_ElementType::Window, this), parent(nullptr), guiManager(guiMgr),
	movable(false), beingMoved(false), showTitleBar(false), focused(false),
	scrollHorizontal(0), scrollVertical(0), contentRedraw(true), controlRedraw(true)
{
	backdropTexture = new sf::RenderTexture();
	contentTexture = new sf::RenderTexture();
	controlTexture = new sf::RenderTexture();
}

GUI_Interface::~GUI_Interface() {
	delete backdropTexture;
	delete contentTexture;
	delete controlTexture;
	for (auto &itr : elements) {
		delete itr.second;
	}
}

void GUI_Interface::setPosition(const sf::Vector2f& pos) {
	GUI_Element::setPosition(pos);
	backdrop.setPosition(pos);
	content.setPosition(pos);
	control.setPosition(pos);
	titleBar.setPosition(position.x, position.y - titleBar.getSize().y);
	visual.text.setPosition(titleBar.getPosition() + style[state].textPadding);
}

bool GUI_Interface::addElement(const GUI_ElementType& type, const std::string& name) {
	if (elements.find(name) != elements.end())
		return false;
	GUI_Element* element = nullptr;
	element = guiManager->createElement(type, this);
	if (!element)
		return false;
	element->setName(name);
	element->setOwner(this);
	elements.emplace(name, element);
	contentRedraw = true;
	controlRedraw = true;
	return true;
}

GUI_Element* GUI_Interface::getElement(const std::string& name) {
	auto itr = elements.find(name);
	return (itr != elements.end() ? itr->second : nullptr);
}

bool GUI_Interface::removeElement(const std::string& name) {
	auto itr = elements.find(name);
	if (itr == elements.end())
		return false;
	delete itr->second;
	elements.erase(itr);
	contentRedraw = true;
	controlRedraw = true;
	adjustContentSize();
	return true;
}

bool GUI_Interface::isInside(const sf::Vector2f& point) {
	if (GUI_Element::isInside(point))
		return true;
	return titleBar.getGlobalBounds().contains(point);
}

void GUI_Interface::readIn(std::stringstream& stream) {
	std::string movableState;
	std::string titleShow;
	std::string title;
	stream >> elementPadding.x >> elementPadding.y >> movableState >> titleShow;
	Utils::ReadQuotedString(stream, title);
	visual.text.setString(title);
	if (movableState == "Movable")
		movable = true;
	if (titleShow == "Title")
		showTitleBar = true;
}

void GUI_Interface::onClick(const sf::Vector2f& mousePos) {
	defocusTextfields();
	if (titleBar.getGlobalBounds().contains(mousePos) && movable && showTitleBar)
		beingMoved = true;
	else {
		GUI_Event event;
		event.type = GUI_EventType::Click;
		event.mInterface = name.c_str();
		event.element = "";
		event.clickCoords.x = mousePos.x;
		event.clickCoords.y = mousePos.y;
		guiManager->addEvent(event);
		for (auto &itr : elements) {
			if (!itr.second->isInside(mousePos))
				continue;
			itr.second->onClick(mousePos);
			event.element = itr.second->name.c_str();
			guiManager->addEvent(event);
		}
		setState(GUI_ElementState::Clicked);
	}
}

void GUI_Interface::onRelease() {
	GUI_Event event;
	event.type = GUI_EventType::Release;
	event.mInterface = name.c_str();
	event.element = "";
	guiManager->addEvent(event);
	for (auto &itr : elements) {
		if (itr.second->getState() != GUI_ElementState::Clicked)
			continue;
		itr.second->onRelease();
		event.element = itr.second->name.c_str();
		guiManager->addEvent(event);
	}
	setState(GUI_ElementState::Neutral);
}

void GUI_Interface::onTextEntered(const char& ch) {
	for (auto &itr : elements) {
		if (itr.second->getType() != GUI_ElementType::Textfield)
			continue;
		if (itr.second->getState() != GUI_ElementState::Clicked)
			continue;
		if (ch == 8) {
			//Backspace
			const auto& text = itr.second->getText();
			itr.second->setText(text.substr(0, text.length() - 1));
			return;
		}
		if (ch < 32 || ch > 126)
			return;
		std::string text = itr.second->getText();
		text.push_back(ch);
		itr.second->setText(text);
		return;
	}
}

void GUI_Interface::defocusTextfields() {
	GUI_Event event;
	event.type = GUI_EventType::Release;
	event.mInterface = name.c_str();
	event.element = "";
	for (auto &itr : elements) {
		if (itr.second->getType() != GUI_ElementType::Textfield)
			continue;
		itr.second->setState(GUI_ElementState::Neutral);
		event.element = itr.second->name.c_str();
		guiManager->addEvent(event);
	}
}

void GUI_Interface::onHover(const sf::Vector2f& mousePos) {
	GUI_Event event;
	event.type = GUI_EventType::Hover;
	event.mInterface = name.c_str();
	event.element = "";
	event.clickCoords.x = mousePos.x;
	event.clickCoords.y = mousePos.y;
	guiManager->addEvent(event);
	setState(GUI_ElementState::Focused);
}

void GUI_Interface::onLeave() {
	GUI_Event event;
	event.type = GUI_EventType::Leave;
	event.mInterface = name.c_str();
	event.element = "";
	guiManager->addEvent(event);
	setState(GUI_ElementState::Neutral);
}

void GUI_Interface::update(float dT) {
	sf::Vector2f mousePos = sf::Vector2f(guiManager->getContext()->eventManager->getMousePos(guiManager->getContext()->window->getRenderWindow()));

	if (beingMoved && moveMouseLast != mousePos) {
		sf::Vector2f difference = mousePos - moveMouseLast;
		moveMouseLast = mousePos;
		sf::Vector2f newPosition = position + difference;
		setPosition(newPosition);
	}

	for (auto &itr : elements) {
		if (itr.second->needsRedraw()) {
			if (itr.second->isControl()) {
				controlRedraw = true;
			}
			else {
				contentRedraw = true;
			}
		}
		if (!itr.second->isActive())
			continue;
		itr.second->update(dT);
		if (beingMoved)
			continue;
		GUI_Event event;
		event.mInterface = name.c_str();
		event.element = itr.second->name.c_str();
		event.clickCoords.x = mousePos.x;
		event.clickCoords.y = mousePos.y;
		if (isInside(mousePos) && itr.second->isInside(mousePos) && !titleBar.getGlobalBounds().contains(mousePos)) {
			if (itr.second->getState() != GUI_ElementState::Neutral)
				continue;
			itr.second->onHover(mousePos);
			event.type = GUI_EventType::Hover;
		}
		else if (itr.second->getState() == GUI_ElementState::Focused) {
			itr.second->onLeave();
			event.type = GUI_EventType::Leave;
		}
		guiManager->addEvent(event);
	}
}

void GUI_Interface::draw(sf::RenderTarget* target) {
	target->draw(backdrop);
	target->draw(content);
	target->draw(control);

	if (!showTitleBar)
		return;
	target->draw(titleBar);
	target->draw(visual.text);
}