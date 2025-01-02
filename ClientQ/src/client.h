#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <WS2tcpip.h>
#include "Message.h"

class Client
{
public:
    Client();

    ~Client();

private:
    // 初始化socket
    bool InitNet();

public:
    bool client_SendMessage(char* buffer  , long size);

    bool client_RecvMessage(char* buffer , MsgHead& head);

public:
    SOCKET clientSocket;

private:
    WSADATA wsaData;
    sockaddr_in serverAddr;
    char sendBuffer[512];
    char recvBuffer[512];
    int recvSize;
};

#endif // CLIENT_H
