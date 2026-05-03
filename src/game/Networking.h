// Networking.h - LittleWolf en red
#pragma once
#include <SDL_stdinc.h>
#include "../utils/Vector2D.h"
#include "messages.h"

class Networking {
public:
    Networking();
    virtual ~Networking();

    bool init(const char *host, Uint16 port);
    void disconnect();
    void update();

    Uint8 get_client_id() const { return _client_Id; }
    bool is_master() const { return _client_Id == _master_Id; }

    // Métodos para enviar mensajes de juego
    void send_player_state(const PlayerStateMsg &msg);
    void send_shoot(const ShootMsg &msg);
    void send_dead(const DeadMsg &msg);
    void send_restart(const RestartMsg &msg);

private:
    // Métodos internos para manejar mensajes recibidos
    void handle_new_client(Uint8 id);
    void handle_disconnect(Uint8 id);
    void handle_player_state(const PlayerStateMsg &msg);
    void handle_shoot(const ShootMsg &msg);
    void handle_dead(const DeadMsg &msg);
    void handle_restart(const RestartMsg &msg);

    // Aquí irán los sockets y datos de red
    void *sock; // Cambiar por el tipo real
    Uint8 _client_Id;
    Uint8 _master_Id;
};
