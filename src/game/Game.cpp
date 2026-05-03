// This file is part of the course TPV2@UCM - Samir Genaim

#include "Game.h"

#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"
#include "LittleWolf.h"
#include "Networking.h"

Game::Game() :
	_little_wolf(nullptr),
	_networking(nullptr),
	_state(NEWGAME),
	_network_mode(false)
{
}

Game::~Game() {
	if (_networking) {
		_networking->disconnect();
		delete _networking;
	}

	// release InputHandler if the instance was created correctly.
	if (InputHandler::HasInstance())
		InputHandler::Release();

	// release SLDUtil if the instance was created correctly.
	if (SDLUtils::HasInstance())
		SDLUtils::Release();

	delete _little_wolf;
}
// Inicialización para modo red
bool Game::init_game(const char *host, unsigned short port) {
	_network_mode = true;
	_networking = new Networking();
	if (!_networking->init(host, port)) {
		std::cerr << "Error inicializando red" << std::endl;
		return false;
	}

	_little_wolf = new LittleWolf();
	// Aquí podrías cargar el mapa y jugadores según el estado de la red
	// Por ahora, carga el mapa base
	_little_wolf->load("resources/maps/little_wolf/map_0.json");

	if (!SDLUtils::Init("[Little Wolf - Network]",
			_little_wolf->get_xres(),
			_little_wolf->get_yres(),
			"resources/config/littlewolf.resources.json")) {
		std::cerr << "Error inicializando SDLUtils" << std::endl;
		return false;
	}

	if (!InputHandler::Init()) {
		std::cerr << "Error inicializando InputHandler" << std::endl;
		return false;
	}

	_little_wolf->init(sdlutils().window(), sdlutils().renderer());
	// Los jugadores se sincronizarán por red
	return true;
}

void Game::start() {

	// a boolean to exit the loop
	bool exit = false;

	auto &ihdlr = ih();

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		// refresh the input handler
		ihdlr.refresh();

		if (ihdlr.keyDownEvent()) {

			// ESC exists the game
			if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
				exit = true;
				continue;
			}

		}

		_little_wolf->update();

		// the clear is not necessary since the texture we copy to the window occupies the whole screen
		// sdlutils().clearRenderer();

		_little_wolf->render();

		sdlutils().presentRenderer();

		Uint32 frameTime = sdlutils().currRealTime() - startTime;

		if (frameTime < 10)
			SDL_Delay(10 - frameTime);
	}

}

void
Game::setState(State state) {
	_state = state;
}

