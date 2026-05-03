// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once


class LittleWolf;

enum State {
	NEWGAME,
	NEWROUND, 
	GAMEOVER, 
	PAUSED, 
	RUNNING
};

class Game {
public:
	Game();
	virtual ~Game();
	void init(const char *map);
	void start();

	void setState(State state);
private:
	LittleWolf *_little_wolf;
	State _state;
};

