#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <functional>
#include <fstream>
#include <sstream>
#include <iostream>

enum class EventType{
	KeyDown = sf::Event::KeyPressed,
	KeyUp = sf::Event::KeyReleased,
	MButtonDown = sf::Event::MouseButtonPressed,
	MButtonUp = sf::Event::MouseButtonReleased,
	MouseWheel = sf::Event::MouseWheelMoved,
	WindowResized = sf::Event::Resized,
	GainedFocus = sf::Event::GainedFocus,
	LostFocus = sf::Event::LostFocus,
	MouseEntered = sf::Event::MouseEntered,
	MouseLeft = sf::Event::MouseLeft,
	Closed = sf::Event::Closed,
	TextEntered = sf::Event::TextEntered,
	Keyboard = sf::Event::Count + 1, Mouse, Joystick,
	GUI_Click, GUI_Release, GUI_Hover, GUI_Leave
};

struct EventInfo{
	EventInfo(){ code = 0; }
	EventInfo(int l_event){ code = l_event; }
	EventInfo(GUI_Event guiEvent) { gui = guiEvent; }
	union{
		int code;
		GUI_Event gui;
	};
};

struct EventDetails{
	EventDetails(const std::string& l_bindName)
		: name(l_bindName){
		clear();
	}
	std::string name;

	sf::Vector2i size;
	sf::Uint32 textEntered;
	sf::Vector2i mouse;
	int mouseWheelDelta;
	int keyCode; // Single key code.

	std::string guiInterface;
	std::string guiElement;
	GUI_EventType guiEvent;

	void clear(){
		size = sf::Vector2i(0, 0);
		textEntered = 0;
		mouse = sf::Vector2i(0, 0);
		mouseWheelDelta = 0;
		keyCode = -1;
		guiInterface = "";
		guiElement = "";
		guiEvent = GUI_EventType::None;
	}
};

using Events = std::vector<std::pair<EventType, EventInfo>>;

struct Binding{
	Binding(const std::string& l_name) : name(l_name), details(l_name), c(0){}
	~Binding(){
		//GUI Portion
		for (auto itr = events.begin(); itr != events.end(); ++itr) {
			if (itr->first == EventType::GUI_Click || itr->first == EventType::GUI_Release ||
				itr->first == EventType::GUI_Hover || itr->first == EventType::GUI_Leave) {
				delete[] itr->second.gui.mInterface;
				delete[] itr->second.gui.element;
			}
		}
	}
	void bindEvent(EventType l_type, EventInfo l_info = EventInfo()){
		events.emplace_back(l_type, l_info);
	}

	Events events;
	std::string name;
	int c; // Count of events that are "happening".

	EventDetails details;
};

using Bindings = std::unordered_map<std::string, Binding*>;
// Callback container.
using CallbackContainer = std::unordered_map<std::string, std::function<void(EventDetails*)>>;
// State callback container.
enum class StateType;
using Callbacks = std::unordered_map<StateType, CallbackContainer>;

class EventManager{
public:
	EventManager();
	~EventManager();

	bool addBinding(Binding *l_binding);
	bool removeBinding(std::string l_name);

	void setCurrentState(StateType l_state);
	void setFocus(const bool& l_focus);

	// Needs to be defined in the header!
	template<class T>
	bool addCallback(StateType l_state, const std::string& l_name,
		void(T::*l_func)(EventDetails*), T* l_instance)
	{
		auto itr = callbacks.emplace(l_state, CallbackContainer()).first;
		auto temp = std::bind(l_func, l_instance, std::placeholders::_1);
		return itr->second.emplace(l_name, temp).second;
	}

	bool removeCallback(StateType l_state, const std::string& l_name){
		auto itr = callbacks.find(l_state);
		if (itr == callbacks.end()){ return false; }
		auto itr2 = itr->second.find(l_name);
		if (itr2 == itr->second.end()){ return false; }
		itr->second.erase(l_name);
		return true;
	}

	void handleEvent(sf::Event& l_event);
	void update();

	// Getters.
	sf::Vector2i getMousePos(sf::RenderWindow* l_wind = nullptr){
		return (l_wind ? sf::Mouse::getPosition(*l_wind) : sf::Mouse::getPosition());
	}
private:
	void loadBindings();

	StateType currentState;
	Bindings bindings;
	Callbacks callbacks;

	bool hasFocus;
};