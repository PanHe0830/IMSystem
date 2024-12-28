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
	// ��ʼ������
	bool InitNet();

public:
	// �ͻ�������
	bool ClientConnent();
private:
	// ���ն�Ӧ�����ݲ����д���
	void HandMsg(int code, MsgHead& head);
	// �����¼����
	void HandleCommit(int clientSocket, MsgHead& head);
	// ����ע�Ṧ��
	void HandleRegister(int clientSocket, MsgHead& head);

private:
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	sockaddr_in serverAddr, clientAddr;
	int clientAddrSize = sizeof(clientAddr);
	std::vector<int> ClientSocket; // �洢�ͻ��˵�socket
};

#endif __SERVER__