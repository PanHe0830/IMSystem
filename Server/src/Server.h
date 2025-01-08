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
	// ��ʼ������
	bool InitNet();

public:
	// �ͻ�������
	bool ClientConnent();
private:
	// ���ն�Ӧ�����ݲ����д���
	void HandMsg(SOCKET clientSocket);
	// �����¼����
	void HandleCommit(SOCKET clientSocket, MsgHead& head);
	// ����ע�Ṧ��
	void HandleRegister(SOCKET clientSocket, MsgHead& head);
	// ���������ӹ���
	void HandleFriend(SOCKET clientSocket, MsgHead& head);
	// ������Ѳ�ѯ����
	void HandleFriendQuery(SOCKET clientSocket, MsgHead& head);
	// ������Ϣ���͹���
	void HandleSendChat(SOCKET clientSocket, MsgHead& head);

private:
	// ������Ϣ
	bool RecvMessages(SOCKET clientSocket,char* buffer, MsgHead& head);
	
	// ������Ϣ
	bool SendMessages(SOCKET clientSocket, char* buffer, long size);

	// ���������
	long GetRandomNum();

private:
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	sockaddr_in serverAddr, clientAddr;
	int clientAddrSize = sizeof(clientAddr);
	//std::vector<SOCKET> ClientSocket; // �洢�ͻ��˵�socket

	MySQL* m_SqlOption;
	std::map<std::string , SOCKET> m_UsrToSocket; // ���ÿ���û��Ϳͻ���SOCKET��map  first - QQ  second - SOCKET
	std::mutex mtx_UsrSocket;
};

#endif __SERVER__