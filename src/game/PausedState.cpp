#include "PausedState.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

PausedState::PausedState(Game* game) :
	_game(game) {

}

PausedState::~PausedState() {

}

void
PausedState::update() {
	auto& ihdlr = ih();

	while (!ihdlr.keyDownEvent()) {
		ihdlr.refresh();

		if (ihdlr.keyDownEvent()) {
			_game->setState(State::RUNNING);
			return;
		}

		sdlutils().clearRenderer();
		

		auto& t = sdlutils().msgs().at("pause");
		t.render(static_cast<float>((sdlutils().width() - t.width()) / 2), static_cast<float>((sdlutils().height() - t.height()) / 2));


		sdlutils().presentRenderer();
	}
}

void
PausedState::enter() {
}

void
PausedState::leave() {

}