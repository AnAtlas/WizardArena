#pragma once
#include <unordered_map>

enum class GUI_EventType{ None, Click, Release, Hover, Leave };

struct ClickCoordinates {
	float x; 
	float y;
};

struct GUI_Event {
	GUI_EventType type;
	const char* element;
	const char* mInterface;
	union {
		ClickCoordinates clickCoords;
	};
};