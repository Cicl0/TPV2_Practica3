#include "NewGameState.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

NewGameState::NewGameState(Game* game) :
	_game(game) {

}

NewGameState::~NewGameState() {
}

void
NewGameState::update() {
	auto& ihdlr = ih();

	if (ihdlr.keyDownEvent()) {
		_game->setState(State::RUNNING);
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

	Message m;
	m.id = _m_NEW_GAME;
}

void
NewGameState::leave() {
	
}