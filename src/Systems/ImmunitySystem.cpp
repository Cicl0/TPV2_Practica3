// This file is part of the course TPV2@UCM - Samir Genaim

#include "ImmunitySystem.h"

#include "../ecs/EntityManager.h"
#include "../sdlutils/SDLUtils.h"

namespace {
constexpr float DEFAULT_IMMUNITY_DURATION = 5.0f;
}

ImmunitySystem::ImmunitySystem() :
		_active(false),
		_duration(0.0f),
		_counter(0.0f),
		_lastUpdateTime(0.0f) {
}

ImmunitySystem::~ImmunitySystem() {
}

void ImmunitySystem::initSystem() {
	_lastUpdateTime = static_cast<float>(sdlutils().currRealTime());
}

void ImmunitySystem::update() {
	float now = static_cast<float>(sdlutils().currRealTime());
	float dt = (now - _lastUpdateTime) / 1000.0f;
	if (dt < 0.0f)
		dt = 0.0f;
	_lastUpdateTime = now;

	if (!_active)
		return;

	_counter += dt;
	if (_counter >= _duration) {
		endImmunity();
	}
}

void ImmunitySystem::recieve(const Message &msg) {
	switch (msg.id) {
	case _m_NEW_GAME:
	case _m_ROUND_OVER:
	case _m_GAME_OVER:
		endImmunity();
		break;
	case _m_IMMUNITY_START:
		startImmunity(msg.immunity_data.duration);
		break;
	default:
		break;
	}
}

void ImmunitySystem::startImmunity(float duration) {
	_active = true;
	_counter = 0.0f;
	_duration = (duration > 0.0f) ? duration : DEFAULT_IMMUNITY_DURATION;
}

void ImmunitySystem::endImmunity() {
	if (_active) {
		Message m;
		m.id = _m_IMMUNITY_END;
		m.immunity_data.duration = 0.0f;
		_mngr->send(m, true);
	}

	_active = false;
	_counter = 0.0f;
	_duration = 0.0f;
}
