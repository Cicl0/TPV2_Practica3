// This file is part of the course TPV2@UCM - Samir Genaim

#include "CollisionsSystem.h"

#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../utils/Collisions.h"

CollisionsSystem::CollisionsSystem() {
	// TODO Auto-generated constructor stub

}

CollisionsSystem::~CollisionsSystem() {
	// TODO Auto-generated destructor stub
}

void CollisionsSystem::initSystem() {
}

void CollisionsSystem::update() {

	// the PacMan's Transform
	//
	auto pm = _mngr->getHandler(ecs::hdlr::PACMAN);
	auto pTR = _mngr->getComponent<Transform>(pm);

	// For safety, we traverse with a normal loop until the current size. In this
	// particular case we could use a for-each loop since the list fruits is not
	// modified.
	//
	auto &fruits = _mngr->getEntities(ecs::grp::FRUITS);
	auto n = fruits.size();
	for (auto i = 0u; i < n; i++) {
		auto e = fruits[i];
		if (_mngr->isAlive(e)) { // if the fruit is active (it might have died in this frame)

			// the Fruit's Transform
			//
			auto eTR = _mngr->getComponent<Transform>(e);

			// check if PacMan collides with the Fruit (i.e., eat it)
			if (Collisions::collides(			//
					pTR->getPos(), pTR->getWidth(), pTR->getHeight(), //
					eTR->getPos(), eTR->getWidth(), eTR->getHeight())) {

				Message m;
				m.id = _m_PACMAN_FOOD_COLLISION;
				m.collision_data.pacman = pm;
				m.collision_data.other = e;
				_mngr->send(m, true);

			}
		}
	}

	// Check collisions with Ghosts
	//
	auto &ghosts = _mngr->getEntities(ecs::grp::GHOSTS);
	for (auto ghost : ghosts) {
		if (_mngr->isAlive(ghost)) {

			// the Ghost's Transform
			//
			auto gTR = _mngr->getComponent<Transform>(ghost);

			// check if PacMan collides with the Ghost
			if (Collisions::collides(			//
					pTR->getPos(), pTR->getWidth(), pTR->getHeight(), //
					gTR->getPos(), gTR->getWidth(), gTR->getHeight())) {

				Message m;
				m.id = _m_PACMAN_GHOST_COLLISION;
				m.collision_data.pacman = pm;
				m.collision_data.other = ghost;
				_mngr->send(m, true);

			}
		}
	}

}

