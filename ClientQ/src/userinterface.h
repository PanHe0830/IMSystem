#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QWidget>

#include "addfriendinterface.h"

namespace Ui {
class UserInterface;
}

class UserInterface : public QWidget
{
    Q_OBJECT

public:
    explicit UserInterface(QWidget *parent = nullptr);
    ~UserInterface();

private:
    void Init();

    void Connect();

private slots:
    void slot_SendClieckedEvents();

    void slot_showAddInterface();

    void slot_SendSearchFriend(QString tarAccount);

    void slot_AddFriend();

signals:
    void SIG_clicked();

    void SIG_addFriend();

    void SIG_SearchFriend(QString tarAccount);

private:
    AddFriendInterface* m_addFriendInterface;
    QString m_Account;

private:
    Ui::UserInterface *ui;
};

#endif // USERINTERFACE_H
