#include "imsystem.h"
#include "Message.h"

#include <QDebug>

IMSystem::IMSystem()
{
    m_Client = new Client();
    m_RegisterInterface = new MainWindow();
    m_RegisterInterface->show();

    connect(m_RegisterInterface,&MainWindow::SIG_SendCommitREQ , this , &IMSystem::slot_CommitREQ);
    connect(m_RegisterInterface,&MainWindow::SIG_SendRegisterREQ , this , &IMSystem::slot_RegisterREQ);
}

IMSystem::~IMSystem()
{
    if(m_Client)
    {
        delete m_Client;
        m_Client = nullptr;
    }
    if(m_RegisterInterface)
    {
        delete m_RegisterInterface;
        m_RegisterInterface = nullptr;
    }
}

void IMSystem::slot_CommitREQ(QString account, QString password)
{
    qDebug() << "slot_CommitREQ";

    CCommit_REQ commit;
    memcpy(&commit.account , account.toStdString().c_str() , sizeof(commit.account));
    memcpy(&commit.password , password.toStdString().c_str() , sizeof(commit.password));

    // 发送消息
    if(!m_Client->client_SendMessage((char*)&commit))
    {
        qDebug() << "发送失败";
    }
}

void IMSystem::slot_RegisterREQ()
{
    qDebug() << "slot_RegisterREQ";
    CRegister_REQ reg;
    memcpy(&reg.passWord , "123456789" , sizeof(reg.passWord));

    if(!m_Client->client_SendMessage((char*)&reg))
    {
        qDebug() << "发送成功";
    }
}
