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
	// 处理好友查询功能
	void HandleFriendQuery(SOCKET clientSocket, MsgHead& head);
	// 处理消息发送功能
	void HandleSendChat(SOCKET clientSocket, MsgHead& head);
	// 处理好友添加功能
	void HandleFriend(SOCKET clientSocket, MsgHead& head);
	// 处理好友请求回复功能
	void HandleFriendAck(SOCKET clientSocket, MsgHead& head);
	// 处理心跳请求
	void HandleHeartREQ(SOCKET clientSocket, MsgHead& head);

private:
	// 处理添加好友后接收到回复后服务器上需要处理的事情
	void HandleAddFriendOnServer(char* tarAccount , char* usrAccount);

private:
	// 接收消息
	bool RecvMessages(SOCKET clientSocket,char* buffer, MsgHead& head);
	
	// 发送消息
	bool SendMessages(SOCKET clientSocket, char* buffer, long size);

	// 创建随机数
	long GetRandomNum();

	// 监视心跳 -- 专门处理心跳是否超时
	void HandleHeart();

private:
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	sockaddr_in serverAddr, clientAddr;
	int clientAddrSize = sizeof(clientAddr);
	//std::vector<SOCKET> ClientSocket; // 存储客户端的socket

	MySQL* m_SqlOption;
	std::map<std::string , SOCKET> m_UsrToSocket; // 存放每个用户和客户端SOCKET的map  first - QQ  second - SOCKET
	std::map<std::string, std::chrono::steady_clock::time_point> m_UsrLastHeartbeat;  // first - QQ second - 存放每个用户最后心跳时间
	std::mutex mtx_UsrSocket;
	std::mutex mtx_heartTime;
};

#endif __SERVER__