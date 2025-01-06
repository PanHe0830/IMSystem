#include "friendrelationinterface.h"
#include "ui_friendrelationinterface.h"

#include <QTreeView>
#include <QMenu>
#include <QAction>

FriendRelationInterface::FriendRelationInterface(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FriendRelationInterface)
{
    ui->setupUi(this);
    Init();
    Connect();
}

FriendRelationInterface::~FriendRelationInterface()
{
    delete ui;
}

void FriendRelationInterface::InitInterface(QRect rect)
{
    this->setGeometry(rect);
}

void FriendRelationInterface::AddFriend(QString str)
{
    QStandardItem* m_stand = new QStandardItem(str);
    m_friend->appendRow(m_stand);
}

void FriendRelationInterface::Init()
{
    m_pModel = new QStandardItemModel(ui->tv_show);
    m_pModel->setHorizontalHeaderLabels(QStringList() << "friend");

    m_friend = new QStandardItem("我的好友");
    m_pModel->appendRow(m_friend);

    // 开启右键菜单栏界面
    ui->tv_show->setContextMenuPolicy(Qt::CustomContextMenu);
}

void FriendRelationInterface::Connect()
{
    connect(ui->tv_show,&QTreeView::customContextMenuRequested,this,&FriendRelationInterface::slot_ActionTriggered);
}

void FriendRelationInterface::slot_ActionTriggered(const QPoint& pos)
{
     //创建右键菜单
    QMenu menu;

    //添加action
    QAction *actionDelete = new QAction(QStringLiteral("删除"));
    menu.addAction(actionDelete);
    connect(actionDelete,&QAction::triggered,this,&FriendRelationInterface::slot_TreeMemberDelete);
}

void FriendRelationInterface::slot_TreeMemberDelete()
{

}
