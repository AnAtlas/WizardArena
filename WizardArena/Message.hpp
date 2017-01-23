#pragma once

using MessageType = unsigned int;
struct TwoFloats { float x; float y; };

struct Message {
	Message(const MessageType& type) : type(type) {}

	MessageType type;
	int sender;
	int receiver;

	union {
		TwoFloats twoF;
		bool boolean;
		int integer;
	};
};