#include "Server.h"
#include <iostream>
#include <thread>

#pragma comment(lib,"ws2_32.lib")

Server::Server()
{
	if (!InitNet())
	{
		std::cout << "��ǰ�����ʼ��ʧ��" << std::endl;
	}
}

Server::~Server()
{
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}

bool Server::InitNet()
{
    // ��ʼ��Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Winsock initialization failed!" << std::endl;
        return false;
    }

    // ����������Socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed!" << std::endl;
        WSACleanup();
        return false;
    }

    // ���÷�������ַ�ṹ
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // �������ӵ��������κ�IP��ַ
    serverAddr.sin_port = htons(12345);       // �����˿�

    // ��Socket���˿�
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed!" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }

    // ��ʼ�����ͻ�������
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        std::cerr << "Listen failed!" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }

    std::cout << "Server is listening on port 12345..." << std::endl;

	return true;
}

bool Server::ClientConnent()
{
    // ���ܿͻ�������
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed!" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }
    
    // ����ͻ���socket
    ClientSocket.push_back(clientSocket);

    std::cout << "Client connected!" << std::endl;

    MsgHead head;
    recv(clientSocket, (char*)&head, sizeof(head), 0);

    HandMsg(head.MsgCode, head);

    //char buffer[2048];
    //recv(clientSocket , buffer , sizeof(buffer),0);
    //CRegister reg;
    //memcpy(&reg, buffer, sizeof(CRegister));


#if 0
    int recvSize;
    char recvBuffer[512];
    // ��������
    recvSize = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
    if (recvSize == SOCKET_ERROR) {
        std::cerr << "Receive failed!" << std::endl;
    }
    else {
        recvBuffer[recvSize] = '\0'; // ȷ�����յ���������'\0'��β
        std::cout << "Received message: " << recvBuffer << std::endl;
    }

    // �������ݵ��ͻ���
    const char* msg = "Hello from server!";
    if (send(clientSocket, msg, strlen(msg), 0) == SOCKET_ERROR) {
        std::cerr << "Send failed!" << std::endl;
    }
#endif
    return true;
}

void Server::HandMsg(int code , MsgHead& head)
{
    switch (code)
    {
    case CLIENT_COMMIT:
        HandleCommit(code, head);
        break;
    case CLIENT_REGISTER:
        HandleRegister(clientSocket,head);
        break;
    default:
        std::cout << "code not exist" << std::endl;
        break;
    }
}

void Server::HandleCommit(int clientSocket, MsgHead& head)
{
    std::cout << "HandleCommit" << std::endl;
}

void Server::HandleRegister(int clientSocket , MsgHead& head)
{
    std::cout << "HandleRegister" << std::endl;
    
    //head.nSize = ntohl(head.nSize);
    //if (head.nSize != sizeof(CRegister))
    //{
    //    return;
    //}
    CRegister registerData;
    int bytesRead = 0;
    while (bytesRead < head.nSize - sizeof(head)) {
        int result = recv(clientSocket, ((char*)&registerData) + bytesRead + sizeof(head), head.nSize - bytesRead - sizeof(head), 0);
        if (result <= 0) {
            perror("Failed to receive CRegister data");
            exit(EXIT_FAILURE);
        }
        bytesRead += result;
    }
    std::cout << "Received msg: " << registerData.msg << "\n";
    std::cout << "Received msgPass: " << registerData.msgPass << "\n";
}
