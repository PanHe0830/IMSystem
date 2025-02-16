﻿#ifndef IMSYSTEM_H
#define IMSYSTEM_H

#include <QObject>

#include "client.h"
#include "mainwindow.h"
#include "register.h"
#include "usrinterface.h"
#include "imsystemopencv.h"

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

    void HandleVideoREQ(SOCKET serverClient , MsgHead& head);

    void HandleVideoData(SOCKET serverClient , MsgHead& head);

    void HandleVideoACK(SOCKET serverClient , MsgHead& head);

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

    void slot_startQTimer();

    void slot_sendVideoREQ(QString usrQQ , QString tarAccount);

    void slot_sendVideoACK(bool bflag , QString usrAccount , QString tarAccount);

signals:
    void SIG_Account(QString Account);

    void SIG_FriendREQ(QString tarAccount , QString sourceAccount);

    void SIG_FriendACK(int flag , QString tarAccount);

    void SIG_ShowChatMessage(QString account , QString message);

    void SIG_AddFriend(QString friAccount);

    void SIG_MasterTimeOut();

    void SIG_VideoREQ(QString usrAccount , QString tarAccount); //  usrAccount - 当前用户  tarAccount - 谁给我发送的请求

    void SIG_videoReceived(cv::Mat video);

private:
    Client* m_Client; // 客户端网络指针
    MainWindow* m_CommitInterface; // 客户端登录界面
    Register* m_RegisterInterface; // 客户端注册界面
    UsrInterface* m_UsrInterface; // 客户端聊天界面

    QTimer* m_heartTimer; // 心跳包定时器
    //QString m_currentQQ; // 当前用户的QQ号码

private:
    void processFrame(cv::Mat frame);
};

#endif // IMSYSTEM_H
