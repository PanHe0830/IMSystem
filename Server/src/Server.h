#ifndef __SERVER__
#define __SERVER__

#include <WinSock2.h>
#include <iostream>
#include <vector>
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
	void HandMsg(int code, MsgHead& head);
	// 处理登录功能
	void HandleCommit(int clientSocket, MsgHead& head);
	// 处理注册功能
	void HandleRegister(int clientSocket, MsgHead& head);

private:
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	sockaddr_in serverAddr, clientAddr;
	int clientAddrSize = sizeof(clientAddr);
	std::vector<int> ClientSocket; // 存储客户端的socket
};

#endif __SERVER__