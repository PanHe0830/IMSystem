#ifndef USRINTERFACE_H
#define USRINTERFACE_H

#include <QWidget>
#include <QPixmap>
#include <QStandardItemModel>
#include <list>

#include "addfriendinterface.h"
#include "chatinterface.h"

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

private slots:
    void slot_showContextMenu(const QPoint &pos);

    void slot_deleteFriend();

    void slot_createGroup();

    void slot_showaddFriendInterface();

    void slot_sendAddFieReq(QString account);

    void slot_AddFie(QString account);

    void slot_showChatInterface(const QModelIndex &index);

    void slot_clickedEvent(const QModelIndex &index);

    void slot_closeChatInterface(); // 当关闭聊天界面时将保存聊天界面指针的容器中删掉对应的界面指针

    void slot_sendChatMessage(QString message , QString tarAccount);

signals:
    void SIG_AddFriendREQ(QString account);

    void SIG_SendMessage(QString message , QString usrAccount , QString tarAccount);

private:
    QStandardItemModel* m_pModel;

    AddFriendInterface* m_addFriend;

    ChatInterface* m_chatInterface; // 聊天界面

    std::list<ChatInterface*> m_chat;

private:
    Ui::UsrInterface *ui;
};

#endif // USRINTERFACE_H
