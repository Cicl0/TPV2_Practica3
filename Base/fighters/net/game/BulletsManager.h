// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once
#include <SDL_stdinc.h>

#include "../utils/Vector2D.h"


class BulletManager {
public:
	virtual ~BulletManager() {
	}
	virtual void shoot(Vector2D p, Vector2D v, int width, int height,
			float r, Uint8 shooter, Uint32 timestamp) = 0;
	virtual void reset() = 0;
	virtual void disable_bullet(Uint8 shooter, Uint32 timestamp) = 0;
};


