#include "client.h"
#include <QDebug>
#include "Message.h"

Client::Client()
{
    if(!InitNet())
    {
        qDebug() << "Net init error";
        return;
    }
}

Client::~Client()
{
    // 关闭连接
    closesocket(clientSocket);

    // 清理Winsock
    WSACleanup();
}

bool Client::InitNet()
{
    // 初始化Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        qDebug() << "Winsock initialization failed!";
        return false;
    }

    // 创建Socket
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        qDebug() << "Socket creation failed!";
        WSACleanup();
        return false;
    }

    // 配置服务器地址结构
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // 服务器的IP地址
    serverAddr.sin_port = htons(12345);                    // 服务器的端口号

    // 连接到服务器
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        qDebug() << "Connection failed!";
        closesocket(clientSocket);
        WSACleanup();
        return false;
    }

    qDebug() << "Connected to server!";
    return true;
}

bool Client::client_SendMessage(char* buffer , long size)
{
    // 发送数据到服务器
    qDebug() << size;
    int sendbytes = 0;
    while(sendbytes < size)
    {
        int senddatas = send(clientSocket, buffer, size, 0);
        if(senddatas < 0)
        {
            qDebug() << "Send failed!";
            closesocket(clientSocket);
            WSACleanup();
            return false;
        }
        sendbytes += senddatas;
    }
    return true;
}

bool Client::client_RecvMessage(char* buffer , MsgHead& head)
{
    int bytesRead = 0;
    while (bytesRead < head.nSize - sizeof(head)) {
        int result = recv(clientSocket, buffer + bytesRead + sizeof(head), head.nSize - bytesRead - sizeof(head), 0);
        if (result <= 0) {
            perror("Failed to receive CRegister data");
            return false;
        }
        bytesRead += result;
    }
    return true;
}
