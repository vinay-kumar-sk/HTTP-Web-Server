#include "../include/Server.h"

#include <iostream>
#include <fstream>
#include <iterator>

#pragma comment(lib,"ws2_32.lib")

using namespace std;

Server::Server(int port)
{
    this->port = port;
}

string Server::readTextFile(const string& path)
{
    ifstream file(path);

    if(!file.is_open())
    {
        return "<h1>404 - File Not Found</h1>";
    }

    return string(
        istreambuf_iterator<char>(file),
        istreambuf_iterator<char>()
    );
}

bool Server::initialize()
{
    WSADATA wsData;

    if(WSAStartup(MAKEWORD(2,2), &wsData)!=0)
    {
        return false;
    }

    serverSocket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(serverSocket==INVALID_SOCKET)
    {
        return false;
    }

    serverAddress.sin_family=AF_INET;
    serverAddress.sin_port=htons(port);
    serverAddress.sin_addr.s_addr=INADDR_ANY;

    if(bind(serverSocket,(sockaddr*)&serverAddress,sizeof(serverAddress))==SOCKET_ERROR)
    {
        return false;
    }

    if(listen(serverSocket,SOMAXCONN)==SOCKET_ERROR)
    {
        return false;
    }

    cout<<"=================================="<<endl;
    cout<<" C++ HTTP Web Server"<<endl;
    cout<<" Running on http://localhost:"<<port<<endl;
    cout<<"=================================="<<endl;

    return true;
}

void Server::start()
{
    while(true)
    {
        SOCKET clientSocket=accept(serverSocket,NULL,NULL);

        if(clientSocket==INVALID_SOCKET)
        {
            continue;
        }

        char buffer[4096];

        int received=recv(clientSocket,buffer,sizeof(buffer)-1,0);

        if(received<=0)
        {
            closesocket(clientSocket);
            continue;
        }

        buffer[received]='\0';

        string request(buffer);

        cout<<"\n=========================="<<endl;
        cout<<request<<endl;
        cout<<"=========================="<<endl;

        string file;

        if(request.find("GET / ")!=string::npos)
            file="public/index.html";

        else if(request.find("GET /about ")!=string::npos)
            file="public/about.html";

        else if(request.find("GET /contact ")!=string::npos)
            file="public/contact.html";

        else
            file="public/404.html";

        string html=readTextFile(file);

        string response=
            "HTTP/1.1 200 OK\r\n"
            "Content-Type:text/html\r\n"
            "Connection: close\r\n\r\n"
            +html;

        send(clientSocket,
             response.c_str(),
             response.length(),
             0);

        closesocket(clientSocket);
    }
}

void Server::stop()
{
    closesocket(serverSocket);
    WSACleanup();
}