#include "EventManager.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

EventManager::EventManager() : hasFocus(true) {
	loadBindings();
}

EventManager::~EventManager() {
	for (auto itr : bindings) {
		bindings.clear();
	}
}

void EventManager::loadBindings() {
	std::string delimiter = ":";

	std::ifstream bindings;
	bindings.open("keys.cfg");
	if (!bindings.is_open()) {
		std::cout << "! Failed loading keys.cfg" << std::endl;
		return;
	}
	std::string line;
	while (std::getline(bindings, line)) {
		std::stringstream keystream(line);
		std::string callbackName;
		keystream >> callbackName;
		std::shared_ptr<Binding> bind(new Binding(callbackName));
		while (!keystream.eof()) {
			std::string keyval;
			keystream >> keyval;
			int start = 0;
			int end = keyval.find(delimiter);
			if (end == std::string::npos) {
				bind = nullptr;
				break;
			}
			EventType type = EventType(stoi(keyval.substr(start, end - start)));
			int code = stoi(keyval.substr(end + delimiter.length(), keyval.find(delimiter, end + delimiter.length())));
			EventInfo eventInfo;
			eventInfo.code = code;
			bind->bindEvent(type, eventInfo);
		}

		addBinding(bind);
		bind = nullptr;
	}
	bindings.close();
}

bool EventManager::addBinding(std::shared_ptr<Binding> binding) {
	if (bindings.find(binding->name) != bindings.end())
		return false;

	return bindings.emplace(binding->name, binding).second;
}

bool EventManager::RemoveBinding(std::string name) {
	auto itr = bindings.find(name);
	if (itr == bindings.end())
		return false;
	bindings.erase(itr);
	return true;
}

void EventManager::handleEvent(sf::Event& event) {
	//Handling SFML Events
	for (auto itr : bindings) {
		std::shared_ptr<Binding> bind = itr.second;
		for (auto itr2 : bind->events) {
			EventType sfmlEvent = (EventType)event.type;
			if (itr2.first != sfmlEvent)
				continue;
			if (sfmlEvent == EventType::KeyDown || sfmlEvent == EventType::KeyUp) {
				if (itr2.second.code == event.key.code) {
					//Matching event/keystroke
					//Increase count
					if (bind->details.keyCode != -1)
						bind->details.keyCode = itr2.second.code;
					++(bind->c);
					break;
				}
			}
			else if (sfmlEvent == EventType::MouseButtonDown || sfmlEvent == EventType::MouseButtonUp) {
				if (itr2.second.code == event.mouseButton.button) {
					//Matching event/keystroke
					//Increase count
					bind->details.mouse.x = event.mouseButton.x;
					bind->details.mouse.y = event.mouseButton.y;
					if (bind->details.keyCode != -1)
						bind->details.keyCode = itr2.second.code;
					++(bind->c);
					break;
				}
			}
			else {
				//No need for additional checking
				if (sfmlEvent == EventType::MouseWheel) {
					bind->details.mouseWheelDelta = event.mouseWheel.delta;
				}
				else if (sfmlEvent == EventType::WindowResized) {
					bind->details.size.x = event.size.width;
					bind->details.size.y = event.size.height;
				}
				else if (sfmlEvent == EventType::TextEntered)
					bind->details.textEntered = event.text.unicode;
				++(bind->c);
			}
		}
	}
}
void EventManager::update() {
	if (!hasFocus)
		return;
	for (auto itr : bindings) {
		std::shared_ptr<Binding> bind = itr.second;
		for (auto itr2 : bind->events) {
			switch (itr2.first) {
			case (EventType::Keyboard) :
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(itr2.second.code))) {
					if (bind->details.keyCode != -1) {
						bind->details.keyCode = itr2.second.code;
					}
					++(bind->c);
				}
				break;
			case (EventType::Mouse) : 
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button(itr2.second.code))) {
					if (bind->details.keyCode != -1) {
						bind->details.keyCode = itr2.second.code;
					}
					++(bind->c);
				}
				break;
			case(EventType::Joystick):

				break;
			}
		}
		if (bind->events.size() == bind->c) {
			auto stateCallbacks = callbacks.find(currentState);
			auto otherCallbacks = callbacks.find(StateType(0));

			if (stateCallbacks != callbacks.end()) {
				auto callItr = stateCallbacks->second.find(bind->name);
				if (callItr != stateCallbacks->second.end()) {
					//Pass in information about events
					callItr->second(std::shared_ptr<EventDetails>(&bind->details));
				}
			}
			if (otherCallbacks != callbacks.end()) {
				auto callItr = otherCallbacks->second.find(bind->name);
				if (callItr != otherCallbacks->second.end()) {
					//Pass in information about events
					callItr->second(std::shared_ptr<EventDetails>(&bind->details));
				}
			}
		}
		bind->c = 0;
		bind->details.clear();
	}
}