#include "Server.h"
#include <iostream>
#include <thread>

#pragma comment(lib,"ws2_32.lib")

Server::Server()
{
	if (!InitNet())
	{
		std::cout << "当前网络初始化失败" << std::endl;
	}

    m_SqlOption = new MySQL();
    if (!m_SqlOption->MySqlInit())
    {
        std::cout << "数据库初始化失败" << std::endl;
        return;
    }
    if (m_SqlOption->MySqlConnent("127.0.0.1", "root", "125801ph", "IMDataBase", 3306) == nullptr)
    {
        std::cout << "数据库连接失败" << std::endl;
        return;
    }
}

Server::~Server()
{
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    if (m_SqlOption)
    {
        if (m_SqlOption->GetIsInit() == true)
        {
            m_SqlOption->MySqlColse();
        }
        delete m_SqlOption;
        m_SqlOption = nullptr;
    }
}

bool Server::InitNet()
{
    // 初始化Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Winsock initialization failed!" << std::endl;
        return false;
    }

    // 创建服务器Socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed!" << std::endl;
        WSACleanup();
        return false;
    }

    // 配置服务器地址结构
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // 允许连接到本机的任何IP地址
    serverAddr.sin_port = htons(12345);       // 监听端口

    // 绑定Socket到端口
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed!" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }

    // 开始监听客户端连接
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
    // 接受客户端连接
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed!" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }
    
    // 保存客户端socket
    ClientSocket.push_back(clientSocket);

    std::cout << "Client connected!" << std::endl;

    std::thread thread(&Server::HandMsg,this);
    thread.join();

#if 0
    int recvSize;
    char recvBuffer[512];
    // 接收数据
    recvSize = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
    if (recvSize == SOCKET_ERROR) {
        std::cerr << "Receive failed!" << std::endl;
    }
    else {
        recvBuffer[recvSize] = '\0'; // 确保接收到的数据以'\0'结尾
        std::cout << "Received message: " << recvBuffer << std::endl;
    }

    // 发送数据到客户端
    const char* msg = "Hello from server!";
    if (send(clientSocket, msg, strlen(msg), 0) == SOCKET_ERROR) {
        std::cerr << "Send failed!" << std::endl;
    }
#endif
    return true;
}

void Server::HandMsg()
{
    MsgHead head;
    recv(clientSocket, (char*)&head, sizeof(head), 0);

    switch (head.MsgCode)
    {
    case CLIENT_COMMIT:
        HandleCommit(clientSocket, head);
        break;
    case CLIENT_REGISTER:
        HandleRegister(clientSocket,head);
        break;
    default:
        std::cout << "code not exist" << std::endl;
        break;
    }
}

void Server::HandleCommit(SOCKET clientSocket, MsgHead& head)
{
    std::cout << "HandleCommit" << std::endl;
}

void Server::HandleRegister(SOCKET clientSocket , MsgHead& head)
{
    std::cout << "HandleRegister" << std::endl;

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
