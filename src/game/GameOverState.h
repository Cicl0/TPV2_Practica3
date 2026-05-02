#pragma once

#include "../game/GameState.h"
#include "../game/Game.h"

class GameOverState : public GameState
{
public:
	GameOverState(Game* game);
	~GameOverState();

	void update() override;
	void enter() override;
	void leave() override;
private:
	Game* _game;
	bool _win;
};

