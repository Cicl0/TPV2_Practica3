// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../components/PacManComponent.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../systems/CollisionsSystem.h"
#include "../systems/GameCtrlSystem.h"
#include "../systems/PacManSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/FoodSystem.h"
#include "../systems/GhostSystem.h"
#include "../systems/ImmunitySystem.h"
#include "../utils/Vector2D.h"
#include "../utils/Collisions.h"
#include "../game/ecs_defs.h"

#include "PausedState.h"
#include "RunningState.h"
#include "NewGameState.h"
#include "NewRoundState.h"
#include "GameOverState.h"

using ecs::EntityManager;

Game::Game() :
		_mngr(), //
		_pacmanSys(), //
		_renderSys(), //
		_collisionSys(), //
		_foodSys(), //
		_ghostSys(), //
		_immunitySys(),
		_state(nullptr),
		_paused_state(nullptr),
		_runing_state(nullptr),
		_newgame_state(nullptr),
		_newround_state(nullptr),
		_gameover_state(nullptr) {

}

Game::~Game() {
	delete _mngr;

	// release InputHandler if the instance was created correctly.
	if (InputHandler::HasInstance())
		InputHandler::Release();

	// release SLDUtil if the instance was created correctly.
	if (SDLUtils::HasInstance())
		SDLUtils::Release();
}

void Game::initGame() {

	// initialize the SDL singleton
	if (!SDLUtils::Init("PacMan, Stars, ...", 800, 600,
			"resources/config/asteroid.resources.json")) {

		std::cerr << "Something went wrong while initializing SDLUtils"
				<< std::endl;
		return;
	}

	// initialize the InputHandler singleton
	if (!InputHandler::Init()) {
		std::cerr << "Something went wrong while initializing SDLHandler"
				<< std::endl;
		return;

	}

	// Create the manager
	_mngr = new EntityManager();

	// add the systems
	_pacmanSys = _mngr->addSystem<PacManSystem>();
	_renderSys = _mngr->addSystem<RenderSystem>();
	_collisionSys = _mngr->addSystem<CollisionsSystem>();
	_foodSys = _mngr->addSystem<FoodSystem>();
	_ghostSys =	_mngr->addSystem<GhostSystem>();
	_immunitySys = _mngr->addSystem<ImmunitySystem>();
	_gameCtrlSys = _mngr->addSystem<GameCtrlSystem>();
	static_cast<GameCtrlSystem*>(_gameCtrlSys)->setGame(this);
	// create a visible PacMan entity so movement/render can be tested immediately
	auto pacman = _mngr->addEntity();
	_mngr->addComponent<Transform>(
			pacman,
			Vector2D(sdlutils().width() / 2.0f - 24.0f, sdlutils().height() / 2.0f - 24.0f),
			Vector2D(0.0f, 0.0f),
			48.0f,
			48.0f,
			0.0f);
	_mngr->addComponent<PacManComponent>(pacman, 3);
	_mngr->setHandler(ecs::hdlr::PACMAN, pacman);

	static_cast<PacManSystem*>(_pacmanSys)->setPacMan(pacman);

	// Creacion de estados
	_paused_state = new PausedState(this);
	_runing_state = new RunningState(this);
	_newgame_state = new NewGameState(this);
	_newround_state = new NewRoundState(this);
	_gameover_state = new GameOverState(this);

	_state = _newgame_state;
	_state->enter();
}

void Game::start() {

	// a boolean to exit the loop
	bool exit = false;

	auto &ihdlr = ih();

	while (!exit) {
		Uint64 startTime = sdlutils().currRealTime();

		// refresh the input handler
		ihdlr.refresh();

		if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
			exit = true;
			continue;
		}

		_state->update();

		Uint64 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(static_cast<Uint32>(10 - frameTime));
	}

}

