#include "GameOverState.h"

#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"

GameOverState::GameOverState(Game* game) :
	 _game(game), _win(false)
{

}

GameOverState::~GameOverState() {

}

void
GameOverState::update() {
	auto& ihdlr = ih();

	if (ihdlr.keyDownEvent() && ihdlr.isKeyDown(SDL_SCANCODE_RETURN)) {
		_game->setState(State::NEWGAME);
		return;
	}

	sdlutils().clearRenderer();

	if (_win) {
		auto& t = sdlutils().msgs().at("win");
		t.render(static_cast<float>((sdlutils().width() - t.width()) / 2), static_cast<float>((sdlutils().height() - t.height()) / 2));
	}
	else {
		auto& t = sdlutils().msgs().at("dead");
		t.render(static_cast<float>((sdlutils().width() - t.width()) / 2), static_cast<float>((sdlutils().height() - t.height()) / 2));
	}

	sdlutils().presentRenderer();
}

void
GameOverState::enter() {
	
}

void
GameOverState::leave() {
	_win = false;
}