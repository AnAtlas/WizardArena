#include "EventManager.hpp"
#include "Utilities.hpp"

EventManager::EventManager()
	:currentState(StateType(0)), hasFocus(true)
{
	loadBindings();
}

EventManager::~EventManager(){
	for (auto &itr : bindings){
		delete itr.second;
	}
}

bool EventManager::addBinding(Binding *l_binding){
	if (bindings.find(l_binding->name) != bindings.end())
		return false;

	return bindings.emplace(l_binding->name, l_binding).second;
}

bool EventManager::removeBinding(std::string l_name){
	auto itr = bindings.find(l_name);
	if (itr == bindings.end()){ return false; }
	delete itr->second;
	bindings.erase(itr);
	return true;
}

void EventManager::setCurrentState(StateType l_state){
	currentState = l_state;
}

void EventManager::setFocus(const bool& l_focus){ hasFocus = l_focus; }

void EventManager::handleEvent(sf::Event& l_event){
	// Handling SFML events.
	for (auto &b_itr : bindings){
		Binding* bind = b_itr.second;
		for (auto &e_itr : bind->events){
			EventType sfmlEvent = (EventType)l_event.type;
			if (e_itr.first != sfmlEvent){ continue; }
			if (sfmlEvent == EventType::KeyDown || sfmlEvent == EventType::KeyUp){
				if (e_itr.second.code == l_event.key.code){
					// Matching event/keystroke.
					// Increase count.
					if (bind->details.keyCode != -1){
						bind->details.keyCode = e_itr.second.code;
					}
					++(bind->c);
					break;
				}
			} else if (sfmlEvent == EventType::MButtonDown || sfmlEvent == EventType::MButtonUp){
				if (e_itr.second.code == l_event.mouseButton.button){
					// Matching event/keystroke.
					// Increase count.
					bind->details.mouse.x = l_event.mouseButton.x;
					bind->details.mouse.y = l_event.mouseButton.y;
					if (bind->details.keyCode != -1){
						bind->details.keyCode = e_itr.second.code;
					}
					++(bind->c);
					break;
				}
			} else {
				// No need for additional checking.
				if (sfmlEvent == EventType::MouseWheel){
					bind->details.mouseWheelDelta = l_event.mouseWheel.delta;
				} else if (sfmlEvent == EventType::WindowResized){
					bind->details.size.x = l_event.size.width;
					bind->details.size.y = l_event.size.height;
				} else if (sfmlEvent == EventType::TextEntered){
					bind->details.textEntered = l_event.text.unicode;
				}
				++(bind->c);
			}
		}
	}
}

void EventManager::update(){
	if (!hasFocus){ return; }
	for (auto &b_itr : bindings){
		Binding* bind = b_itr.second;
		for (auto &e_itr : bind->events){
			switch (e_itr.first){
			case(EventType::Keyboard) :
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(e_itr.second.code))){
					if (bind->details.keyCode != -1){
						bind->details.keyCode = e_itr.second.code;
					}
					++(bind->c);
				}
			break;
			case(EventType::Mouse) :
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button(e_itr.second.code))){
					if (bind->details.keyCode != -1){
						bind->details.keyCode = e_itr.second.code;
					}
					++(bind->c);
				}
			break;
			case(EventType::Joystick) :
				// Up for expansion.
				break;
			}
		}

		if (bind->events.size() == bind->c){
			auto stateCallbacks = callbacks.find(currentState);
			auto otherCallbacks = callbacks.find(StateType(0));

			if (stateCallbacks != callbacks.end()){
				auto callItr = stateCallbacks->second.find(bind->name);
				if (callItr != stateCallbacks->second.end()){
					// Pass in information about events.
					callItr->second(&bind->details);
				}
			}

			if (otherCallbacks != callbacks.end()){
				auto callItr = otherCallbacks->second.find(bind->name);
				if (callItr != otherCallbacks->second.end()){
					// Pass in information about events.
					callItr->second(&bind->details);
				}
			}
		}
		bind->c = 0;
		bind->details.clear();
	}
}

void EventManager::loadBindings(){
	std::string delimiter = ":";

	std::ifstream bindings;
	bindings.open(Utils::GetWorkingDirectory() + "keys.cfg");
	if (!bindings.is_open()){ std::cout << "! Failed loading keys.cfg." << std::endl; return; }
	std::string line;
	while (std::getline(bindings, line)){
		std::stringstream keystream(line);
		std::string callbackName;
		keystream >> callbackName;
		Binding* bind = new Binding(callbackName);
		while (!keystream.eof()){
			std::string keyval;
			keystream >> keyval;
			int start = 0;
			int end = keyval.find(delimiter);
			if (end == std::string::npos){ delete bind; bind = nullptr; break; }
			EventType type = EventType(stoi(keyval.substr(start, end - start)));
			int code = stoi(keyval.substr(end + delimiter.length(),
				keyval.find(delimiter, end + delimiter.length())));

			EventInfo eventInfo;
			eventInfo.code = code;
			bind->bindEvent(type, eventInfo);
		}

		if (!addBinding(bind)){ delete bind; }
		bind = nullptr;
	}
	bindings.close();
}