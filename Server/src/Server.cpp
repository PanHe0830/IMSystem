#include "Server.h"
#include <iostream>
#include <thread>
#include <random>
#include <string>

#pragma comment(lib,"ws2_32.lib")

Server::Server()
{
    m_UsrToSocket.clear();
	if (!InitNet())
	{
		std::cout << "��ǰ�����ʼ��ʧ��" << std::endl;
	}

    m_SqlOption = new MySQL();
    if (!m_SqlOption->MySqlInit())
    {
        std::cout << "���ݿ��ʼ��ʧ��" << std::endl;
        return;
    }
    if (m_SqlOption->MySqlConnent("127.0.0.1", "root", "125801ph", "IMDataBase", 3306) == nullptr)
    {
        std::cout << "���ݿ�����ʧ��" << std::endl;
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
    //ClientSocket.push_back(clientSocket);

    std::cout << "Client connected!" << std::endl;

    std::thread thread(&Server::HandMsg, this, clientSocket);
    thread.detach();

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

void Server::HandMsg(SOCKET clientSocket)
{
    while (1)
    {
        MsgHead head;
        recv(clientSocket, (char*)&head, sizeof(head), 0);

        switch (head.MsgCode)
        {
        case CLIENT_COMMIT_REQ:
            HandleCommit(clientSocket, head);
            break;
        case CLIENT_REGISTER_REQ:
            HandleRegister(clientSocket, head);
            break;
        case CLIENT_FRIEND_REQ:
            HandleFriend(clientSocket, head);
            break;
        case CLIENT_FRIEND_QUERY_REQ:
            HandleFriendQuery(clientSocket , head);
            break;
        case CLIENT_MESSAGE_CHAT:
            HandleSendChat(clientSocket,head);
            break;
        case CLIENT_FRIEND_ACK:
            HandleFriendAck(clientSocket, head);
            break;
        }
    }
}

void Server::HandleCommit(SOCKET clientSocket, MsgHead& head)
{
    std::cout << "HandleCommit" << std::endl;

    CCommit_REQ commitReq;
    if (!RecvMessages(clientSocket, (char*)&commitReq, head))
    {
        //std::cout << "������Ϣʧ��" << std::endl;
        return;
    }
    std::cout << commitReq.account << std::endl;
    std::cout << commitReq.password << std::endl;

    mtx_UsrSocket.lock();
    m_UsrToSocket.emplace(std::string(commitReq.account), clientSocket);
    mtx_UsrSocket.unlock();

    std::string sqlStr = "select * from account where numAccount = ";
    sqlStr = sqlStr + std::string(commitReq.account) + " and numPassWord = " + std::string(commitReq.password);
    m_SqlOption->MySqlQuery(sqlStr.c_str());


    CCommit_ACK commmitAck;
    if (m_SqlOption->MySqlGetResult() == nullptr)
    {
        commmitAck.flag = CLIENT_COMMIT_FAILED;
    }
    else
    {
        commmitAck.flag = CLIENT_COMMIT_SUCCESS;
    }

    if (!SendMessages(clientSocket, (char*)&commmitAck, sizeof(commmitAck)))
    {
        std::cout << "������Ϣʧ��" << std::endl;
        return;
    }
}

void Server::HandleRegister(SOCKET clientSocket , MsgHead& head)
{
    std::cout << "HandleRegister" << std::endl;

    CRegister_REQ registerData;
    if (!RecvMessages(clientSocket, (char*)&registerData, head))
    {
        std::cout << "������Ϣʧ��" << std::endl;
        return;
    }
   
    // 
    long account = 0;
    do
    {
        account = GetRandomNum();
        std::string sqlStr = "select * from account where numAccount = ";
        std::string accountstr = std::to_string(account);
        sqlStr = sqlStr + accountstr + ";";
        m_SqlOption->MySqlQuery(sqlStr.c_str());
    } while (m_SqlOption->MySqlGetResult() == nullptr);

    std::string insertSql = "insert into account (numAccount , numPassWord , numName) values(";
    std::string newStr =    insertSql + std::to_string(account) + "," + std::string(registerData.passWord) + "," 
                            + "\""  + std::string(registerData.nName) + "\"" + ");";

    if (!m_SqlOption->MySqlQuery(newStr.c_str()))
    {
        std::cout << "sql failed" << std::endl;
    }
    
    CRegister_ACK registerAck;
    memcpy(&registerAck.Account, std::to_string(account).c_str(),sizeof(registerAck.Account));

    if (!SendMessages(clientSocket, (char*)&registerAck , sizeof(registerAck)))
    {
        std::cout << "������Ϣʧ��" << std::endl;
        return;
    }
}

void Server::HandleFriendQuery(SOCKET clientSocket, MsgHead& head)
{
    std::cout << "HandleFriendQuery" << std::endl;
    CFriendQuery_REQ friendQueryReq;

    if (!RecvMessages(clientSocket, (char*)&friendQueryReq, head))
    {
        std::cout << "������Ϣʧ��" << std::endl;
        return;
    }

    std::string sqlStr = "select * from account where numAccount = ";
    sqlStr = sqlStr + std::string(friendQueryReq.tarAccount);
    m_SqlOption->MySqlQuery(sqlStr.c_str());


    CFriendQuery_ACK friendQueryACK;    
    if (m_SqlOption->MySqlGetResult() == nullptr)
    {
        friendQueryACK.flag = CLIENT_FRIEND_QUERY_NOEXIST;
    }
    else
    {
        friendQueryACK.flag = CLIENT_FRIEND_QUERY_EXIST;
    }

    if (!SendMessages(clientSocket, (char*)&friendQueryACK, sizeof(friendQueryACK)))
    {
        std::cout << "������Ϣʧ��" << std::endl;
        return;
    }
}

void Server::HandleSendChat(SOCKET clientSocket, MsgHead& head)
{
    std::cout << "HandleSendChat" << std::endl;
    CSendMessage msg;
    if (!RecvMessages(clientSocket, (char*)&msg, head))
    {
        std::cout << "������Ϣʧ��" << std::endl;
        return;
    }

    std::string tarAccount = std::string(msg.tarAccount);

    auto ite = m_UsrToSocket.find(tarAccount);

    if (ite == m_UsrToSocket.end()) return;

    // ���ҵ���ӦQQ�ͻ��˵�SOCKET������Ϣ
    if (!SendMessages(ite->second, (char*)&msg, sizeof(msg)))
    {
        std::cout << "������Ϣʧ��" << std::endl;
        return;
    }
}

void Server::HandleFriend(SOCKET clientSocket, MsgHead& head)
{
    std::cout << "HandleFriend" << std::endl;

    CFriend_REQ frireq;
    if (!RecvMessages(clientSocket, (char*)&frireq, head))
    {
        std::cout << "������Ϣʧ��" << std::endl;
        return;
    }

    std::string tarAccount = std::string(frireq.tarAccount);

    auto ite = m_UsrToSocket.find(tarAccount);

    if (ite == m_UsrToSocket.end()) return;

    // ���ҵ���ӦQQ�ͻ��˵�SOCKET������Ϣ
    if (!SendMessages(ite->second, (char*)&frireq, sizeof(frireq)))
    {
        std::cout << "������Ϣʧ��" << std::endl;
        return;
    }
}

void Server::HandleFriendAck(SOCKET clientSocket, MsgHead& head)
{
    CFriend_ACK msg;
    if (!RecvMessages(clientSocket, (char*)&msg, head))
    {
        std::cout << "������Ϣʧ��" << std::endl;
        return;
    }

    std::string tarAccount = std::string(msg.tarAccount);

    auto ite = m_UsrToSocket.find(tarAccount);

    if (ite == m_UsrToSocket.end()) return;

    if (!SendMessages(ite->second, (char*)&msg, sizeof(msg)))
    {
        std::cout << "������Ϣʧ��" << std::endl;
        return;
    }

    switch (msg.flag)
    {
    case CLIENT_FRIEND_SUCCESS: // ͬ���Ϊ����
        HandleAddFriendOnServer(msg.tarAccount, msg.sourceAccount);
        break;
    case CLIENT_FRIEND_FAILED: // ��ͬ���Ϊ����

        break;
    case CLIENT_FRIEND_DEFAULT: // ��Ϣ�����

        break;
    }
}

void Server::HandleAddFriendOnServer(char* tarAccount, char* usrAccount)
{
    std::cout << "HandleAddFriendOnServer" << std::endl;

    /** �������� ������˫��� */
    std::string insertSql = "insert into friend (nQQ , mQQ) values(";
    insertSql = insertSql + std::string(tarAccount) + "," + std::string(usrAccount) + ")";
    if (!m_SqlOption->MySqlQuery(insertSql.c_str()))
    {
        std::cout << "����ʧ��" << std::endl;
        return;
    }

    std::string insertSql2 = "insert into friend (nQQ , mQQ) values(";
    insertSql2 = insertSql2 + std::string(usrAccount) + "," + std::string(tarAccount) + ")";
    if (!m_SqlOption->MySqlQuery(insertSql2.c_str()))
    {
        std::cout << "����ʧ��" << std::endl;
        return;
    }
}

bool Server::RecvMessages(SOCKET clientSocket , char* buffer , MsgHead& head)
{
    std::cout << sizeof(head);
    int bytesRead = 0;
    while (bytesRead < head.nSize - sizeof(head)) {
        int result = recv(clientSocket, buffer + bytesRead + sizeof(head), head.nSize - bytesRead - sizeof(head), 0);
        if (result <= 0) {
            perror("Failed to receive CRegister data");
            return false;
        }
        bytesRead += result;
    }
    return true;
}

bool Server::SendMessages(SOCKET clientSocket, char* buffer , long size)
{
    int sendbytes = 0;
    while (sendbytes < size)
    {
        int senddatas = send(clientSocket, buffer, size, 0);
        if (senddatas < 0)
        {
            return false;
        }
        sendbytes += senddatas;
    }
    return true;
}

long Server::GetRandomNum()
{
    // ����һ�����������
    std::random_device rd;
    std::mt19937 gen(rd());

    // �����������Χ������9λ���������
    std::uniform_int_distribution<> dist(100000000, 999999999);

    // ����һ��9λ�������
    return dist(gen);
}