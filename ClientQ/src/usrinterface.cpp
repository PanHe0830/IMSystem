#include "usrinterface.h"
#include "ui_usrinterface.h"

#include <QMenu>
#include <QCursor>

UsrInterface::UsrInterface(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UsrInterface)
{
    ui->setupUi(this);
    Init();
    Connect();
}

UsrInterface::~UsrInterface()
{
    delete ui;
}

void UsrInterface::AddFriend(QString account)
{
    QStandardItem* model = new QStandardItem(account);
    m_pModel->appendRow(model);
}

void UsrInterface::Init()
{
    this->setMinimumSize(280,800);
    this->setMaximumSize(280,800);

    QImage image;
    if(!image.load("://Resource/picture.jpg"))
    {
        qDebug() << "加载图片失败";
    }
    ui->lb_usrpicture->setPixmap(QPixmap::fromImage(image));

    ui->le_label->setText("123456789");

    ui->tv_relation->setContextMenuPolicy(Qt::CustomContextMenu);

    m_pModel = new QStandardItemModel();

    ui->tv_relation->setModel(m_pModel);

    m_addFriend = new AddFriendInterface();
    m_addFriend->close();
    connect(m_addFriend,&AddFriendInterface::SIG_addFriendREQ , this , &UsrInterface::slot_sendAddFieReq);
    connect(m_addFriend,&AddFriendInterface::SIG_addFriend , this , &UsrInterface::slot_AddFie);
}

void UsrInterface::Connect()
{
    connect(ui->tv_relation, &QTreeView::customContextMenuRequested,this, &UsrInterface::slot_showContextMenu);
    connect(ui->pb_search , &QPushButton::clicked , this , &UsrInterface::slot_showaddFriendInterface);
    connect(ui->tv_relation , &QTreeView::doubleClicked , this , &UsrInterface::slot_showChatInterface);
    connect(ui->tv_relation , &QTreeView::clicked , this , &UsrInterface::slot_clickedEvent);
}

void UsrInterface::slot_showContextMenu(const QPoint &pos)
{
    QMenu* menu = new QMenu();

    QAction *actionDelete = menu->addAction("删除好友");
    connect(actionDelete , &QAction::triggered , this , &UsrInterface::slot_deleteFriend);

    QAction *createGroup = menu->addAction("创建分组");
    connect(createGroup , &QAction::triggered , this , &UsrInterface::slot_createGroup);

    menu->exec(QCursor::pos());
}

void UsrInterface::slot_deleteFriend()
{

}

void UsrInterface::slot_createGroup()
{

}

void UsrInterface::slot_showaddFriendInterface()
{
    m_addFriend->show();
}

void UsrInterface::slot_sendAddFieReq(QString account)
{
    emit SIG_AddFriendREQ(account);
}

void UsrInterface::slot_AddFie(QString account)
{
    AddFriend(account);
}

void UsrInterface::slot_showChatInterface(const QModelIndex &index)
{
    QString str = index.data().toString();
    qDebug() << str;

    m_chatInterface = new ChatInterface();
    m_chatInterface->setTarUsr(str);
    m_chatInterface->show();
}

void UsrInterface::slot_clickedEvent(const QModelIndex &index)
{

}
