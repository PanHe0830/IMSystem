#ifndef __SERVER__
#define __SERVER__

#include <WinSock2.h>
#include <iostream>
#include <vector>

#include "MySQL.h"
#include "Message.h"

class Server
{
public:
	Server();
	~Server();

private:
	// 初始化网络
	bool InitNet();

public:
	// 客户端连接
	bool ClientConnent();
private:
	// 接收对应的数据并进行处理
	void HandMsg();
	// 处理登录功能
	void HandleCommit(SOCKET clientSocket, MsgHead& head);
	// 处理注册功能
	void HandleRegister(SOCKET clientSocket, MsgHead& head);

private:
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	sockaddr_in serverAddr, clientAddr;
	int clientAddrSize = sizeof(clientAddr);
	std::vector<SOCKET> ClientSocket; // 存储客户端的socket

	MySQL* m_SqlOption;
};

#endif __SERVER__