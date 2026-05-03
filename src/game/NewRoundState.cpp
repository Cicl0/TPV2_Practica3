#include "NewRoundState.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

NewRoundState::NewRoundState(Game* game) :
	_game(game) {

}

NewRoundState::~NewRoundState() {

}

void
NewRoundState::update() {
	auto& ihdlr = ih();

	if (ihdlr.keyDownEvent() && ihdlr.isKeyDown(SDL_SCANCODE_RETURN)) {
		_game->setState(State::RUNNING);
		return;
	}

	sdlutils().clearRenderer();

	auto& t = sdlutils().msgs().at("start_round");
	t.render(static_cast<float>((sdlutils().width() - t.width()) / 2),
		static_cast<float>((sdlutils().height() - t.height()) / 2));

	sdlutils().presentRenderer();
}

void
NewRoundState::enter() {
}

void
NewRoundState::leave() {
	
}