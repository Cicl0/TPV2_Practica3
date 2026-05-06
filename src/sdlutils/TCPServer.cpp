#include "TCPServer.h"

#include <cassert>
#include <csignal>

#include "../game/messages.h"

Uint16 TCPServer::shutDownTriggetPort = 0;
bool TCPServer::done = true;

TCPServer::TCPServer(Uint8 maxClients) :
	maxClients(maxClients), //
	serv(nullptr), //
	usedConn(0) {
	conn = new void* [maxClients + 1] {};
	clients = new NET_StreamSocket * [maxClients] {};
}

TCPServer::~TCPServer() {
	shutdown();
	delete[] conn;
	delete[] clients;
}

// this just connects to the server to wake it up.
void TCPServer::shutdownTrigger() {

	if (done)
		return;

	std::cout << std::endl << "Shutting down  ..." << std::endl;

	done = true;

	if (shutDownTriggetPort != 0) {

		// Resolve the localhost to an IP address
		NET_Address* addr = NET_ResolveHostname("localhost");
		if (NET_WaitUntilResolved(addr, 1000) != NET_SUCCESS) {
			SDLNetUtils::print_SDLNet_error_and_exit(1);
		}

		// Connect to the server, just to wake it up if it is sleeping
		NET_StreamSocket* conn = NET_CreateClient(addr, shutDownTriggetPort);
		if (NET_WaitUntilConnected(conn, 1000) != NET_SUCCESS) {
			SDLNetUtils::print_SDLNet_error_and_exit(1);
		}

		// UnReference the address so it can be freed
		NET_UnrefAddress(addr);

		// Destroy the socket
		NET_DestroyStreamSocket(conn);
	}
}

/* Signal Handler for SIGINT */
void TCPServer::handlerSIGINT(int) {
	TCPServer::shutdownTrigger();
}

bool TCPServer::connect(const Uint16 port) {

	TCPServer::shutDownTriggetPort = port;
	signal(SIGINT, TCPServer::handlerSIGINT);

	serv = NET_CreateServer(nullptr, port);
	if (!serv) {
#ifndef NDEBUG
		SDLNetUtils::print_SDLNet_error();
#endif
		return false;
	}

	conn[0] = serv;
	++usedConn;

	done = false;

	std::cout << "The server is up and kicking ..." << std::endl;

	return true;
}

void TCPServer::listen() {

	while (!done) {
		// Espera hasta que alguien entre al servidor
		if (NET_WaitUntilInputAvailable(conn, usedConn, SDL_MAX_SINT32) > 0) {

			// si se cierra
			if (done)
				continue;

			NET_StreamSocket* client = nullptr;
			if (NET_AcceptClient(serv, &client) && client != nullptr) {

				if (usedConn - 1 < maxClients) {
					Uint8 j = 0;
					while (j < maxClients && clients[j] != nullptr)
						j++;

					assert(j < maxClients);

					conn[usedConn] = clients[j] = client;
					std::cout << "New client assigned id " << (int)j
						<< std::endl;
					++usedConn;

					MsgWithMasterId m;
					m.type = _CONN_REQUEST_ACCEPTED;
					m.clientId = j;
					m.masterId = whoIsTheMaster();
					SDLNetUtils::serialized_send(m, client);

					// Comunica a los otros clientes que hay uno nuevo conectado
					m.type = _CLIENT_CONNECTED; 
					for (auto i = 1u; i < usedConn - 1; i++) {
						if (client != static_cast<NET_StreamSocket*>(conn[i]))
							SDLNetUtils::serialized_send(m,
								static_cast<NET_StreamSocket*>(conn[i]));
					}
				}
				else {
					Msg m;
					m.type = _CONN_REQUEST_REJECTED;
					SDLNetUtils::serialized_send(m, client);
				}
			}

			for (auto i = 1u; i < usedConn; i++) {

    				void* socketArray[1] = { conn[i] };
    				if (NET_WaitUntilInputAvailable(socketArray, 1, 0) <= 0)
        			continue;

    // Lee mensaje
    SDLNetUtils::buff_t buf = SDLNetUtils::receive(
        static_cast<NET_StreamSocket*>(conn[i]));

    if (!buf.error) { 
        if (buf.size > 0) {
            for (auto j = 1u; j < usedConn; j++) {
				if (j == i) continue;
                SDLNetUtils::send(
                    static_cast<NET_StreamSocket*>(conn[j]),
                    buf.data,
                    buf.size);
            }
        }
    }
    else {
        // Cliente desconectado

        Uint8 j = 0;
        while (j < maxClients &&
               clients[j] != static_cast<NET_StreamSocket*>(conn[i]))
            j++;

        assert(j < maxClients);

        std::cout << "Client " << (int)j << " disconnected!" << std::endl;

        clients[j] = nullptr;
        conn[i] = conn[usedConn - 1];
        conn[usedConn - 1] = nullptr;
        --usedConn;

        MsgWithMasterId m;
        m.type = _CLIENT_DISCONNECTED;
        m.clientId = j;
        m.masterId = whoIsTheMaster();

        for (auto k = 1u; k < usedConn; k++) 
            SDLNetUtils::serialized_send( m, static_cast<NET_StreamSocket*>(conn[k]));

    		   i--;
    		}
		  }
		}
	}

	shutdown();

}

void TCPServer::shutdown() {

	if (serv == nullptr)
		return;

	Msg m;

	m.type = _SERVER_SHUTDOWN;

	for (auto i = 1u; i < usedConn; i++) {
		SDLNetUtils::serialized_send(m,
			static_cast<NET_StreamSocket*>(conn[i]));
		NET_DestroyStreamSocket(static_cast<NET_StreamSocket*>(conn[i]));
	}

	NET_DestroyServer(serv);

	serv = nullptr;
}

int TCPServer::whoIsTheMaster() {
	// el master es siempre el de menor id
	for (auto i = 0u; i < maxClients; i++) {
		if (clients[i] != nullptr)
			return i;
	}

	return -1;
}