#include "Anim_Base.hpp"
#include "SpriteSheet.hpp"

Anim_Base::Anim_Base() : frameCurrent(0), frameStart(0), frameEnd(0), frameRow(0), frameTime(0.f), elapsedTime(0),
	frameActionStart(-1), frameActionEnd(-1), loop(false), playing(false), hasMoved(false)
{

}

Anim_Base::~Anim_Base() {

}

void Anim_Base::setSpriteSheet(SpriteSheet* sheet) {
	spriteSheet = sheet;
}

bool Anim_Base::setFrame(Frame frame) {
	if ((frame >= frameStart && frame <= frameEnd) ||
		(frame >= frameEnd && frame <= frameStart)) {
		frameCurrent = frame;
		hasMoved = true;
		return true;
	}
	return false;
}

bool Anim_Base::isInAction() {
	if (frameActionStart == -1 || frameActionEnd == -1)
		return true;

	return (frameCurrent >= frameActionStart && frameCurrent <= frameActionEnd);
}

void Anim_Base::play() { playing = true; }

void Anim_Base::pause() { playing = false; }

void Anim_Base::stop() { playing = false; reset(); }

void Anim_Base::reset() {
	frameCurrent = frameStart;
	elapsedTime = 0.0f;
	cropSprite();
}

void Anim_Base::update(const float& dT) {
	if (!playing)
		return;
	elapsedTime += dT;
	if (elapsedTime < frameTime)
		return;
	frameStep();
	cropSprite();
	elapsedTime = 0;
}

void Anim_Base::setStartFrame(Frame frame) { frameStart = frame; }
void Anim_Base::setEndFrame(Frame frame) { frameEnd = frame; }
void Anim_Base::setFrameRow(Frame row) { frameRow = row; }
void Anim_Base::setActionStart(Frame frame) { frameActionStart = frame; }
void Anim_Base::setActionEnd(Frame frame) { frameActionEnd = frame; }
void Anim_Base::setFrameTime(float time) { frameTime = time; }
void Anim_Base::setLooping(bool loop) { this->loop = loop; }
void Anim_Base::setName(const std::string& name) { this->name = name; }

SpriteSheet* Anim_Base::getSpriteSheet() { return spriteSheet; }
Frame Anim_Base::getFrame() { return frameCurrent; }
Frame Anim_Base::getStartFrame() { return frameStart; }
Frame Anim_Base::getEndFrame() { return frameEnd; }
Frame Anim_Base::getFrameRow() { return frameRow; }
int Anim_Base::getActionStart() { return frameActionStart; }
int Anim_Base::getActionEnd() { return frameActionEnd; }
float Anim_Base::getFrameTime() { return frameTime; }
float Anim_Base::getElapsedTime() { return elapsedTime; }
std::string Anim_Base::getName() { return name; }
bool Anim_Base::isLooping() { return loop; }
bool Anim_Base::isPlaying() { return playing; }
bool Anim_Base::checkMoved() { 
	bool result = hasMoved;
	hasMoved = false;
	return result;
}