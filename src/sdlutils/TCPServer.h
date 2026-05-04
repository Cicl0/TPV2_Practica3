#pragma once

#include <cstddef>
#include "../sdlutils/SDLNetUtils.h"

class TCPServer {
public:
    TCPServer(Uint8 maxClients);
    virtual ~TCPServer();
    bool connect(const Uint16 port);
    void listen();

private:
    static bool done;
    static Uint16 shutDownTriggetPort;
    static void shutdownTrigger();
    static void handlerSIGINT(int);

    void shutdown();
    int whoIsTheMaster();

    Uint8 maxClients;
    NET_Server *serv;
    void **conn;
    std::size_t usedConn;
    NET_StreamSocket **clients;
};
