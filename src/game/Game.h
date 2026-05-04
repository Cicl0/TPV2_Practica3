// This file is part of the course TPV2@UCM - Samir Genaim

#include "Networking.h"

#pragma once
class LittleWolf;

enum State {
	NEWGAME,
	NEWROUND,
	GAMEOVER,
	PAUSED,
	RUNNING,
	WAITING_RESTART
};

struct LWPlayer {
	int id;
	float x, y;
	float angle;
	bool alive = true;
	int health = 100;
	int score = 0;
	char name[11];
};

class Networking; // Forward declaration

class Game {
public:
	Game();
	virtual ~Game();

	// Inicialización para modo red
	bool init_game(const char *host, unsigned short port);

	void start();

	void setState(State state);

private:
	LittleWolf *_little_wolf;
	Networking *_networking;
	State _state;
	bool _network_mode = false;

	// --- New fields for restart countdown ---
	bool _restartPending = false;
	unsigned int _restartStartTime = 0; // ms
	const float _restartDelay = 5.0f; // seconds
};

