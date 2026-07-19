#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <string>

class Server
{
private:
    SOCKET serverSocket;
    sockaddr_in serverAddress;
    int port;

    std::string readTextFile(const std::string& path);

public:
    Server(int port);

    bool initialize();

    void start();

    void stop();
};

#endif