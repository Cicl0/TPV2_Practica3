#pragma once

#include "../game/GameState.h"
#include "../game/Game.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

class PausedState : public GameState
{
public:
	PausedState(Game* game);
	~PausedState();

	void update() override;
	void enter() override;
	void leave() override;
private:
	ecs::EntityManager* _mngr;
	Game* _game;
};
