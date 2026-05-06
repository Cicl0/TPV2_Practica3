// Networking.h - LittleWolf en red
#pragma once
#include <SDL_stdinc.h>
#include "messages.h"

class LittleWolf; // forward

class Networking {
public:
    Networking();
    virtual ~Networking();

    bool init(const char* host, Uint16 port);
    void disconnect();
    void update();

    Uint8 get_client_id() const { return _client_Id; }
    bool is_master() const { return _client_Id == _master_Id; }

    void send_player_state(const PlayerStateMsg& msg);
    void send_shoot(const ShootMsg& msg);
    void send_dead(const DeadMsg& msg);
    void send_restart(const RestartMsg& msg);

    // permitir que Networking invoque metodos de LittleWolf
    void setLittleWolf(LittleWolf* lw) { _lw = lw; }

private:
    void handle_new_client(Uint8 id);
    void handle_disconnect(Uint8 id);
    void handle_player_state(const PlayerStateMsg& msg);
    void handle_shoot(const ShootMsg& msg);
    void handle_dead(const DeadMsg& msg);
    void handle_restart(const RestartMsg& msg);

    void* sock;
    Uint8 _client_Id;
    Uint8 _master_Id;

    LittleWolf* _lw = nullptr;
};