#pragma once
#include <sstream>

class SpriteSheet;

using Frame = unsigned int;

class Anim_Base {
	friend class SpriteSheet;

protected:
	virtual void frameStep() = 0;
	virtual void cropSprite() = 0;
	virtual void readIn(std::stringstream& stream) = 0;

	Frame frameCurrent;
	Frame frameStart;
	Frame frameEnd;
	Frame frameRow;
	int frameActionStart;
	int frameActionEnd;
	float frameTime;
	float elapsedTime;
	bool loop;
	bool playing;
	bool hasMoved;
	std::string name;
	SpriteSheet* spriteSheet;

public:
	Anim_Base();
	virtual ~Anim_Base();

	void play();
	void pause();
	void stop();
	void reset();
	
	virtual void update(const float& dT);

	friend std::stringstream& operator >> (std::stringstream& stream, Anim_Base& a) {
		a.readIn(stream);
		return stream;
	}

	void setSpriteSheet(SpriteSheet* sheet);
	bool setFrame(Frame frame);
	void setStartFrame(Frame frame);
	void setEndFrame(Frame frame);
	void setFrameRow(Frame row);
	void setActionStart(Frame frame);
	void setActionEnd(Frame frame);
	void setFrameTime(float time);
	void setLooping(bool loop);
	void setName(const std::string& name);

	SpriteSheet* getSpriteSheet();
	Frame getFrame();
	Frame getStartFrame();
	Frame getEndFrame();
	Frame getFrameRow();
	int getActionStart();
	int getActionEnd();
	float getFrameTime();
	float getElapsedTime();
	std::string getName();

	bool isLooping();
	bool isPlaying();
	bool isInAction();
	bool checkMoved();
};