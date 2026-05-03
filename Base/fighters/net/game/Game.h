// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <SDL_stdinc.h>
#include <vector>

#include "../utils/Singleton.h"

class Fighter;
class Bullets;
class Networking;

class Game : public Singleton<Game> {
	friend Singleton<Game>;

public:
	bool init_game(const char *host, Uint16 port);
	void start();

	Fighter& get_fighters() {
		return *_fighters;
	}

	Bullets& get_bullets() {
		return *_bm;
	}

	inline Networking& get_networking() {
		return *_net;
	}
private:
	Game();
	virtual ~Game();
	bool init();

	void check_collisions();


	Bullets *_bm;
	Fighter *_fighters;
	Networking *_net;

};

