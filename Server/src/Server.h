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
	// ������Ѳ�ѯ����
	void HandleFriendQuery(SOCKET clientSocket, MsgHead& head);
	// ������Ϣ���͹���
	void HandleSendChat(SOCKET clientSocket, MsgHead& head);
	// ���������ӹ���
	void HandleFriend(SOCKET clientSocket, MsgHead& head);
	// �����������ظ�����
	void HandleFriendAck(SOCKET clientSocket, MsgHead& head);
	// ������������
	void HandleHeartREQ(SOCKET clientSocket, MsgHead& head);

private:
	// ������Ӻ��Ѻ���յ��ظ������������Ҫ���������
	void HandleAddFriendOnServer(char* tarAccount , char* usrAccount);

private:
	// ������Ϣ
	bool RecvMessages(SOCKET clientSocket,char* buffer, MsgHead& head);
	
	// ������Ϣ
	bool SendMessages(SOCKET clientSocket, char* buffer, long size);

	// ���������
	long GetRandomNum();

	// �������� -- ר�Ŵ��������Ƿ�ʱ
	void HandleHeart();

private:
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	sockaddr_in serverAddr, clientAddr;
	int clientAddrSize = sizeof(clientAddr);
	//std::vector<SOCKET> ClientSocket; // �洢�ͻ��˵�socket

	MySQL* m_SqlOption;
	std::map<std::string , SOCKET> m_UsrToSocket; // ���ÿ���û��Ϳͻ���SOCKET��map  first - QQ  second - SOCKET
	std::map<std::string, std::chrono::steady_clock::time_point> m_UsrLastHeartbeat;  // first - QQ second - ���ÿ���û��������ʱ��
	std::mutex mtx_UsrSocket;
	std::mutex mtx_heartTime;
};

#endif __SERVER__