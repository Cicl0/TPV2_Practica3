#pragma once

#include "../game/GameState.h"
#include "../game/Game.h"
#include "../ecs/EntityManager.h"

class NewGameState : public GameState
{
public:
	NewGameState(Game* game);
	~NewGameState();

	void update() override;
	void enter() override;
	void leave() override;
private:
	Game* _game;
};

