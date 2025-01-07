#ifndef USRINTERFACE_H
#define USRINTERFACE_H

#include <QWidget>
#include <QPixmap>
#include <QStandardItemModel>

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

private slots:
    void slot_showContextMenu(const QPoint &pos);

    void slot_deleteFriend();

    void slot_createGroup();

    void slot_showaddFriendInterface();

    void slot_sendAddFieReq(QString account);

    void slot_AddFie(QString account);

    void slot_showChatInterface(const QModelIndex &index);

    void slot_clickedEvent(const QModelIndex &index);

signals:
    void SIG_AddFriendREQ(QString account);

private:
    QStandardItemModel* m_pModel;

    AddFriendInterface* m_addFriend;

    ChatInterface* m_chatInterface;

private:
    Ui::UsrInterface *ui;
};

#endif // USRINTERFACE_H
