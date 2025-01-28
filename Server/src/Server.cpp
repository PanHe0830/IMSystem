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

    std::cout << "Client connected!" << std::endl;

    std::thread threadHeart(&Server::HandleHeart , this);
    threadHeart.detach();

    std::thread thread(&Server::HandMsg, this, clientSocket);
    thread.detach();

    return true;
}

void Server::HandMsg(SOCKET clientSocket)
{
    while (1)
    {
        //MsgHead head;
        //recv(clientSocket, (char*)&head, sizeof(head), 0);

        MsgHead head;
        int receivedBytes = recv(clientSocket, reinterpret_cast<char*>(&head), sizeof(MsgHead), 0);
        if (receivedBytes <= 0)
        {
            std::cerr << "Lost connection with sender.\n";
            break;
        }

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
        case CLIENT_MESSAGE_HEART_REQ:
            HandleHeartREQ(clientSocket, head);
            break;
        case CLIENT_VIDEO_REQ:
            HandleVideoREQ(clientSocket, head);
            break;
        case CLIENT_VIDEO_ACK:
            HandleVideoACK(clientSocket, head);
            break;
        case CLIENT_VIDEO_DATA:
            HandleVideoData(clientSocket, head);
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

    SendClientFriend(clientSocket, std::string(commitReq.account));
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

void Server::HandleHeartREQ(SOCKET clientSocket, MsgHead& head)
{
    std::lock_guard<std::mutex> lock(mtx_heartTime);

    CHeartMessage_REQ msg;
    if (!RecvMessages(clientSocket, (char*)&msg, head))
    {
        std::cout << "������Ϣʧ��" << std::endl;
        return;
    }
    auto ite = m_UsrLastHeartbeat.find(std::string(msg.usrAccount));
    if (ite == m_UsrLastHeartbeat.end())
    {
        m_UsrLastHeartbeat.emplace(std::string(msg.usrAccount), std::chrono::steady_clock::now()); // û�������
    }
    else
    {
        m_UsrLastHeartbeat[std::string(msg.usrAccount)] = std::chrono::steady_clock::now(); // �о͸���ʱ��
    }
}

void Server::HandleVideoREQ(SOCKET clientSocket, MsgHead& head)
{
    CVideo_REQ msg;
    if (!RecvMessages(clientSocket, (char*)&msg, head))
    {
        std::cout << "������Ϣʧ��" << std::endl;
        return;
    }

    auto ite = m_UsrToSocket.find(msg.tarAccount);
    if (ite == m_UsrToSocket.end()) return;

    if (!SendMessages(ite->second, (char*)&msg, sizeof(msg)))
    {
        std::cout << "������Ϣʧ��" << std::endl;
        return;
    }
}

void Server::HandleVideoACK(SOCKET clientSocket, MsgHead& head)
{
    CVideo_ACK msg;
    if (!RecvMessages(clientSocket, (char*)&msg, head))
    {
        std::cout << "������Ϣʧ��" << std::endl;
        return;
    }

    auto ite = m_UsrToSocket.find(msg.tarAccount);
    if (ite == m_UsrToSocket.end()) return;

    if (!SendMessages(ite->second, (char*)&msg, sizeof(msg)))
    {
        std::cout << "������Ϣʧ��" << std::endl;
        return;
    }
}

void Server::HandleVideoData(SOCKET clientSocket, MsgHead& head)
{
    int dataSize = head.nSize - sizeof(MsgHead);
    std::vector<unsigned char> buffer(dataSize);

    int totalReceived = 0;
    while (totalReceived < dataSize)
    {
        int bytes = recv(clientSocket, reinterpret_cast<char*>(buffer.data()) + sizeof(head) + totalReceived, dataSize - totalReceived, 0);
        if (bytes <= 0)
        {
            std::cerr << "Error receiving full message.\n";
            return;
        }
        totalReceived += bytes;
    }

    CVideo_Data msg = CVideo_Data::deserialize(buffer);

    //CVideo_Data msg;
    //msg.deserialize(buffer.data(), dataSize + sizeof(MsgHead));
    
    auto ite = m_UsrToSocket.find(msg.tarAccount);
    if (ite == m_UsrToSocket.end()) return;

    std::vector<unsigned char> sendBuffer;
    sendBuffer = msg.serialize();

    int bytesSent = send(ite->second, reinterpret_cast<char*>(sendBuffer.data()), sendBuffer.size(), 0);

    if (bytesSent <= 0)
    {
        std::cerr << "Error forwarding video data.\n";
        return;
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

void Server::HandleHeart()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2)); // ���2s���һ�� �������5s��һ��

        std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
        for (auto it = m_UsrLastHeartbeat.begin(); it != m_UsrLastHeartbeat.end();)
        {
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - it->second).count();
            if (duration > 10)
            {
                SOCKET sock = m_UsrToSocket[it->first];
                closesocket(sock);

                // �������б��SOCKET�б����Ƴ����û�
                m_UsrToSocket.erase(it->first);
                it = m_UsrLastHeartbeat.erase(it);  // ɾ����ʱ�û�
            }
            else
            {
                ++it;
            }
        }
    }
}

void Server::SendClientFriend(SOCKET clientSocket, std::string usrQQ)
{
    //select mQQ from friend where nQQ = 918996674;

    std::string sql = "select mQQ from friend where nQQ = ";
    sql = sql + usrQQ;

    if (!m_SqlOption->MySqlQuery(sql.c_str()))
    {
        std::cout << "���ݿ�ִ��ʧ��" << std::endl;
        return;
    }

    auto ptr = m_SqlOption->MySqlGetResult();

    if (ptr == nullptr)
    {
        std::cout << "û�ж�Ӧ����" << std::endl;
        return;
    }

    std::vector<std::string> temp = m_SqlOption->MySqlPrintfResult(ptr);

    for (auto ite : temp)
    {
        SFriendCheck msg;
        memcpy(msg.friAccount, ite.c_str(), sizeof(msg.friAccount));

        if (!SendMessages(clientSocket, (char*)&msg, sizeof(msg)))
        {
            std::cout << "����ʧ��" << std::endl;
            return;
        }

        // ��ǰ���1s����һ����Ϣ
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
