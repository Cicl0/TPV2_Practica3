#include "RunningState.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include "../utils/Collisions.h"

RunningState::RunningState(Game* game) :
	_mngr(nullptr), _game(game), _needReset(false), _end(false), _pause(false) {

}

RunningState::~RunningState() {
    delete _mngr;
}

void
RunningState::update() {
	auto& ihdlr = ih();

	if (ihdlr.isKeyDown(SDL_SCANCODE_P)) {
		Message m;
		m.id = _m_GAME_PAUSED;
		_mngr->send(m, true);
		return;
	}

	_game->getPacManSys()->update();
	_game->getFoodSys()->update();
	_game->getGhostSys()->update();
	_game->getImmunitySys()->update();
	_game->getCollisionSys()->update();
	_game->getGameCtrlSys()->update();
	_mngr->flushMessages();

	_mngr->refresh();

	sdlutils().clearRenderer();
	_game->getRenderSys()->update();
	sdlutils().presentRenderer();
}

void 
RunningState::enter() {
	_mngr = _game->getMngr();

    assert(_mngr != nullptr);

	Message m;
	m.id = _m_ROUND_START;
	_mngr->send(m);
}

void 
RunningState::leave() {
	_needReset = false;
	_end = false;
	_pause = false;
}

void
RunningState::handleCollision() {

}