#include "GUI_Manager.hpp"
#include "SharedContext.hpp"

GUI_Manager::GUI_Manager(EventManager* evMgr, SharedContext* shared)
	: eventManager(evMgr), context(shared), currentState(StateType(0))
{
	registerElement<GUI_Label>(GUI_ElementType::Label);
	registerElement<GUI_Scrollbar>(GUI_ElementType::Scrollbar);
	registerElement<GUI_Textfield>(GUI_ElementType::Textfield);
	
	elemTypes.emplace("Label", GUI_ElementType::Label);
	elemTypes.emplace("Button", GUI_ElementType::Button);
	elemTypes.emplace("Scrollbar", GUI_ElementType::Scrollbar);
	elemTypes.emplace("Textfield", GUI_ElementType::Textfield);
	elemTypes.emplace("Interface", GUI_ElementType::Window);

	eventManager->addCallback(StateType(0), "Mouse_Left", &GUI_Manager::handleClick, this);
	eventManager->addCallback(StateType(0), "Mouse_Left_Release", &GUI_Manager::handleRelease, this);
	eventManager->addCallback(StateType(0), "Text_Entered", &GUI_Manager::handleTextEntered, this);
}

GUI_Manager::~GUI_Manager() {
	eventManager->removeCallback(StateType(0), "Mouse_Left");
	eventManager->removeCallback(StateType(0), "Mouse_Left_Release");
	eventManager->removeCallback(StateType(0), "Text_Entered");

	for (auto &itr : interfaces) {
		for (auto & itr2 : itr.second) {
			delete itr2.second;
		}
	}
}

bool GUI_Manager::addInterface(const StateType& state, const std::string& name) {
	auto s = interfaces.emplace(state, GUI_Interfaces()).first;
	GUI_Interface* temp = new GUI_Interface(name, this);
	if (s->second.emplace(name, temp).second)
		return true;
	delete temp;
	return false;
}

GUI_Interface* GUI_Manager::getInterface(const StateType& state, const std::string& name) {
	auto s = interfaces.find(state);
	if (s == interfaces.end())
		return nullptr;
	auto i = s->second.find(name);
	return (i != s->second.end() ? i->second : nullptr);
}

bool GUI_Manager::removeInterface(const StateType& state, const std::string& name) {
	auto s = interfaces.find(state);
	if (s == interfaces.end())
		return false;
	auto i = s->second.find(name);
	if (i == s->second.end())
		return false;
	delete i->second;
	return s->second.erase(name));
}

void GUI_Manager::addEvent(GUI_Event event) {
	events[currentState].push_back(event);
}

GUI_Element* GUI_Manager::createElement(const GUI_ElementType& id, GUI_Interface* owner) {
	if (id == GUI_ElementType::Window)
		return new GUI_Interface("", this);
	auto f = factory.find(id);
	return (f != factory.end() ? f->second(owner) : nullptr);
}

bool GUI_Manager::loadInterface(const StateType& state, const std::string& intface, const std::string& name) {
	std::ifstream file;
	file.open(Utils::GetWorkingDirectory() + "media/GUI_Interfaces/" + intface);
	std::string interfaceName;

	if (!file.is_open()) {
		std::cerr << "! Failed to load: " << intface << std::endl;
		return false;
	}

	std::string line;
	while (std::getline(file, line)) {
		if (line[0] == '|')
			continue;
		std::stringstream keystream(line);
		std::string key;
		if (key == "Interface") {
			std::string style;
			keystream >> interfaceName >> style;
			if (!addInterface(state, name)) {
				std::cerr << "Failed adding interface: " << name << std::endl;
				return false;
			}
			GUI_Interface* i = getInterface(state, name);
			keystream >> *i;
			if (!loadStyle(style, i))
				std::cerr << "Failed loading style file: " << style << " for interface " << name << std::endl;
			i->setContentSize(i->getSize());
		}
		else if (key == "Element") {
			if (interfaceName == "") {
				std::cerr << "Error: 'Element' outside or before declaration of interface" << std::endl;
				continue;
			}
			std::string type;
			std::string name;
			sf::Vector2f position;
			std::string style;
			keystream >> type >> name >> position.x >> position.y >> style;
			GUI_ElementType eType = stringToType(type);
			if (eType == GUI_ElementType::None) {
				std::cerr << "Unknown element('" << name << "') type: '" << type << "'" << std::endl;
				continue;
			}

			GUI_Interface* i = getInterface(state, name);
			if (!i)
				continue;
			if (!i->addElement(eType, name))
				continue;
			GUI_Element* e = i->getElement(name);
			keystream >> *e;
			e->setPosition(position);
			if (!loadStyle(style, e)) {
				std::cerr << "Failed loading style file: " << style << " for element " << name << std::endl;
				continue;
			}
		}
	}
	file.close();
	return true;
}

