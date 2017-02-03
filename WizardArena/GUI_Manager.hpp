#pragma once
#include <unordered_map>
#include <functional>
#include <vector>

#include "EventManager.hpp"

#include "GUI_Interface.hpp"
#include "GUI_Event.hpp"
#include "GUI_Label.hpp"
#include "GUI_Scrollbar.hpp"
#include "GUI_Textfield.hpp"

using GUI_Interfaces = std::unordered_map<std::string, GUI_Interface*>;
using GUI_Container = std::unordered_map<StateType, GUI_Interfaces>;
using GUI_Events = std::unordered_map<StateType, std::vector<GUI_Event>>;
using GUI_Factory = std::unordered_map<GUI_ElementType, std::function<GUI_Element*(GUI_Interface*)>>;
using GUI_ElemTypes = std::unordered_map<std::string, GUI_ElementType>;

struct SharedContext;

class GUI_Manager {
	friend class GUI_Interface;

private:
	GUI_Container interfaces;
	GUI_Events events;
	SharedContext* context;
	EventManager* eventManager;
	StateType currentState;
	GUI_Factory factory;
	GUI_ElemTypes elemTypes;

	GUI_Element* createElement(const GUI_ElementType& id, GUI_Interface* owner);
	GUI_ElementType stringToType(const std::string& string);
	bool loadStyle(const std::string& file, GUI_Element* element);

public:
	GUI_Manager(EventManager* evMgr, SharedContext* context);
	~GUI_Manager();

	bool addInterface(const StateType& state, const std::string& name);
	GUI_Interface* getInterface(const StateType& state, const std::string& name);
	bool removeInterface(const StateType& state, const std::string& name);
	
	bool loadInterface(const StateType& state, const std::string& intface, const std::string& name);

	void setCurrentState(const StateType& state);

	SharedContext* getContext();

	void defocusAllInterfaces();

	void handleClick(EventDetails* details);
	void handleRelease(EventDetails* details);
	void handleTextEntered(EventDetails* details);

	void addEvent(GUI_Event event);
	void pollEvent(GUI_Event& event);

	void update(float dT);
	void render(sf::RenderWindow* window);

	template<class T>
	void registerElement(const GUI_ElementType& id) {
		factory[id] = [](GUI_Interface* owner) -> GUI_Element* { return new T("", owner); }
	}
};