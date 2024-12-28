#include <iostream>
#include <winsock2.h>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib") 

#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

int main()
{
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    char sendBuffer[512];
    char recvBuffer[512];
    int recvSize;

    // 初始化Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Winsock initialization failed!" << std::endl;
        return -1;
    }

    // 创建Socket
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed!" << std::endl;
        WSACleanup();
        return -1;
    }

    // 配置服务器地址结构
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // 服务器的IP地址
    serverAddr.sin_port = htons(12345);                    // 服务器的端口号

    // 连接到服务器
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed!" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Connected to server!" << std::endl;

    // 发送数据到服务器
    const char* message = "Hello from client!";
    if (send(clientSocket, message, strlen(message), 0) == SOCKET_ERROR) {
        std::cerr << "Send failed!" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }
    std::cout << "Message sent to server: " << message << std::endl;

    // 接收服务器返回的数据
    recvSize = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
    if (recvSize == SOCKET_ERROR) {
        std::cerr << "Receive failed!" << std::endl;
    }
    else {
        recvBuffer[recvSize] = '\0';  // 确保接收到的数据以'\0'结尾
        std::cout << "Received from server: " << recvBuffer << std::endl;
    }

    // 关闭连接
    closesocket(clientSocket);

    // 清理Winsock
    WSACleanup();

	return 0;
}