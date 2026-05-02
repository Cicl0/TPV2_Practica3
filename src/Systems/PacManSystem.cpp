// This file is part of the course TPV2@UCM - Samir Genaim

#include "PacManSystem.h"

#include "../components/Transform.h"
#include "../ecs/EntityManager.h"
#include "../sdlutils/InputHandler.h"
#include "../sdlutils/SDLUtils.h"

void PacManSystem::setPacMan(entity_t pacman) {
	_pacman = pacman;
	_pacmanTransform = _mngr->getComponent<Transform>(pacman);
	_pacmanComponent = _mngr->getComponent<PacManComponent>(pacman);
}

void PacManSystem::update() {
	if (_pacman == nullptr || _pacmanTransform == nullptr || _pacmanComponent == nullptr)
		return;

	handleInput();
	updatePosition();
	checkBoundaryCollisions();
}

void PacManSystem::recieve(const Message& msg) {
	switch (msg.id) {
	case _m_NEW_GAME:
		onNewGame();
		break;
	case _m_ROUND_START:
		onNewRound();
		break;
	default:
		break;
	}
}


void PacManSystem::handleInput() {
	InputHandler& input = *InputHandler::Instance();

	if (input.keyDownEvent()) {
		// Flecha hacia arriba cambia velocidad a (0, -Velocidad).rotate(rot)
		if (input.isKeyDown(SDL_SCANCODE_UP)) {
			Vector2D newVel(0.0f, -_SPEED);
			newVel = newVel.rotate(_pacmanTransform->getRot());
			_pacmanTransform->getVel() = newVel;
		}
		// Flecha hacia abajo se para velocidad = 0
		else if (input.isKeyDown(SDL_SCANCODE_DOWN)) {
			_pacmanTransform->getVel() = Vector2D(0.0f, 0.0f);
		}

		// Flechas izquierda/derecha: giran solo cuando se pulsa la tecla,
		// no en cada frame.
		if (input.isKeyDown(SDL_SCANCODE_LEFT)) {
			rotatePacman(-90.0f);
		}
		else if (input.isKeyDown(SDL_SCANCODE_RIGHT)) {
			rotatePacman(90.0f);
		}
	}
}

void PacManSystem::rotatePacman(float deltaDegrees) {
	float currentRot = _pacmanTransform->getRot();
	_pacmanTransform->setRot(currentRot + deltaDegrees);

	Vector2D currentVel = _pacmanTransform->getVel();
	if (currentVel.magnitude() > 0.0f) {
		currentVel = currentVel.rotate(deltaDegrees);
		_pacmanTransform->getVel() = currentVel;
	}
}

void PacManSystem::updatePosition() {
	_pacmanTransform->update();
}

void PacManSystem::checkBoundaryCollisions() {
	Vector2D pos = _pacmanTransform->getPos();
	float width = _pacmanTransform->getWidth();
	float height = _pacmanTransform->getHeight();
	Vector2D vel = _pacmanTransform->getVel();

	int screenWidth = sdlutils().width();
	int screenHeight = sdlutils().height();

	// Colisión con borde izquierdo
	if (pos.getX() < 0.0f && vel.getX() < 0.0f) {
		_pacmanTransform->getVel().setX(0.0f);
		_pacmanTransform->getPos().setX(0.0f);
	}
	// Colisión con borde derecho
	else if (pos.getX() + width > screenWidth && vel.getX() > 0.0f) {
		_pacmanTransform->getVel().setX(0.0f);
		_pacmanTransform->getPos().setX(screenWidth - width);
	}

	// Colisión con borde superior
	if (pos.getY() < 0.0f && vel.getY() < 0.0f) {
		_pacmanTransform->getVel().setY(0.0f);
		_pacmanTransform->getPos().setY(0.0f);
	}
	// Colisión con borde inferior
	else if (pos.getY() + height > screenHeight && vel.getY() > 0.0f) {
		_pacmanTransform->getVel().setY(0.0f);
		_pacmanTransform->getPos().setY(screenHeight - height);
	}
}

void PacManSystem::resetPosition() {
	if (_pacmanTransform == nullptr)
		return;

	// Lo colocamos en el centro de la pantalla
	float centerX = sdlutils().width()  / 2.0f ;
	float centerY = sdlutils().height() / 2.0f;

	_pacmanTransform->getPos() = Vector2D(centerX - _pacmanTransform->getWidth() / 2.0f, centerY - _pacmanTransform->getHeight() / 2.0f);
	_pacmanTransform->getVel() = Vector2D(0.0f, 0.0f);
	_pacmanTransform->setRot(0.0f);
}

// Métodos públicos para ser llamados desde Game 
void PacManSystem::onNewGame() {
	if (_pacmanComponent != nullptr) {
		_pacmanComponent->resetForNewGame();
	}
	resetPosition();
}

void PacManSystem::onNewRound() {
	if (_pacmanComponent != nullptr) {
		_pacmanComponent->resetForNewRound();
	}
	resetPosition();
}
