#ifndef IMSYSTEM_H
#define IMSYSTEM_H

#include <QObject>

#include "client.h"
#include "mainwindow.h"
#include "register.h"

class IMSystem : public QObject
{
    Q_OBJECT
public:
    IMSystem();
    ~IMSystem();

private:
    void HandleMessage();

    void HandleCommitACK(SOCKET serverClient , MsgHead& head);

    void HandleRegisterACK(SOCKET serverClient , MsgHead& head);

private slots:
    void slot_CommitREQ(QString account , QString password);

    void slot_RegisterREQ(QString passWord);

    void slot_ShowRegisterInterface();

signals:
    void SIG_Account(QString Account);

private:
    Client* m_Client; // 客户端网络指针
    MainWindow* m_CommitInterface; // 客户端登录界面
    Register* m_RegisterInterface; // 客户端注册界面
};

#endif // IMSYSTEM_H
