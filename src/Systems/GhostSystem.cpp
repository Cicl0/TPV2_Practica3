#include "GhostSystem.h"

#include "../components/PacManComponent.h"
#include "../ecs/EntityManager.h"

namespace {
constexpr float GHOST_SIZE = 48.0f;
} // namespace

GhostSystem::GhostSystem() :
		_lastUpdateTime(0.0f),
		_lastSpawnTime(0.0f),
		_roundActive(false),
		_immunityActive(false) {
}

GhostSystem::~GhostSystem() {
}

void GhostSystem::initSystem() {
	_lastUpdateTime = static_cast<float>(sdlutils().currRealTime());
	_lastSpawnTime = _lastUpdateTime;
}

void GhostSystem::update() {
	if (!_roundActive)
		return;

	float now = static_cast<float>(sdlutils().currRealTime());
	float dt = (now - _lastUpdateTime) / 1000.0f;
	if (dt <= 0.0f)
		dt = 0.0001f;

	trySpawnGhost();
	updateGhosts(dt);

	_lastUpdateTime = now;
}

void GhostSystem::recieve(const Message &msg) {
	switch (msg.id) {
	case _m_NEW_GAME:
		onNewGame();
		break;
	case _m_ROUND_START:
		onNewRound();
		break;
	case _m_ROUND_OVER:
        _roundActive = false;
        clearAllGhosts();
        break;
	case _m_GAME_OVER:
		_roundActive = false;
		clearAllGhosts();
		break;
	case _m_IMMUNITY_START:
		_immunityActive = true;
		break;
	case _m_IMMUNITY_END:
		_immunityActive = false;
		break;
	case _m_PACMAN_GHOST_COLLISION:
		if (_immunityActive) {
			removeGhost(msg.collision_data.other);
		} else {
			if (!_roundActive)
				break;

			auto pacman = _mngr->getHandler(ecs::hdlr::PACMAN);
			auto pmComp = _mngr->getComponent<PacManComponent>(pacman);

			if (pmComp != nullptr)
				pmComp->loseLife();

			_roundActive = false;
			clearAllGhosts();

			Message m;
			if (pmComp != nullptr && pmComp->getLives() <= 0)
				m.id = _m_GAME_OVER;
			else
				m.id = _m_ROUND_OVER;

			_mngr->send(m, true);
		}
		break;
	default:
		break;
	}
}

void GhostSystem::onNewGame() {
	clearAllGhosts();
	_roundActive = false;
	_immunityActive = false;
	_lastSpawnTime = static_cast<float>(sdlutils().currRealTime());
}

void GhostSystem::onNewRound() {
	clearAllGhosts();
	_roundActive = true;
	_lastSpawnTime = static_cast<float>(sdlutils().currRealTime());
}

void GhostSystem::trySpawnGhost() {
	if (!_roundActive || _immunityActive)
		return;

	if (countAliveGhosts() >= MAX_GHOSTS)
		return;

	float now = static_cast<float>(sdlutils().currRealTime());
	if (now - _lastSpawnTime < SPAWN_COOLDOWN * 1000.0f)
		return;

	spawnGhost();
	_lastSpawnTime = now;
}

void GhostSystem::spawnGhost() {
	auto ghost = _mngr->addEntity(ecs::grp::GHOSTS);
	Vector2D pos = randomCornerPos();
	Vector2D initialVel(
			(pos.getX() < 1.0f) ? GHOST_SPEED : -GHOST_SPEED,
			(pos.getY() < 1.0f) ? GHOST_SPEED : -GHOST_SPEED);

	// Lo spawneamos en una esquina random.
	_mngr->addComponent<Transform>(
			ghost,
			pos,
			initialVel,
			GHOST_SIZE,
			GHOST_SIZE,
			0.0f);
}

void GhostSystem::updateGhosts(float dt) {
	auto pacman = _mngr->getHandler(ecs::hdlr::PACMAN);
	auto pmTr = _mngr->getComponent<Transform>(pacman);
	if (pmTr == nullptr)
		return;

	auto &ghosts = _mngr->getEntities(ecs::grp::GHOSTS);
	for (auto e : ghosts) {
		if (!_mngr->isAlive(e))
			continue;

		auto tr = _mngr->getComponent<Transform>(e);
		if (tr == nullptr)
			continue;

		// Con poca probabilidad en algun caso se mueve hacia Pacman
		if (sdlutils().rand().nextInt(0, 10000) < (int)(RETARGET_PROB * 10000.0f)) {
			Vector2D toPacman = pmTr->getPos() - tr->getPos();
			if (toPacman.magnitude() > 0.0f) {
				tr->getVel() = toPacman.normalize() * GHOST_SPEED;
			}
		}

		tr->update();
		handleWallBounce(e);
	}
}

void GhostSystem::handleWallBounce(ecs::entity_t ghost) {
	auto tr = _mngr->getComponent<Transform>(ghost);
	if (tr == nullptr)
		return;

	float screenW = static_cast<float>(sdlutils().width());
	float screenH = static_cast<float>(sdlutils().height());

	auto &pos = tr->getPos();
	auto &vel = tr->getVel();
	float w = tr->getWidth();
	float h = tr->getHeight();

	if (pos.getX() < 0.0f) {
		pos.setX(0.0f);
		if (vel.getX() < 0.0f)
			vel.setX(-vel.getX());
	} else if (pos.getX() + w > screenW) {
		pos.setX(screenW - w);
		if (vel.getX() > 0.0f)
			vel.setX(-vel.getX());
	}

	if (pos.getY() < 0.0f) {
		pos.setY(0.0f);
		if (vel.getY() < 0.0f)
			vel.setY(-vel.getY());
	} else if (pos.getY() + h > screenH) {
		pos.setY(screenH - h);
		if (vel.getY() > 0.0f)
			vel.setY(-vel.getY());
	}
}

Vector2D GhostSystem::randomCornerPos() const {
	int corner = sdlutils().rand().nextInt(0, 4);
	float maxX = static_cast<float>(sdlutils().width()) - GHOST_SIZE;
	float maxY = static_cast<float>(sdlutils().height()) - GHOST_SIZE;

	switch (corner) {
	case 0:
		return Vector2D(0.0f, 0.0f);
	case 1:
		return Vector2D(maxX, 0.0f);
	case 2:
		return Vector2D(0.0f, maxY);
	default:
		return Vector2D(maxX, maxY);
	}
}

int GhostSystem::countAliveGhosts() const {
	int count = 0;
	auto &ghosts = _mngr->getEntities(ecs::grp::GHOSTS);
	for (auto e : ghosts) {
		if (_mngr->isAlive(e))
			++count;
	}
	return count;
}

void GhostSystem::clearAllGhosts() {
	auto &ghosts = _mngr->getEntities(ecs::grp::GHOSTS);
	for (auto e : ghosts) {
		if (_mngr->isAlive(e))
			_mngr->setAlive(e, false);
	}
}

void GhostSystem::removeGhost(ecs::entity_t ghost) {
	if (ghost == nullptr)
		return;
	if (_mngr->isAlive(ghost))
		_mngr->setAlive(ghost, false);
}
