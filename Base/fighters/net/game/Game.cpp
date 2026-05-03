// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "Bullets.h"
#include "Fighter.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Collisions.h"
#include "Networking.h"

Game::Game() :
		_bm(nullptr), //
		_fighters(nullptr), //
		_net(nullptr)
{
}

Game::~Game() {
	delete _fighters;
	delete _bm;
	delete _net;

	// close SDLNet
	SDLNetUtils::close_SDLNet();

	// release InputHandler if the instance was created correctly.
	if (InputHandler::HasInstance())
		InputHandler::Release();

	// release SLDUtil if the instance was created correctly.
	if (SDLUtils::HasInstance())
		SDLUtils::Release();


}

bool Game::init() {

	// initialize the SDL singleton
	if (!SDLUtils::Init("SDLNet Game", 900, 480,
			"resources/config/asteroids.multiplayer.resources.json")) {

		std::cerr << "Something went wrong while initializing SDLUtils"
				<< std::endl;
		return false;
	}

	// initialize the InputHandler singleton
	if (!InputHandler::Init()) {
		std::cerr << "Something went wrong while initializing SDLHandler"
				<< std::endl;
		return false;

	}

	// initialize SDLNet
	SDLNetUtils::init_SDLNet();

	return true;
}

bool Game::init_game(const char *host, Uint16 port) {

	_net = new Networking();

	// establish connection to server first
	if (!_net->init(host, port))
		return false;

	_bm = new Bullets();
	_fighters = new Fighter(*_bm);

	// add some players
	_fighters->addPlayer(_net->get_client_id());

	return true;

}

void Game::start() {

	// a boolean to exit the loop
	bool exit = false;

	auto &ihdlr = ih();

	auto &vt = sdlutils().virtualTimer();
	vt.resetTime();

	while (!exit) {
		Uint32 startTime = vt.regCurrTime();

		// refresh the input handler
		ihdlr.refresh();
		if (ihdlr.keyDownEvent()) {

			// ESC exists the game
			if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
				exit = true;
				continue;
			}

			// ESC exists the game
			if (ihdlr.isKeyDown(SDL_SCANCODE_R)) {
				_net->send_restart();
			}

		}

		_fighters->update();
		_bm->update();
		_net->update();

		check_collisions();

		sdlutils().clearRenderer();

		_fighters->render();
		_bm->render();

		sdlutils().presentRenderer();

		Uint32 frameTime = vt.currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}

	_net->disconnect();
}

void Game::check_collisions() {
	if (!_net->is_master())
		return;

	for (Bullets::Bullet &b : *_bm) {
		if (b.used)
			for (Fighter::Player &p : *_fighters) {
				if (p.id != b.shooter && p.state == Fighter::ALIVE)
					if (Collisions::collidesWithRotation(p.pos, p.width,
							p.height, p.rot, b.pos, b.width, b.height, b.rot)) {
						b.used = false;
						_net->send_dead(p.id, b.shooter, b.timestamp);
						continue;
					}
			}
	}
}
