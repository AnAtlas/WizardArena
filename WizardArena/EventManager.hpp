          #pragma once
#include <functional>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include <memory>

enum class EventType {
	KeyDown = sf::Event::KeyPressed,
	KeyUp = sf::Event::KeyReleased,
	MouseButtonDown = sf::Event::MouseButtonPressed,
	MouseButtonUp = sf::Event::MouseButtonReleased,
	MouseWheel = sf::Event::MouseWheelMoved,
	WindowResized = sf::Event::Resized,
	TextEntered = sf::Event::TextEntered,
	Keyboard = sf::Event::Count + 1, Mouse, Joystick
};

struct EventInfo {
	EventInfo() { code = 0; }
	EventInfo(int event) { code = event; }
	union {
		int code;
	};
};

struct EventDetails {
	EventDetails(const std::string bindName) : name(bindName) {
		clear();
	}
	std::string name;
	sf::Vector2i size;
	sf::Uint32 textEntered;
	sf::Vector2i mouse;
	int mouseWheelDelta;
	int keyCode;

	void clear() {
		size = sf::Vector2i(0, 0);
		textEntered = 0;
		mouse = sf::Vector2i(0, 0);
		mouseWheelDelta = 0;
		keyCode = -1;
	}
};

using Events = std::vector<std::pair<EventType, EventInfo>>;

struct Binding {
	Binding(const std::string name) : name(name), details(name), c(0) {}
	void bindEvent(EventType type, EventInfo info = EventInfo()) {
		events.emplace_back(type, info);
	}

	Events events;
	std::string name;
	int c; //Count of events that are "happening"

	EventDetails details;
};

using Bindings = std::unordered_map<std::string, std::shared_ptr<Binding>>;

using CallbackContainer = std::unordered_map<std::string, std::function<void(std::shared_ptr<EventDetails>)>>;
enum class StateType;
using Callbacks = std::unordered_map<StateType, CallbackContainer>;







class EventManager {
private:
	Bindings bindings;
	Callbacks callbacks;
	bool hasFocus;
	StateType currentState;

	void loadBindings();
public:

	EventManager();
	~EventManager();

	bool addBinding(std::shared_ptr<Binding> binding);
	bool RemoveBinding(std::string name);
	void setFocus(const bool& focus);

	template<class T>
	bool addCallback(StateType state, const std::string name, void (T::* func)(EventDetails*), T* instance) {
		auto itr = callbacks.emplace(state, CallbackContainer()).first;
		auto temp = std::bind(func, instance, std::placeholders::_1);
		return itr->second.emplace(name, temp).second;
	}

	bool removeCallback(StateType state, const std::string name) {
		auto itr = callbacks.find(state);
		if (itr == callbacks.end())
			return false;
		auto itr2 = itr->second.find(name);
		if (itr2 == itr->second.end())
			return false;
		itr->second.erase(name);
		return true;
	}

	void handleEvent(sf::Event& event);

	void update();

	sf::Vector2i getMousePosition(std::shared_ptr<sf::RenderWindow> window = nullptr) {
		return (window ? sf::Mouse::getPosition(*window) : sf::Mouse::getPosition());
	}
};