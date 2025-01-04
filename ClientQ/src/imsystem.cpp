#include "imsystem.h"
#include "Message.h"

#include <QDebug>
#include <thread>
#include <qobjectdefs.h>

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

    m_usrInterface = new UserInterface();
    m_usrInterface->close();
    connect(m_usrInterface,&UserInterface::SIG_clicked,this,&IMSystem::slot_ShowUsrInformation);

    std::thread thread(&IMSystem::HandleMessage,this);
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

void IMSystem::HandleMessage()
{
    while (1)
    {
        MsgHead head;
        recv(m_Client->clientSocket, (char*)&head, sizeof(head), 0);

        switch (head.MsgCode)
        {
        case CLIENT_COMMIT_ACK:
            HandleCommitACK(m_Client->clientSocket , head);
            break;
        case CLIENT_REGISTER_ACK:
            HandleRegisterACK(m_Client->clientSocket , head);
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
         // 登录成功显示界面
        QMetaObject::invokeMethod(m_usrInterface , "show" ,Qt::QueuedConnection);
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

void IMSystem::slot_RegisterREQ(QString passWord)
{
    qDebug() << "slot_RegisterREQ";
    CRegister_REQ reg;
    memcpy(&reg.passWord , passWord.toStdString().c_str() , sizeof(reg.passWord));

    qDebug() << sizeof(reg);
    if(!m_Client->client_SendMessage((char*)&reg,sizeof(reg)))
    {
        qDebug() << "发送成功";
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
