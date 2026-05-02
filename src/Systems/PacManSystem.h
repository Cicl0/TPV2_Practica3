#pragma once

#include "../ecs/System.h"
#include "../ecs/EntityManager.h"
#include "../components/Transform.h"
#include "../components/PacManComponent.h"
#include "../sdlutils/SDLUtils.h"
#include "../sdlutils/InputHandler.h"
#include <SDL.h>

using entity_t = ecs::Entity*;

class PacManSystem : public ecs::System {
public:
	PacManSystem() : 
		_pacman(nullptr),
		_pacmanTransform(nullptr),
		_pacmanComponent(nullptr) {
	}

	virtual ~PacManSystem() {
	}

	void initSystem() {
	}

	void setPacMan(entity_t pacman);

	void update();
//Esto para el tema de los mensajes
	void recieve(const Message& msg);

	// Métodos públicos para ser llamados desde Game 
	void onNewGame();

	void onNewRound();

private:
	entity_t _pacman;
	Transform* _pacmanTransform;
	PacManComponent* _pacmanComponent;

	// Constantes
	static constexpr float _SPEED = 3.0f;

	void handleInput();

	void updatePosition();

	void checkBoundaryCollisions();

	void resetPosition();
	void rotatePacman(float deltaDegrees);

};
