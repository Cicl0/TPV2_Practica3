#pragma once

#include "../game/Game.h"
#include "../game/GameState.h"
#include "../ecs/EntityManager.h"

class RunningState : public GameState
{
public:
	RunningState(Game* game);
	~RunningState();

	void update() override;
	void enter() override;
	void leave() override;
private:

	void handleCollision();

	ecs::EntityManager* _mngr;
	Game* _game;
	bool _needReset;
	bool _end;
	bool _pause;
};

