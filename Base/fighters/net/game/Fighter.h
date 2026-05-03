// This file is part of the course TPV2@UCM - Samir Genaim

/*
 * The content of this class is based on https://glouw.com/2018/03/11/littlewolf.html
 */

#pragma once

#include <SDL.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <array>
#include <string>
#include <fstream>

#include "BulletsManager.h"
#include "../sdlutils/InputHandler.h"
#include "../utils/Vector2D.h"

class Texture;
class InputHandler;

class Fighter {
public:

	// the status of a player
	enum PlayerState : Uint8 {
		NOT_USED, ALIVE, DEAD
	};

	// player information
	struct Player {
		Uint8 id;          // the id
		Vector2D vel;        // vel
		Vector2D pos;        // position;
		int width;
		int height;
		float speed;         // maximum speed
		float rot;         // rotation
		PlayerState state;   // the state
		Uint32 lastShoot; // last time the player shot
	};

	// maximum number of player
	static constexpr Uint8 max_player = 10;

	typedef std::array<Player, max_player> players_array;
	typedef players_array::iterator iterator;

	iterator begin() {
		return _players.begin();
	}

	iterator end() {
		return _players.end();
	}

	Fighter(BulletManager &bm);
	virtual ~Fighter();

	// add a new player with identifier <id>
	bool addPlayer(Uint8 id);

	//
	void die(Player &p) {
		p.state = Fighter::DEAD;
		if (p.id == _local_player_id) {
			switchToNextPlayer();
		}
	}

	// mark all (used) player alive
	void restart();

	// switch to the view of the next player
	void switchToNextPlayer();

	// render the walls, etc
	void render();

	// update the world, tec
	void update();

	void removePlayer(Uint8 id);
	void update_player_state(Uint8 id, float x, float y, float w, float h,
			float rot);
	void killPlayer(Uint8 id);
	void update_player_info(Uint8 id, float x, float y, float w, float h,
			float rot, Uint8 state);
	void send_my_info();

private:
	void resetPlayer(Uint8 id);
	void shoot(Player &p);
	void spin(Player &p);
	void move(Player &p);
	void draw_limits(Player &p);


	// array of players
	std::array<Player, max_player> _players;

	// id of the current player, used since we allows switching between players
	Uint8 _local_player_id;

	const Texture &_fighter_img;
	InputHandler &_ihdlr;
	BulletManager &_bm;
	bool _show_help;

};

