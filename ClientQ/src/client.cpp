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
    SendMessage();
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

bool Client::SendMessage()
{
    CRegister reg;
    memcpy(&reg.msg,"client register",sizeof(reg.msg));
    memcpy(&reg.msgPass,"client register password",sizeof(reg.msgPass));
    // 发送数据到服务器
    //const char* message = "Hello from client!";
    if (send(clientSocket, (char*)&reg, sizeof(reg) + 1, 0) == SOCKET_ERROR) {
        qDebug() << "Send failed!";
        closesocket(clientSocket);
        WSACleanup();
        return false;
    }
    //qDebug() << "Message sent to server: " << message;

#if 0
    // 接收服务器返回的数据
    recvSize = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
    if (recvSize == SOCKET_ERROR) {
        qDebug() << "Receive failed!";
        return false;
    }
    else {
        recvBuffer[recvSize] = '\0';  // 确保接收到的数据以'\0'结尾
        qDebug() << "Received from server: " << recvBuffer;
    }
#endif
    return true;
}
