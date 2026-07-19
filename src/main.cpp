#include "../include/Server.h"

#include <iostream>

using namespace std;

int main()
{
    Server server(8080);

    if (!server.initialize())
    {
        cout << "Server initialization failed!" << endl;
        return 1;
    }

    server.start();

    server.stop();

    return 0;
}