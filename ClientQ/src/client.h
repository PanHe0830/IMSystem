#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <WS2tcpip.h>

class Client
{
public:
    Client();

    ~Client();

private:
    // 初始化socket
    bool InitNet();

    //
    bool SendMessage();

private:
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    char sendBuffer[512];
    char recvBuffer[512];
    int recvSize;
};

#endif // CLIENT_H
