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

	//DEREK
	for (auto itr = state->second
}