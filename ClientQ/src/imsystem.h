#ifndef IMSYSTEM_H
#define IMSYSTEM_H

#include <QObject>

#include "client.h"
#include "mainwindow.h"

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

    void slot_RegisterREQ();

private:
    Client* m_Client; // 客户端网络指针
    MainWindow* m_RegisterInterface; // 客户端登录界面
};

#endif // IMSYSTEM_H
