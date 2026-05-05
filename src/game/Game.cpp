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
bool Game::init_game(const char* host, unsigned short port) {
	_network_mode = true;
	_networking = new Networking();
	if (!_networking->init(host, port)) {
		std::cerr << "Error inicializando red" << std::endl;
		return false;
	}

	_little_wolf = new LittleWolf();
	// conectar referencias entre Networking y LittleWolf
	_little_wolf->setNetworking(_networking);
	_networking->setLittleWolf(_little_wolf);

	// Cargar el mapa (esto NO usa SDLUtils)
	_little_wolf->load("resources/maps/little_wolf/map_0.json");

	// Inicializar SDLUtils e InputHandler antes de usar cualquier sdlutils().*
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

	// Inicializar LittleWolf con la ventana/renderer ya creados
	_little_wolf->init(sdlutils().window(), sdlutils().renderer());

	// Registrar jugador local
	Uint8 localId = _networking->get_client_id();
	if (!_little_wolf->addPlayer(localId)) {
    std::cout << "ERROR: couldn't add local player "
              << (int)localId << std::endl;
			  
	_little_wolf->setCurrentPlayer(localId);
}
else {
    std::cout << "Added local player "
              << (int)localId << std::endl;
}

	_state = RUNNING;

	return true;
}

void Game::start() {

	// a boolean to exit the loop
	bool exit = false;

	auto &ihdlr = ih();

	Uint32 restartStart = 0;
	int restartCountdown = 5;

	while (!exit) {
		Uint32 startTime = sdlutils().currRealTime();

		// refresh the input handler
		ihdlr.refresh();

		// --- BLOQUEO DE CONTROLES DURANTE REINICIO ---
		if (_state == WAITING_RESTART) {
			// Mostrar mensaje de cuenta atrás
			int y = sdlutils().height() / 2;
			std::string msg = "The game will restart in " + std::to_string(restartCountdown) + " seconds";
			Texture t(sdlutils().renderer(), msg, sdlutils().fonts().at("MFR12"), build_sdlcolor(0xFFFFFFFF));
			t.render((sdlutils().width() - t.width()) / 2, y);
			sdlutils().presentRenderer();

			if (restartStart == 0) restartStart = sdlutils().currRealTime();
			Uint32 elapsed = (sdlutils().currRealTime() - restartStart) / 1000;
			if (elapsed >= 5) {
				
				_state = RUNNING;
				restartStart = 0;
				restartCountdown = 5;
			} else {
				restartCountdown = 5 - elapsed;
			}
			SDL_Delay(50);
			continue;
		}

		if (ihdlr.keyDownEvent()) {
			if (ihdlr.isKeyDown(SDL_SCANCODE_ESCAPE)) {
				exit = true;
				continue;
			}
		}
			
		_networking->update();

		//Miramos si solo hay 1 jugador ALIVE
		int conectados = _little_wolf->countUsedPlayers();
		int vivos = _little_wolf->countAlivePlayers();
		if (conectados >= 2 && vivos < 2 && _state == RUNNING) {
			_state = WAITING_RESTART;
			restartStart = 0;
			restartCountdown = 5;
			continue;
		}

	
		_little_wolf->update();
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

