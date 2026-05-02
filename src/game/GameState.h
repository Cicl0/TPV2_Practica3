#pragma once

class GameState {
public:
	GameState() {}
	virtual ~GameState() {}
	virtual void update() = 0;
	virtual void enter() = 0;
	virtual void leave() = 0;
};

