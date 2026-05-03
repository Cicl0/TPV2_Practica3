#pragma once

#include "../game/GameState.h"
#include "../game/Game.h"
#include "../ecs/EntityManager.h"

class NewRoundState : public GameState
{
public:
	NewRoundState(Game* game);
	~NewRoundState();

	void update() override;
	void enter() override;
	void leave() override;
private:
	Game* _game;
};

