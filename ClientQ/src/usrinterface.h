#ifndef USRINTERFACE_H
#define USRINTERFACE_H

#include <QWidget>
#include <QPixmap>
#include <QStandardItemModel>
#include <map>

#include "addfriendinterface.h"
#include "chatinterface.h"

#include "imsystemopencv.h"

/**
*   用户界面
*/

namespace Ui {
class UsrInterface;
}

class UsrInterface : public QWidget
{
    Q_OBJECT

public:
    explicit UsrInterface(QWidget *parent = nullptr);
    ~UsrInterface();

public:
    void AddFriend(QString account);

private:
    void Init();

    void Connect();

public:
    void setUsrName(QString name);

    void closeVideo(QString tarNumber);

    void openVideo(QString tarNumber);

private slots:
    void slot_showContextMenu(const QPoint &pos);

    void slot_deleteFriend();

    void slot_createGroup();

    void slot_showaddFriendInterface();

    void slot_sendAddFieReq(QString account);

    void slot_AddFie(QString account);

    void slot_showChatInterface(const QModelIndex &index);

    void slot_clickedEvent(const QModelIndex &index);

    void slot_closeChatInterface(QString tarAccount); // 当关闭聊天界面时将保存聊天界面指针的容器中删掉对应的界面指针

    void slot_sendChatMessage(QString message , QString tarAccount);

    void slot_sendVideoREQ(QString usrQQ, QString tarAccount);

    void slot_interfaceVideoACK(bool bflag , QString usrAccount , QString tarAccount);

public slots:
    void slot_interfaceVideoREQ(QString usrAccount , QString tarAccount);

    void slot_videoReceived(cv::Mat temp);

public slots:
    void slot_recvChatMessage(QString account , QString message);

    void slot_showFriendREQ(QString tarAccount , QString sourceAccount);

    void slot_showFriendACK(int flag, QString tarAccount);

    void slot_addFriend(QString friAccount);

signals:
    void SIG_AddFriend(QString account , QString usrAccount);

    void SIG_AddFriendREQ(QString account);

    void SIG_SendMessage(QString message , QString usrAccount , QString tarAccount);

    void SIG_FriendAgree(QString tarAccount , QString sourceAccount , int flag);

    void SIG_usrInterFaceSendVideoREQ(QString usr , QString tarAccount);

    void SIG_usrVideoREQ(QString usrAccount , QString tarAccount);

    void SIG_interfaceVideoACK(bool bflag  , QString usrAccount , QString tarAccount);

    void SIG_videoRecv(cv::Mat temp);

private:
    QStandardItemModel* m_pModel;

    AddFriendInterface* m_addFriend;

    ChatInterface* m_chatInterface; // 聊天界面

    std::map<QString , ChatInterface*> m_AccountToChat; // 每个好友和于好友聊天界面  first -- IMSnumber  second -- 聊天界面指针

    QString m_currentClickedItem; // 当前鼠标点击的用户

private:
    Ui::UsrInterface *ui;
};

#endif // USRINTERFACE_H
