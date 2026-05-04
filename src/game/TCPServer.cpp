#include "TCPServer.h"

#include <cassert>
#include <csignal>
#include "messages.h"

Uint16 TCPServer::shutDownTriggetPort = 0;
bool TCPServer::done = true;

TCPServer::TCPServer(Uint8 maxClients) :
    maxClients(maxClients),
    serv(nullptr),
    usedConn(0) {
    conn = new void*[maxClients + 1] {};
    clients = new NET_StreamSocket*[maxClients] {};
}

TCPServer::~TCPServer() {
    shutdown();
    delete[] conn;
    delete[] clients;
}

void TCPServer::shutdownTrigger() {
    if (done)
        return;
    std::cout << std::endl << "Shutting down  ..." << std::endl;
    done = true;
    if (shutDownTriggetPort != 0) {
        NET_Address *addr = NET_ResolveHostname("localhost");
        if (NET_WaitUntilResolved(addr, 1000) != NET_SUCCESS) {
            SDLNetUtils::print_SDLNet_error_and_exit(1);
        }
        NET_StreamSocket *conn = NET_CreateClient(addr, shutDownTriggetPort);
        if (NET_WaitUntilConnected(conn, 1000) != NET_SUCCESS) {
            SDLNetUtils::print_SDLNet_error_and_exit(1);
        }
        NET_UnrefAddress(addr);
        NET_DestroyStreamSocket(conn);
    }
}

void TCPServer::handlerSIGINT(int) {
    // Implementación de handler de señal si es necesario
}

void TCPServer::shutdown() {
    // Implementación de apagado del servidor
}

int TCPServer::whoIsTheMaster() {
    // Implementación para calcular el id del máster (mínimo id conectado)
    return 0;
}

bool TCPServer::connect(const Uint16 port) {
    // Implementación de conexión del servidor
    return true;
}

void TCPServer::listen() {
    // Implementación del bucle principal del servidor
}
