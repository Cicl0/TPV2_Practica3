#pragma once

#include "../ecs/Component.h"

struct PacManComponent : public ecs::Component {

	PacManComponent(int initialLives = 3) :
			_lives(initialLives),
			_immune(false),
			_immunityTimeLeft(0.0f) {
	}

	virtual ~PacManComponent() {
	}

	void resetForNewGame(int initialLives = 3) {
		_lives = initialLives;
		_immune = false;
		_immunityTimeLeft = 0.0f;
	}

	void resetForNewRound() {
		_immune = false;
		_immunityTimeLeft = 0.0f;
	}

	int getLives() const {
		return _lives;
	}

	void setLives(int lives) {
		_lives = lives;
	}

	void loseLife() {
		if (_lives > 0)
			--_lives;
	}

	bool isImmune() const {
		return _immune;
	}

	void startImmunity(float durationSeconds) {
		if (!_immune) {
			_immune = true;
			_immunityTimeLeft = durationSeconds;
		}
	}

	void stopImmunity() {
		_immune = false;
		_immunityTimeLeft = 0.0f;
	}

	float getImmunityTimeLeft() const {
		return _immunityTimeLeft;
	}

	// Actualizar el tiempo restante de inmunidad.
	void updateImmunity(float dt) {
		if (!_immune)
			return;

		_immunityTimeLeft -= dt;
		if (_immunityTimeLeft <= 0.0f)
			stopImmunity();
	}

private:
	int _lives;
	bool _immune;
	float _immunityTimeLeft;
};
