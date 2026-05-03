// Networking.cpp - LittleWolf en red
#include "Networking.h"

Networking::Networking() : sock(nullptr), _client_Id(0), _master_Id(0) {}
Networking::~Networking() { disconnect(); }

bool Networking::init(const char *host, Uint16 port) {
    // Inicializar conexión de red
    return true;
}

void Networking::disconnect() {
    // Cerrar conexión
}

void Networking::update() {
    // Procesar mensajes entrantes
}

void Networking::send_player_state(const PlayerStateMsg &msg) {
    // Enviar estado del jugador (msg)
}

void Networking::send_shoot(const ShootMsg &msg) {
    // Enviar disparo (msg)
}

void Networking::send_dead(const DeadMsg &msg) {
    // Enviar muerte (msg)
}

void Networking::send_restart(const RestartMsg &msg) {
    // Enviar reinicio (msg)
}

void Networking::handle_new_client(Uint8 id) {
    // Lógica para nuevo cliente
}

void Networking::handle_disconnect(Uint8 id) {
    // Lógica para desconexión
}

void Networking::handle_player_state(const PlayerStateMsg &msg) {
    // Lógica para estado de jugador (msg)
}

void Networking::handle_shoot(const ShootMsg &msg) {
    // Lógica para disparo (msg)
}

void Networking::handle_dead(const DeadMsg &msg) {
    // Lógica para muerte (msg)
}

void Networking::handle_restart(const RestartMsg &msg) {
    // Lógica para reinicio (msg)
}
