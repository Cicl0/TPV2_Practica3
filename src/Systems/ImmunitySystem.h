// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../ecs/System.h"
#include "../sdlutils/SDLUtils.h"

class ImmunitySystem: public ecs::System {
public:
	ImmunitySystem();
	virtual ~ImmunitySystem();

	void initSystem() override;
	void update() override;
	void recieve(const Message &msg) override;

private:
	void startImmunity(float duration);
	void endImmunity();

	bool _active;
	float _duration;
	float _counter;
	float _lastUpdateTime;
};
