// This file is part of the course TPV2@UCM - Samir Genaim

#include "GameCtrlSystem.h"

#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../game/Game.h"
GameCtrlSystem::GameCtrlSystem() 
		 {
	// TODO Auto-generated constructor stub

}

GameCtrlSystem::~GameCtrlSystem() {
	// TODO Auto-generated destructor stub
}

void GameCtrlSystem::initSystem() {
}

void GameCtrlSystem::update() {
	(void)_mngr;
}

void GameCtrlSystem::recieve(const Message& msg) {
	switch (msg.id) {

	case _m_GAME_OVER:
		_game->setState(State::GAMEOVER);
		break;

	case _m_ROUND_OVER:
		_game->setState(State::NEWROUND);
		break;

	case _m_GAME_PAUSED:
		_game->setState(State::PAUSED);
		break;

	default:
		break;
	}
}
