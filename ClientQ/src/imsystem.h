#ifndef IMSYSTEM_H
#define IMSYSTEM_H

#include <QObject>

#include "client.h"
#include "mainwindow.h"
#include "register.h"
#include "usrinterface.h"

class QTimer;

class IMSystem : public QObject
{
    Q_OBJECT
public:
    IMSystem();
    ~IMSystem();

private:
    void HandleMessage(SOCKET clientSocket);

    void HandleCommitACK(SOCKET serverClient , MsgHead& head);

    void HandleRegisterACK(SOCKET serverClient , MsgHead& head);

    void HandleFriendACK(SOCKET serverClient , MsgHead& head);

    void HandleFriendQueryACK(SOCKET serverClient , MsgHead& head);

    void HandleFriendREQ(SOCKET serverClient , MsgHead& head);

    void HandleChatMessage(SOCKET serverClient , MsgHead& head);

    void HandleFriendCheck(SOCKET serverClient , MsgHead& head);

private slots:
    void slot_CommitREQ(QString account , QString password);

    void slot_RegisterREQ(QString passWord , QString nName);

    void slot_ShowRegisterInterface();

    void slot_ShowUsrInformation();

    void slot_FriendREQ(QString tarAccount);

    void slot_sendMessage(QString message,QString usrAccount , QString tarAccount);

    void slot_FriendRequest(QString account , QString usrAccount);

    void slot_FriendAgree(QString tarAccount , QString sourceAccount , int flag);

    void slot_sendHeart();

signals:
    void SIG_Account(QString Account);

    void SIG_FriendREQ(QString tarAccount , QString sourceAccount);

    void SIG_FriendACK(int flag , QString tarAccount);

    void SIG_ShowChatMessage(QString account , QString message);

    void SIG_AddFriend(QString friAccount);

private:
    Client* m_Client; // 客户端网络指针
    MainWindow* m_CommitInterface; // 客户端登录界面
    Register* m_RegisterInterface; // 客户端注册界面
    UsrInterface* m_UsrInterface; // 客户端聊天界面

    QTimer* m_heartTimer; // 心跳包定时器
    //QString m_currentQQ; // 当前用户的QQ号码
};

#endif // IMSYSTEM_H
