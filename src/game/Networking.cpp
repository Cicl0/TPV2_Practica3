// Networking.cpp - LittleWolf en red
#include "Networking.h"

#include <iostream>
#include <memory>

#include "../sdlutils/SDLNetUtils.h"
#include <SDL_net.h>

Networking::Networking() : sock(nullptr), _client_Id(0), _master_Id(0) {}
Networking::~Networking() { disconnect(); }

bool Networking::init(const char* host, Uint16 port) {
	// Resolve hostname
	NET_Address* addr = NET_ResolveHostname(host);
	if (!addr) {
		std::cerr << "Networking::init: NET_ResolveHostname returned nullptr\n";
		return false;
	}
	if (NET_WaitUntilResolved(addr, 2000) != NET_SUCCESS) {
		std::cerr << "Networking::init: DNS resolve failed for host " << host << std::endl;
		NET_UnrefAddress(addr);
		return false;
	}

	// Create client
	NET_StreamSocket* stream = NET_CreateClient(addr, port);
	NET_UnrefAddress(addr);
	if (!stream) {
		std::cerr << "Networking::init: NET_CreateClient failed\n";
		return false;
	}

	// Wait until connected (non-blocking style with timeout)
	if (NET_WaitUntilConnected(stream, 3000) != NET_SUCCESS) {
		std::cerr << "Networking::init: connection to " << host << ":" << port << " failed\n";
		NET_DestroyStreamSocket(stream);
		return false;
	}

	// store socket
	void* sockets[1] = { stream };

	if (NET_WaitUntilInputAvailable(sockets, 1, 3000) <= 0) {
		std::cerr << "Timeout esperando mensaje del servidor\n";
		return false;
	}

	SDLNetUtils::buff_t buf = SDLNetUtils::receive(stream);

	if (buf.error || buf.size == 0) {
		std::cerr << "Networking::init: did not receive acceptance message from server\n";
		NET_DestroyStreamSocket(stream);
		sock = nullptr;
		return false;
	}

	// Inspect first byte to know message type
	Uint8 type = buf.data[0];

	if (type == _CONN_REQUEST_ACCEPTED) {
		MsgWithMasterId m;
		m.deserialize(buf.data);
		_client_Id = m.clientId;
		_master_Id = static_cast<Uint8>(m.masterId < 0 ? 0 : m.masterId);
		std::cout << "Connected. Assigned client id: " << (int)_client_Id
			<< " master: " << (int)_master_Id << std::endl;
	}
	else {
		// Unexpected, but attempt to deserialize as generic Msg
		Msg m;
		m.deserialize(buf.data);
		std::cout << "Networking::init: unexpected first message type " << (int)m.type << std::endl;
	}

	return true;
}

void Networking::disconnect() {
	if (sock != nullptr) {
		NET_StreamSocket* s = static_cast<NET_StreamSocket*>(sock);
		NET_DestroyStreamSocket(s);
		sock = nullptr;
		std::cout << "Networking: disconnected\n";
	}
}

