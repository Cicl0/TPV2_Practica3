// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <vector>

#include "../ecs/ecs.h"
#include "GameState.h"

class Game {
public:
	Game();
	virtual ~Game();
	void initGame();
	void start();

	enum State {
		RUNNING, PAUSED, NEWGAME, NEWROUND, GAMEOVER
	};

	inline void setState(State s) {
		_state->leave();
		switch (s) {
		case RUNNING:
			_stateEnum = RUNNING;
			_state = _runing_state;
			break;
		case PAUSED:
			_stateEnum = PAUSED;
			_state = _paused_state;
			break;
		case NEWGAME:
			_stateEnum = NEWGAME;
			_state = _newgame_state;
			break;
		case NEWROUND:
			_stateEnum = NEWROUND;
			_state = _newround_state;
			break;
		case GAMEOVER:
			_stateEnum = GAMEOVER;
			_state = _gameover_state;
			break;
		default:
			break;
		}
		_state->enter();
	}

	inline State getState() {
		return _stateEnum;
	}

	inline ecs::EntityManager* getMngr() {
		return _mngr;
	}

	inline ecs::System* getPacManSys() {
		return _pacmanSys;
	}

	inline ecs::System* getRenderSys() {
		return _renderSys;
	}

	inline ecs::System* getCollisionSys() {
		return _collisionSys;
	}

	inline ecs::System* getFoodSys() {
		return _foodSys;
	}

	inline ecs::System* getGhostSys() {
		return _ghostSys;
	}

	inline ecs::System* getImmunitySys() {
		return _immunitySys;
	}

	inline ecs::System* getGameCtrlSys() {
		return _gameCtrlSys;
	}
private:
	ecs::EntityManager *_mngr;
	ecs::System *_pacmanSys;
	ecs::System *_renderSys;
	ecs::System *_collisionSys;
	ecs::System *_foodSys;
	ecs::System *_ghostSys;
	ecs::System *_immunitySys;
	ecs::System* _gameCtrlSys;

	State _stateEnum;
	GameState* _state;
	GameState* _paused_state;
	GameState* _runing_state;
	GameState* _newgame_state;
	GameState* _newround_state;
	GameState* _gameover_state;
};



