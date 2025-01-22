#include "imsystem.h"
#include "Message.h"

#include <QDebug>
#include <thread>
#include <qobjectdefs.h>
#include <QMessageBox>
#include <QApplication>
#include <QTimer>
#include "imsystemopencv.h"

IMSystem::IMSystem()
{
    // ============================================
    //IMSystemOpenCV* temp = new IMSystemOpenCV();

    //return;
    // ============================================

    m_Client = new Client();
    m_CommitInterface = new MainWindow();
    m_CommitInterface->show();

    connect(m_CommitInterface,&MainWindow::SIG_SendCommitREQ , this , &IMSystem::slot_CommitREQ);
    connect(m_CommitInterface,&MainWindow::SIG_SendRegisterREQ , this , &IMSystem::slot_ShowRegisterInterface);
    connect(this,&IMSystem::SIG_Account , m_CommitInterface , &MainWindow::slot_setAccount);

    m_RegisterInterface = new Register();
    connect(m_RegisterInterface , &Register::SIG_NewPassWord , this , &IMSystem::slot_RegisterREQ);

    m_UsrInterface = new UsrInterface();
    m_UsrInterface->close();

    connect(m_UsrInterface , &UsrInterface::SIG_AddFriendREQ , this , &IMSystem::slot_FriendREQ);
    connect(m_UsrInterface, &UsrInterface::SIG_SendMessage,this,&IMSystem::slot_sendMessage);
    connect(m_UsrInterface , &UsrInterface::SIG_AddFriend , this , &IMSystem::slot_FriendRequest);
    connect(this,&IMSystem::SIG_FriendREQ,m_UsrInterface,&UsrInterface::slot_showFriendREQ);
    connect(m_UsrInterface , &UsrInterface::SIG_FriendAgree , this , &IMSystem::slot_FriendAgree);
    connect(this,&IMSystem::SIG_FriendACK,m_UsrInterface,&UsrInterface::slot_showFriendACK);
    connect(this,&IMSystem::SIG_ShowChatMessage,m_UsrInterface,&UsrInterface::slot_recvChatMessage);
    connect(this,&IMSystem::SIG_AddFriend , m_UsrInterface , &UsrInterface::slot_addFriend);
    connect(m_UsrInterface,&UsrInterface::SIG_usrInterFaceSendVideoREQ , this , &IMSystem::slot_sendVideoREQ);
    connect(this,&IMSystem::SIG_VideoREQ,m_UsrInterface,&UsrInterface::slot_interfaceVideoREQ);
    connect(m_UsrInterface,&UsrInterface::SIG_interfaceVideoACK,this , &IMSystem::slot_sendVideoACK);

    m_heartTimer = new QTimer();
    connect(m_heartTimer , &QTimer::timeout , this , &IMSystem::slot_sendHeart);

    connect(this , &IMSystem::SIG_MasterTimeOut , this , &IMSystem::slot_startQTimer);

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

    if(m_heartTimer)
    {
        killTimer(m_heartTimer->timerId());
        delete m_heartTimer ; m_heartTimer = nullptr;
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
        case CLIENT_FRIEND_REQ:
            HandleFriendREQ(clientSocket , head);
            break;
        case CLIENT_MESSAGE_CHAT:
            HandleChatMessage(clientSocket,head);
            break;
        case SERVER_FRIEND_CHECK:
            HandleFriendCheck(clientSocket,head);
            break;
        case CLIENT_VIDEO_REQ:
            HandleVideoREQ(clientSocket,head);
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
        // 开启发送心跳包计时器
        emit SIG_MasterTimeOut();
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
    CFriend_ACK msg;
    if(!m_Client->client_RecvMessage((char*)&msg,head))
    {
        qDebug() << "HandleFriendACK 接收失败";
    }

    switch(msg.flag)
    {
    case CLIENT_FRIEND_SUCCESS:
        // 同意添加SIG_FriendAgree
        emit SIG_FriendACK(1 , QString(msg.sourceAccount));
        break;
    case CLIENT_FRIEND_FAILED:
        // 不同意添加
        emit SIG_FriendACK(0, QString(msg.sourceAccount));
        break;
    case CLIENT_FRIEND_DEFAULT:
        // 消息码错误
        emit SIG_FriendACK(-1, QString(msg.sourceAccount));
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

void IMSystem::HandleFriendREQ(SOCKET serverClient, MsgHead &head)
{
    CFriend_REQ msg;
    if (!m_Client->client_RecvMessage((char*)&msg, head))
    {
        qDebug() << "HandleFriendREQ 接收失败";
        return;
    }

    emit SIG_FriendREQ(QString(msg.sourceAccount) , QString(msg.tarAccount));
}

void IMSystem::HandleChatMessage(SOCKET serverClient, MsgHead &head)
{
    CSendMessage msg;
    if (!m_Client->client_RecvMessage((char*)&msg, head))
    {
        qDebug() << "HandleFriendREQ 接收失败";
        return;
    }

    emit SIG_ShowChatMessage( QString(msg.usrAccount) , QString(msg.message));
}

void IMSystem::HandleFriendCheck(SOCKET serverClient, MsgHead &head)
{
    SFriendCheck msg;
    if (!m_Client->client_RecvMessage((char*)&msg, head))
    {
        qDebug() << "HandleFriendREQ 接收失败";
        return;
    }

    // 让界面添加上好友
    emit SIG_AddFriend(QString(msg.friAccount));
}

void IMSystem::HandleVideoREQ(SOCKET serverClient, MsgHead &head)
{
    CVideo_REQ msg;
    if (!m_Client->client_RecvMessage((char*)&msg, head))
    {
        qDebug() << "HandleFriendREQ 接收失败";
        return;
    }

    emit SIG_VideoREQ(msg.tarAccount,msg.usrAccount);
}

void IMSystem::slot_CommitREQ(QString account, QString password)
{
    qDebug() << "slot_CommitREQ";

    CCommit_REQ commit;
    memcpy(&commit.account , account.toStdString().c_str() , sizeof(commit.account));
    memcpy(&commit.password , password.toStdString().c_str() , sizeof(commit.password));

    QString name = m_CommitInterface->GetUsrName();
    m_UsrInterface->setUsrName(name);

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

void IMSystem::slot_sendMessage(QString message , QString usrAccount , QString tarAccount)
{
    CSendMessage msg;
    memcpy(msg.usrAccount , usrAccount.toStdString().c_str() , sizeof(msg.usrAccount));
    memcpy(msg.tarAccount , tarAccount.toStdString().c_str() , sizeof(msg.tarAccount));
    memcpy(msg.message , message.toStdString().c_str(),sizeof(msg.message));

    if(!m_Client->client_SendMessage((char*)&msg,sizeof(msg)))
    {
        qDebug() << "发送失败";
    }
}

void IMSystem::slot_FriendRequest(QString account , QString usrAccount)
{
    CFriend_REQ msg;
    memcpy(msg.tarAccount , account.toStdString().c_str(),sizeof(msg.tarAccount));
    memcpy(msg.sourceAccount , usrAccount.toStdString().c_str(),sizeof(msg.sourceAccount));

    if(!m_Client->client_SendMessage((char*)&msg,sizeof(msg)))
    {
        qDebug() << "发送失败";
    }
}

void IMSystem::slot_FriendAgree(QString tarAccount, QString sourceAccount, int flag)
{
    CFriend_ACK msg;
    memcpy(msg.tarAccount , sourceAccount.toStdString().c_str(),sizeof(msg.sourceAccount));
    memcpy(msg.sourceAccount , tarAccount.toStdString().c_str(),sizeof(msg.tarAccount));
    msg.flag = flag;

    if(!m_Client->client_SendMessage((char*)&msg,sizeof(msg)))
    {
        qDebug() << "发送失败";
    }
}

void IMSystem::slot_sendHeart()
{
    QString account = m_CommitInterface->GetUsrName();
    CHeartMessage_REQ msg;
    memcpy(msg.usrAccount , account.toStdString().c_str(),sizeof(msg.usrAccount));

    if(!m_Client->client_SendMessage((char*)&msg,sizeof(msg)))
    {
        qDebug() << "发送失败";
    }
}

void IMSystem::slot_startQTimer()
{
    m_heartTimer->start(5000);// 每5s发送一次心跳
}

void IMSystem::slot_sendVideoREQ(QString usrQQ , QString tarAccount)
{
    CVideo_REQ msg;
    memcpy(msg.usrAccount,usrQQ.toStdString().c_str(),sizeof(msg.usrAccount));
    memcpy(msg.tarAccount,tarAccount.toStdString().c_str(),sizeof(msg.tarAccount));

    if(!m_Client->client_SendMessage((char*)&msg,sizeof(msg)))
    {
        qDebug() << "发送失败";
    }
}

void IMSystem::slot_sendVideoACK(bool bflag)
{
    CVideo_ACK msg;
    if(bflag)
    {
        msg.flag = CLIENT_VIDEO_SUCCESS;
    }
    else
    {
        msg.flag = CLIENT_VIDEO_FAILED;
    }

    if(!m_Client->client_SendMessage((char*)&msg,sizeof(msg)))
    {
        qDebug() << "发送失败";
    }
}
