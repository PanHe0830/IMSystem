#ifndef ADDFRIENDINTERFACE_H
#define ADDFRIENDINTERFACE_H

#include <QWidget>

namespace Ui {
class AddFriendInterface;
}

class AddFriendInterface : public QWidget
{
    Q_OBJECT

public:
    explicit AddFriendInterface(QWidget *parent = nullptr);
    ~AddFriendInterface();

private:
    void Init();

    void Connect();

private slots:
    void slot_search();

    void slot_addFriend();

signals:
    void SIG_addFriendREQ(QString tarAccount);

    void SIG_addFriend();

private:
    Ui::AddFriendInterface *ui;
};

#endif // ADDFRIENDINTERFACE_H
