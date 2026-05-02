#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/SDLUtils.h"

struct FruitComponent : public ecs::Component {

	FruitComponent() :
			_isMagic(false),
			_isCurrentlyMagic(false),
			_normalDurationN(0),
			_magicDurationM(0),
			_nextStateChangeTime(0.0f),
			_stateStartTime(0.0f) {
		// 10% probabilidad de ser fruta milagrosa
		auto &rng = sdlutils().rand();
		int roll = rng.nextInt(0, 100);
		if (roll < 10) {
			_isMagic = true;
			// N es entre 10 y 20 segundos
			_normalDurationN = rng.nextInt(10, 21);
			_nextStateChangeTime = static_cast<float>(_normalDurationN);
		}
	}

	virtual ~FruitComponent() {
	}

	bool isMagic() const {
		return _isMagic;
	}

	bool isCurrentlyMagic() const {
		return _isCurrentlyMagic;
	}

	void update(float dt) {
		if (!_isMagic)
			return;

		_stateStartTime += dt;

		if (!_isCurrentlyMagic) {
			// Esperando para pasar a milagroso
			if (_stateStartTime >= _nextStateChangeTime) {
				_isCurrentlyMagic = true;
				_stateStartTime = 0.0f;
				// M es entre 1 y 5 segundos
				auto &rng = sdlutils().rand();
				_magicDurationM = rng.nextInt(1, 6);
				_nextStateChangeTime = static_cast<float>(_magicDurationM);
			}
		} else {
			// Esperando para pasar a normal
			if (_stateStartTime >= _nextStateChangeTime) {
				_isCurrentlyMagic = false;
				_stateStartTime = 0.0f;
				_nextStateChangeTime = static_cast<float>(_normalDurationN);
			}
		}
	}

	// Resetea componente para nueva ronda
	void resetForNewRound() {
		if (_isMagic) {
			_isCurrentlyMagic = false;
			_stateStartTime = 0.0f;
			_nextStateChangeTime = static_cast<float>(_normalDurationN);
		}
	}

private:
	bool _isMagic;				// Si es  milagrosa
	bool _isCurrentlyMagic;		// Si está actualmente en estado milagroso
	int _normalDurationN;		// Duración en estado normal 
	int _magicDurationM;			// Duración en estado milagroso
	float _nextStateChangeTime;	// Cuándo ocurre el próximo cambio
	float _stateStartTime;		// Tiempo acumulado en estado actual
};