bool GUI_Manager::loadStyle(const std::string& f, GUI_Element* element) {
	std::ifstream file;
	file.open(Utils::GetWorkingDirectory() + "media/GUI_Styles/" + f);

	std::string currentState;
	GUI_Style parentStyle;
	GUI_Style tempStyle;
	if (!file.is_open()) {
		std::cerr << "! Failed to load: " << f << std::endl;
		return false;
	}

	std::string line;
	while (std::getline(file, line)) {
		if (line[0] == '|')
			continue;
		std::stringstream keystream(line);
		std::string type;
		keystream >> type;
		if (type == "")
			continue;
		if (type == "State") {
			if (currentState != "") {
				std::cerr << "Error: 'State' Keyword found inside another state" << std::endl;
				continue;
			}
			keystream >> currentState;
		}
		else if (type == "/State") {
			if (currentState == "") {
				std::cerr << "Error: '/State' keyword found prior to 'State'" << std::endl;
				continue;
			}
			GUI_ElementState state = GUI_ElementState::Neutral;
			if (currentState == "Hover")
				state = GUI_ElementState::Focused;
			else if (currentState == "Clicked")
				state = GUI_ElementState::Clicked;

			if (state == GUI_ElementState::Neutral) {
				parentStyle = tempStyle;
				element->updateStyle(GUI_ElementState::Neutral, tempStyle);
				element->updateStyle(GUI_ElementState::Focused, tempStyle);
				element->updateStyle(GUI_ElementState::Clicked, tempStyle);
			}
			else {
				element->updateStyle(state, tempStyle);
			}
			tempStyle = parentStyle;
			currentState = "";
		}
		else {
			//Handling Style information
			if (currentState == "") {
				std::cerr << "Error: '" << type << "' keyword found outside of a state" << std::endl;
				continue;
			}
			if (type == "Size") {
				keystream >> tempStyle.size.x >> tempStyle.size.y;
			}
			else if (type == "BgColor") {
				int r, g, b, a = 0;
				keystream >> r >> g >> b >> a;
				tempStyle.backgroundColor = sf::Color(r, g, b, a);
			}
			else if (type == "BgImage") {
				keystream >> tempStyle.backgroundImage;
			}
			else if (type == "BgImageColor") {
				int r, g, b, a = 0;
				keystream >> r >> g >> b >> a;
				tempStyle.backgroundImageColor = sf::Color(r, g, b, a);
			}
			else if (type == "TextColor") {
				int r, g, b, a = 0;
				keystream >> r >> g >> b >> a;
				tempStyle.textColor = sf::Color(r, g, b, a);
			}
			else if (type == "TextSize") {
				keystream >> tempStyle.textSize;
			}
			else if (type == "TextOriginCenter") {
				tempStyle.textCenterOrigin = true;
			}
			else if (type == "Font") {
				keystream >> tempStyle.textFont;
			}
			else if (type == "TextPadding") {
				keystream >> tempStyle.textPadding.x >> tempStyle.textPadding.y;
			}
			else if (type == "ElementColor") {
				int r, g, b, a = 0;
				keystream >> r >> g >> b >> a;
				tempStyle.elementColor = sf::Color(r, g, b, a);
			}
			else if (type == "Glyph") {
				keystream >> tempStyle.glyph;
			}
			else if (type == "GlyphPadding") {
				keystream >> tempStyle.glyphPadding.x >> tempStyle.glyphPadding.y;
			}
			else {
				std::cerr << "Error: style tage '" << type << "' is unknown" << std::endl;
			}
		}
	}
	file.close();
	return true;
}

bool GUI_Manager::pollEvent(GUI_Event& event) {
	if (events[currentState].empty())
		return false;
	event = events[currentState].back();
	events[currentState].pop_back();
	return true;
}

void GUI_Manager::setCurrentState(const StateType& state) {
	if (currentState == state)
		return;
	handleRelease(nullptr);
	currentState = state;
}

void GUI_Manager::handleClick(EventDetails* details) {
	auto state = interfaces.find(currentState);
	if (state == interfaces.end())
		return;
	sf::Vector2i mousePos = eventManager->getMousePos(context->window->getRenderWindow());

	for (auto itr = state->second.begin(); itr != state->second.end(); ++itr) {
		if (!itr->second->isInside(sf::Vector2f(mousePos)))
			continue;
		if (!itr->second->isActive())
			return;
		itr->second->onClick(sf::Vector2f(mousePos));
		itr->second->focus();
		if (itr->second->isBeingMoved())
			itr->second->beginMoving();
		return;
	}
}

void GUI_Manager::handleRelease(EventDetails* details) {
	auto state = interfaces.find(currentState);
	if (state == interfaces.end())
		return;
	for (auto &itr : state->second) {
		GUI_Interface* i = itr.second;
		if (!i->isActive())
			continue;
		if (i->getState() == GUI_ElementState::Clicked)
			i->onRelease();
		if (i->isBeingMoved())
			i->stopMoving();
	}
}

void GUI_Manager::handleTextEntered(EventDetails* details) {
	auto state = interfaces.find(currentState);
	if (state == interfaces.end())
		return;
	for (auto &itr : state->second) {
		if (!itr.second->isActive())
			continue;
		if (!itr.second->isFocused())
			continue;
		itr.second->onTextEntered(details->textEntered);
		return;
	}
}

void GUI_Manager::update(float dT) {
	sf::Vector2i mousePos = eventManager->getMousePos(context->window->getRenderWindow());

	auto state = interfaces.find(currentState);
	if (state == interfaces.end())
		return;
	for (auto itr = state->second.begin(); itr != state->second.end(); ++itr) {
		GUI_Interface* i = itr->second;
		if (!i->isActive())
			continue;
		i->update(dT);
		if (i->isBeingMoved())
			continue;
		if (i->isInside(sf::Vector2f(mousePos))) {
			if (i->getState() == GUI_ElementState::Neutral)
				i->onHover(sf::Vector2f(mousePos));
			return;
		}
		else if (i->getState() == GUI_ElementState::Focused)
			i->onLeave();
	}
}

void GUI_Manager::render(sf::RenderWindow* window) {
	auto state = interfaces.find(currentState);
	if (state == interfaces.end())
		return;
	for (auto &itr : state->second) {
		GUI_Interface* i = itr.second;
		if (!i->isActive())
			continue;
		if (i->needsRedraw())
			i->redraw();
		if (i->needsContentRedraw())
			i->redrawContent();
		if (i->needsControlRedraw())
			i->redrawControls();
		i->draw(window);
	}
}