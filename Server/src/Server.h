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
	// ��ʼ������
	bool InitNet();

public:
	// �ͻ�������
	bool ClientConnent();
private:
	// ���ն�Ӧ�����ݲ����д���
	void HandMsg();
	// �����¼����
	void HandleCommit(SOCKET clientSocket, MsgHead& head);
	// ����ע�Ṧ��
	void HandleRegister(SOCKET clientSocket, MsgHead& head);
	// ���������
	long GetRandomNum();

private:
	// ������Ϣ
	bool RecvMessages(SOCKET clientSocket,char* buffer, MsgHead& head);
	
	// ������Ϣ
	bool SendMessages(SOCKET clientSocket, char* buffer);

private:
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	sockaddr_in serverAddr, clientAddr;
	int clientAddrSize = sizeof(clientAddr);
	std::vector<SOCKET> ClientSocket; // �洢�ͻ��˵�socket

	MySQL* m_SqlOption;
};

#endif __SERVER__