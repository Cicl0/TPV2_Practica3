// messages.h - Definición de mensajes para LittleWolf en red
#pragma once
#include <SDL_stdinc.h>
#include "../utils/Vector2D.h"

// Mensaje de estado de jugador
struct PlayerStateMsg {
    Uint8 id;
    float x, y;
    float rot;
    float vel;
    Uint8 state; // 0 = muerto, 1 = vivo
};

// Mensaje de disparo
struct ShootMsg {
    Uint8 id;
    float x, y;
    float dir_x, dir_y;
};

// Mensaje de muerte
struct DeadMsg {
    Uint8 id;
    Uint8 shooter;
};

// Mensaje de reinicio
struct RestartMsg {
    Uint8 dummy; // solo para compatibilidad
};
