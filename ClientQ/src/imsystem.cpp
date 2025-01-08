#include "imsystem.h"
#include "Message.h"

#include <QDebug>
#include <thread>
#include <qobjectdefs.h>
#include <QMessageBox>
#include <QApplication>

IMSystem::IMSystem()
{
    m_Client = new Client();
    m_CommitInterface = new MainWindow();
    m_CommitInterface->show();

    connect(m_CommitInterface,&MainWindow::SIG_SendCommitREQ , this , &IMSystem::slot_CommitREQ);
    connect(m_CommitInterface,&MainWindow::SIG_SendRegisterREQ , this , &IMSystem::slot_ShowRegisterInterface);
    connect(this,&IMSystem::SIG_Account , m_CommitInterface , &MainWindow::slot_setAccount);

    m_RegisterInterface = new Register();
    connect(m_RegisterInterface , &Register::SIG_NewPassWord , this , &IMSystem::slot_RegisterREQ);

    m_UsrInterface = new UsrInterface();
    QString name = m_CommitInterface->GetUsrName();
    m_UsrInterface->setUsrName(name);
    m_UsrInterface->close();

    connect(m_UsrInterface , &UsrInterface::SIG_AddFriendREQ , this , &IMSystem::slot_FriendREQ);

    std::thread thread(&IMSystem::HandleMessage , this , m_Client->clientSocket);
    thread.detach();
}

IMSystem::~IMSystem()
{
    if(m_Client)
    {
        delete m_Client;
        m_Client = nullptr;
    }
    if(m_CommitInterface)
    {
        delete m_CommitInterface;
        m_CommitInterface = nullptr;
    }
}

void IMSystem::HandleMessage(SOCKET clientSocket)
{
    while (1)
    {
        MsgHead head;
        recv(clientSocket, (char*)&head, sizeof(head), 0);

        switch (head.MsgCode)
        {
        case CLIENT_COMMIT_ACK:
            HandleCommitACK(clientSocket , head);
            break;
        case CLIENT_REGISTER_ACK:
            HandleRegisterACK(clientSocket , head);
            break;
        case CLIENT_FRIEND_ACK:
            HandleFriendACK(clientSocket , head);
            break;
        case CLIENT_FRIEND_QUERY_ACK:
            HandleFriendQueryACK(clientSocket , head);
            break;
        }
    }
}

void IMSystem::HandleCommitACK(SOCKET serverClient, MsgHead &head)
{
    qDebug() << "HandleCommitACK";

    CCommit_ACK commAck;

    if(!m_Client->client_RecvMessage((char*)&commAck,head))
    {
        qDebug() << "HandleCommitACK 接收失败";

        return;
    }

    switch(commAck.flag)
    {
    case CLIENT_COMMIT_SUCCESS:
        // 隐藏登陆界面
        QMetaObject::invokeMethod(m_CommitInterface , "close" ,Qt::QueuedConnection);
        // 登录成功显示界面
        QMetaObject::invokeMethod(m_UsrInterface , "show" ,Qt::QueuedConnection);
        break;
    case CLIENT_COMMIT_FAILED:
        // 登录失败提示用户
        break;
    case CLIENT_COMMIT_DEFAULT:
        // 接收到错误消息
        break;
    }
}

void IMSystem::HandleRegisterACK(SOCKET serverClient, MsgHead &head)
{
    qDebug() << "HandleRegisterACK";

    CRegister_ACK regAck;
    if(!m_Client->client_RecvMessage((char*)&regAck,head))
    {
        qDebug() << "HandleRegisterACK 接收失败";
    }

    qDebug() << "HandleRegisterACK 接收成功";
    qDebug() << regAck.Account;

    emit SIG_Account(QString(regAck.Account));
}

void IMSystem::HandleFriendACK(SOCKET serverClient, MsgHead &head)
{
    CFriend_ACK friAck;
    if(!m_Client->client_RecvMessage((char*)&friAck,head))
    {
        qDebug() << "HandleFriendACK 接收失败";
    }

    switch(friAck.flag)
    {
    case CLIENT_FRIEND_SUCCESS:
        // 同意添加
        break;
    case CLIENT_FRIEND_FAILED:
        // 不同意添加
        break;
    case CLIENT_FRIEND_DEFAULT:
        // 消息码错误
        break;
    }
}

void IMSystem::HandleFriendQueryACK(SOCKET serverClient, MsgHead &head)
{
    CFriendQuery_ACK friQueryAck;
    if(!m_Client->client_RecvMessage((char*)&friQueryAck,head))
    {
        qDebug() << "HandleFriendACK 接收失败";
    }

    switch(friQueryAck.flag)
    {
    case CLIENT_FRIEND_QUERY_EXIST:
        // 存在用户
        QMetaObject::invokeMethod(QApplication::instance(), []() {
            QMessageBox::information(nullptr, "提示", "用户存在");
        });
        break;
    case CLIENT_FRIEND_QUERY_NOEXIST:
        // 不存在用户
        QMetaObject::invokeMethod(QApplication::instance(), []() {
            QMessageBox::information(nullptr, "提示", "用户不存在");
        });
        break;
    case CLIENT_FRIEND_QUERY_DEFAULT:
        // 消息码错误
        QMetaObject::invokeMethod(QApplication::instance(), []() {
            QMessageBox::information(nullptr, "提示", "消息码错误");
        });
        break;
    }
}

void IMSystem::slot_CommitREQ(QString account, QString password)
{
    qDebug() << "slot_CommitREQ";

    CCommit_REQ commit;
    memcpy(&commit.account , account.toStdString().c_str() , sizeof(commit.account));
    memcpy(&commit.password , password.toStdString().c_str() , sizeof(commit.password));

    // 发送消息
    if(!m_Client->client_SendMessage((char*)&commit , sizeof(commit)))
    {
        qDebug() << "发送失败";
    }
}

void IMSystem::slot_RegisterREQ(QString passWord , QString nName)
{
    qDebug() << "slot_RegisterREQ";
    CRegister_REQ reg;
    memcpy(&reg.passWord , passWord.toStdString().c_str() , sizeof(reg.passWord));
    memcpy(&reg.nName , nName.toStdString().c_str(),sizeof(reg.nName));

    qDebug() << sizeof(reg);
    if(!m_Client->client_SendMessage((char*)&reg,sizeof(reg)))
    {
        qDebug() << "发送失败";
        m_RegisterInterface->close();
    }
}

void IMSystem::slot_ShowRegisterInterface()
{
    m_RegisterInterface->show();
}

void IMSystem::slot_ShowUsrInformation()
{
    //显示用户信息
    qDebug() << "显示用户个人信息界面";
}

void IMSystem::slot_FriendREQ(QString tarAccount)
{
    CFriendQuery_REQ friQueryREQ;
    memcpy(friQueryREQ.tarAccount , tarAccount.toStdString().c_str(),sizeof(friQueryREQ.tarAccount));

    if(!m_Client->client_SendMessage((char*)&friQueryREQ,sizeof(friQueryREQ)))
    {
        qDebug() << "发送失败";
    }
}
