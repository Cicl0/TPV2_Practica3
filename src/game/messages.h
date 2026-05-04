// messages.h - Definición de mensajes para LittleWolf en red
#pragma once

#include "../sdlutils/SDLNetUtils.h"
#include <SDL_stdinc.h>
#include "../utils/Vector2D.h"

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
	Uint8 id;
	float x, y;
	float rot;
	float vel;
	Uint8 state; // 0 = muerto, 1 = vivo
	_IMPL_SERIALIZATION_(id, x, y, rot, vel, state)
};

// Mensaje de disparo
struct ShootMsg {
	Uint8 id;
	float x, y;
	float dir_x, dir_y;
	_IMPL_SERIALIZATION_(id, x, y, dir_x, dir_y)
};

// Mensaje de muerte
struct DeadMsg {
	Uint8 id;
	Uint8 shooter;
	_IMPL_SERIALIZATION_(id, shooter)
};

// Mensaje de reinicio
struct RestartMsg {
	// aquí puedes ampliar con posiciones si lo deseas
	Uint8 dummy; // solo para compatibilidad / alineamiento
	_IMPL_SERIALIZATION_(dummy)
};
