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

    // ��ʼ��Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Winsock initialization failed!" << std::endl;
        return -1;
    }

    // ����Socket
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed!" << std::endl;
        WSACleanup();
        return -1;
    }

    // ���÷�������ַ�ṹ
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // ��������IP��ַ
    serverAddr.sin_port = htons(12345);                    // �������Ķ˿ں�

    // ���ӵ�������
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed!" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Connected to server!" << std::endl;

    // �������ݵ�������
    const char* message = "Hello from client!";
    if (send(clientSocket, message, strlen(message), 0) == SOCKET_ERROR) {
        std::cerr << "Send failed!" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }
    std::cout << "Message sent to server: " << message << std::endl;

    // ���շ��������ص�����
    recvSize = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
    if (recvSize == SOCKET_ERROR) {
        std::cerr << "Receive failed!" << std::endl;
    }
    else {
        recvBuffer[recvSize] = '\0';  // ȷ�����յ���������'\0'��β
        std::cout << "Received from server: " << recvBuffer << std::endl;
    }

    // �ر�����
    closesocket(clientSocket);

    // ����Winsock
    WSACleanup();

	return 0;
}