// This file is part of the course TPV2@UCM - Samir Genaim

#include "RenderSystem.h"

#include "../components/PacManComponent.h"
#include "../components/FruitComponent.h"
#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/macros.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/Texture.h"
#include "GameCtrlSystem.h"

RenderSystem::RenderSystem() {
	_immunityActive = false;

}

RenderSystem::~RenderSystem() {
}

void RenderSystem::initSystem() {
}

void RenderSystem::update() {
	drawFood();
	drawPacMan();
	drawHealth();
	drawGhosts();
}

void RenderSystem::recieve(const Message &msg) {
	switch (msg.id) {
	case _m_NEW_GAME:
	case _m_ROUND_OVER:
	case _m_GAME_OVER:
	case _m_IMMUNITY_END:
		_immunityActive = false;
		break;
	case _m_IMMUNITY_START:
		_immunityActive = true;
		break;
	default:
		break;
	}
}

void RenderSystem::drawPacMan() {
	auto e = _mngr->getHandler(ecs::hdlr::PACMAN);
	auto tr = _mngr->getComponent<Transform>(e);
	const Texture &sheet = sdlutils().images().at("pac_sheet");

	const Uint64 frameDurationMs = 80;
	const int numFrames = 4;
	const int animFrame = static_cast<int>((sdlutils().currRealTime() / frameDurationMs) % static_cast<Uint64>(numFrames));
	SDL_FRect src { animFrame * CELL_W, 0.0f, CELL_W, CELL_H };
	SDL_FRect dest = build_sdlfrect(tr->getPos(), tr->getWidth(), tr->getHeight());
	sheet.render(src, dest, tr->getRot());

}

void RenderSystem::drawHealth() {
	auto e = _mngr->getHandler(ecs::hdlr::PACMAN);
	PacManComponent* pacComp = _mngr->getComponent<PacManComponent>(e);
	int lifes = pacComp->getLives();

	const Texture& heart = sdlutils().images().at("heart");
	
	float heartSize = 4;
	SDL_FRect dest = build_sdlfrect(5, 5, heart.width() / heartSize, heart.height() / heartSize);
	for (int i = 0; i < lifes; i++) {
		heart.render(dest);
		dest.x += heart.width() / heartSize;
	}
}

void RenderSystem::drawFood() {
	const Texture &sheet = sdlutils().images().at("pac_sheet");

	for (auto e : _mngr->getEntities(ecs::grp::FRUITS)) {
		if (_mngr->isAlive(e)) {
			auto tr = _mngr->getComponent<Transform>(e);
			auto fruit = _mngr->getComponent<FruitComponent>(e);

			// Cereza columna 4 
			int col = 4;
			if (fruit != nullptr && fruit->isMagic() && fruit->isCurrentlyMagic()){
				//Pera columna 7
				col = 7;
			}

			int row = 1;

			SDL_FRect src { col * CELL_W, row * CELL_H, CELL_W, CELL_H };
			SDL_FRect dest = build_sdlfrect(tr->getPos(), tr->getWidth(), tr->getHeight());
			sheet.render(src, dest);
		}
	}
}

void RenderSystem::drawGhosts() {
	auto &ghosts = _mngr->getEntities(ecs::grp::GHOSTS);
	const Texture &sheet = sdlutils().images().at("pac_sheet");

	const Uint64 frameDurationMs = 100;
	const int numFrames = 8;
	const int animFrame = static_cast<int>((sdlutils().currRealTime() / frameDurationMs) % static_cast<Uint64>(numFrames));

	for (auto ghost : ghosts) {
		if (!_mngr->isAlive(ghost))
			continue;

		auto tr = _mngr->getComponent<Transform>(ghost);
		if (!tr)
			continue;

		int ghostRow = _immunityActive ? 6 : 4;
		SDL_FRect src { animFrame * CELL_W, ghostRow * CELL_H, CELL_W, CELL_H };
		SDL_FRect dest = build_sdlfrect(tr->getPos(), tr->getWidth(), tr->getHeight());
		sheet.render(src, dest, tr->getRot());
	}
}

void RenderSystem::draw(Transform *tr, const Texture *tex) {
	SDL_FRect dest = build_sdlfrect(tr->getPos(), tr->getWidth(), tr->getHeight());

	assert(tex != nullptr);
	tex->render(dest, tr->getRot());
}
