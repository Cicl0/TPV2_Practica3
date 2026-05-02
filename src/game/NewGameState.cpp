#include "NewGameState.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

NewGameState::NewGameState(Game* game) :
	_mngr(nullptr), _game(game) {

}

NewGameState::~NewGameState() {
	delete _mngr;
}

void
NewGameState::update() {
	auto& ihdlr = ih();

	if (ihdlr.keyDownEvent()) {
		_game->setState(Game::RUNNING);
		return;
	}

	sdlutils().clearRenderer();

	auto& t = sdlutils().msgs().at("start");
	t.render(static_cast<float>((sdlutils().width() - t.width()) / 2),
		static_cast<float>((sdlutils().height() - t.height()) / 2));

	sdlutils().presentRenderer();
}

void
NewGameState::enter() {
	_mngr = _game->getMngr();

	assert(_mngr != nullptr);

	Message m;
	m.id = _m_NEW_GAME;
	_mngr->send(m);
}

void
NewGameState::leave() {
	
}