#ifndef FRIENDRELATIONINTERFACE_H
#define FRIENDRELATIONINTERFACE_H

#include <QWidget>
#include <QStandardItemModel>
#include <QPoint>

namespace Ui {
class FriendRelationInterface;
}

class FriendRelationInterface : public QWidget
{
    Q_OBJECT

public:
    explicit FriendRelationInterface(QWidget *parent = nullptr);
    ~FriendRelationInterface();

public:
    void InitInterface(QRect rect);

    void AddFriend(QString str);

private:
    void Init();

    void Connect();

private slots:
    void slot_ActionTriggered(const QPoint& pos);

    void slot_TreeMemberDelete();

private:
    QStandardItemModel* m_pModel;
    QStandardItem* m_friend;

private:
    Ui::FriendRelationInterface *ui;
};

#endif // FRIENDRELATIONINTERFACE_H
