// messages.h - Definición de mensajes para LittleWolf en red
#pragma once

#include "../sdlutils/SDLNetUtils.h"
#include <SDL_stdinc.h>

// máximo de jugadores (coincide con LittleWolf::_max_player)
static constexpr size_t LW_MAX_PLAYERS = 10;

// Tipos de mensaje básicos usados por el servidor
enum MsgType : Uint8 {
	_CONN_REQUEST_ACCEPTED = 1,
	_CONN_REQUEST_REJECTED = 2,
	_CLIENT_CONNECTED = 3,
	_CLIENT_DISCONNECTED = 4,
	_SERVER_SHUTDOWN = 5,
	// tipos específicos del juego (ejemplo)
	_PLAYER_STATE = 10,
	_SHOOT = 11,
	_DEAD = 12,
	_RESTART = 13
};

// Mensaje genérico (solo tipo)
struct Msg {
	Uint8 type;
	_IMPL_SERIALIZATION_(type)
};

// Mensaje que incluye id de cliente y id del master para notificaciones del servidor
struct MsgWithMasterId {
	Uint8 type;
	Uint8 clientId;
	Sint8 masterId; // -1 si no hay master
	_IMPL_SERIALIZATION_(type, clientId, masterId)
};

// Mensaje de estado de jugador
struct PlayerStateMsg {
	Uint8 type = _PLAYER_STATE;
	Uint8 id;
	float prev_x, prev_y;
	float x, y;
	float rot;
	float vel;
	Uint8 state; // 0 = muerto, 1 = vivo
	_IMPL_SERIALIZATION_(type, id, prev_x, prev_y, x, y, rot, vel, state)
};

// Mensaje de disparo
struct ShootMsg {
	Uint8 type = _SHOOT;
	Uint8 id;
	float x, y;
	float dir_x, dir_y;
	_IMPL_SERIALIZATION_(type, id, x, y, dir_x, dir_y)
};

// Mensaje de muerte
struct DeadMsg {
	Uint8 type = _DEAD;
	Uint8 id;
	Uint8 shooter;
	_IMPL_SERIALIZATION_(type, id, shooter)
};

// Mensaje de reinicio: posiciones para hasta LW_MAX_PLAYERS
struct RestartMsg {
	Uint8 type; 
	// arrays de posiciones; si used[i]==0, la entrada se ignora
	float x[LW_MAX_PLAYERS];
	float y[LW_MAX_PLAYERS];
	Uint8 used[LW_MAX_PLAYERS];
	_IMPL_SERIALIZATION_(type, x, y, used)
};