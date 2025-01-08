#ifndef __SERVER__
#define __SERVER__

#include <WinSock2.h>
#include <iostream>
#include <vector>
#include <map>
#include <mutex>

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
	void HandMsg(SOCKET clientSocket);
	// 处理登录功能
	void HandleCommit(SOCKET clientSocket, MsgHead& head);
	// 处理注册功能
	void HandleRegister(SOCKET clientSocket, MsgHead& head);
	// 处理好友添加功能
	void HandleFriend(SOCKET clientSocket, MsgHead& head);
	// 处理好友查询功能
	void HandleFriendQuery(SOCKET clientSocket, MsgHead& head);
	// 处理消息发送功能
	void HandleSendChat(SOCKET clientSocket, MsgHead& head);

private:
	// 接收消息
	bool RecvMessages(SOCKET clientSocket,char* buffer, MsgHead& head);
	
	// 发送消息
	bool SendMessages(SOCKET clientSocket, char* buffer, long size);

	// 创建随机数
	long GetRandomNum();

private:
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	sockaddr_in serverAddr, clientAddr;
	int clientAddrSize = sizeof(clientAddr);
	//std::vector<SOCKET> ClientSocket; // 存储客户端的socket

	MySQL* m_SqlOption;
	std::map<std::string , SOCKET> m_UsrToSocket; // 存放每个用户和客户端SOCKET的map  first - QQ  second - SOCKET
	std::mutex mtx_UsrSocket;
};

#endif __SERVER__