void Networking::update() {
	if (sock == nullptr)
		return;

	NET_StreamSocket* s = static_cast<NET_StreamSocket*>(sock);

	// Try to receive a packet (SDLNetUtils::receive reads length header and payload)
	SDLNetUtils::buff_t buf = SDLNetUtils::receive(s);

	// If there was an error, assume connection closed
	if (buf.error) {
		std::cerr << "Networking::update: socket error or disconnected\n";
		disconnect();
		return;
	}

	if (buf.size == 0)
		return; // nothing received

	// First byte is the message type
	Uint8 type = buf.data[0];

	switch (type) {
	case _CONN_REQUEST_ACCEPTED: {
		MsgWithMasterId m;
		m.deserialize(buf.data);
		_client_Id = m.clientId;
		_master_Id = static_cast<Uint8>(m.masterId < 0 ? 0 : m.masterId);
		std::cout << "Networking: accepted as client " << (int)_client_Id
			<< " master " << (int)_master_Id << std::endl;
		handle_new_client(_client_Id);
		break;
	}
	case _CLIENT_CONNECTED: {
		MsgWithMasterId m;
		m.deserialize(buf.data);
		_master_Id = static_cast<Uint8>(m.masterId < 0 ? 0 : m.masterId);
		std::cout << "Networking: client connected id=" << (int)m.clientId
			<< " new master=" << (int)_master_Id << std::endl;
		handle_new_client(m.clientId);
		break;
	}
	case _CLIENT_DISCONNECTED: {
		MsgWithMasterId m;
		m.deserialize(buf.data);
		_master_Id = static_cast<Uint8>(m.masterId < 0 ? 0 : m.masterId);
		std::cout << "Networking: client disconnected id=" << (int)m.clientId
			<< " new master=" << (int)_master_Id << std::endl;
		handle_disconnect(m.clientId);
		break;
	}
	case _PLAYER_STATE: {
		PlayerStateMsg pm;
		pm.deserialize(buf.data);
		handle_player_state(pm);
		break;
	}
	case _SHOOT: {
		ShootMsg sm;
		sm.deserialize(buf.data);
		handle_shoot(sm);
		break;
	}
	case _DEAD: {
		DeadMsg dm;
		dm.deserialize(buf.data);
		handle_dead(dm);
		break;
	}
	case _RESTART: {
		RestartMsg rm;
		rm.deserialize(buf.data);
		handle_restart(rm);
		break;
	}
	default: {
		Msg m;
		m.deserialize(buf.data);
		std::cout << "Networking::update: unknown message type " << (int)m.type << std::endl;
		break;
	}
	}
}

void Networking::send_player_state(const PlayerStateMsg& msg) {
	if (!sock) return;
	SDLNetUtils::serialized_send(const_cast<PlayerStateMsg&>(msg),
		static_cast<NET_StreamSocket*>(sock));
}

void Networking::send_shoot(const ShootMsg& msg) {
	if (!sock) return;
	SDLNetUtils::serialized_send(const_cast<ShootMsg&>(msg),
		static_cast<NET_StreamSocket*>(sock));
}

void Networking::send_dead(const DeadMsg& msg) {
	if (!sock) return;
	SDLNetUtils::serialized_send(const_cast<DeadMsg&>(msg),
		static_cast<NET_StreamSocket*>(sock));
}

void Networking::send_restart(const RestartMsg& msg) {
	if (!sock) return;
	SDLNetUtils::serialized_send(const_cast<RestartMsg&>(msg),
		static_cast<NET_StreamSocket*>(sock));
}

void Networking::handle_new_client(Uint8 id) {
	// Por ahora solo debug. Si quieres notificar al Game/LittleWolf
	// añade un callback o referencia al constructor de Networking.
	(void)id;
}

void Networking::handle_disconnect(Uint8 id) {
	// Por ahora solo debug
	(void)id;
}

void Networking::handle_player_state(const PlayerStateMsg& msg) {
	// Por ahora solo debug; integrar con LittleWolf para actualizar estado
	std::cout << "Networking: player state id=" << (int)msg.id
		<< " x=" << msg.x << " y=" << msg.y << " rot=" << msg.rot
		<< " state=" << (int)msg.state << std::endl;
}

void Networking::handle_shoot(const ShootMsg& msg) {
	std::cout << "Networking: shoot from " << (int)msg.id
		<< " pos=(" << msg.x << "," << msg.y << ")" << std::endl;
}

void Networking::handle_dead(const DeadMsg& msg) {
	std::cout << "Networking: dead id=" << (int)msg.id
		<< " shooter=" << (int)msg.shooter << std::endl;
}

void Networking::handle_restart(const RestartMsg& msg) {
	std::cout << "Networking: restart message received\n";
	// Si RestartMsg incluye posiciones, aplicar aquí llamando a Game/LittleWolf.
	(void)msg;
}