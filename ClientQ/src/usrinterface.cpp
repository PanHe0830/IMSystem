#include "usrinterface.h"
#include "ui_usrinterface.h"

#include <QMenu>
#include <QCursor>
#include <QMessageBox>

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
    if(!m_AccountToChat.empty())
    {
        for(auto ite : m_AccountToChat)
        {
            delete ite.second;
            ite.second = nullptr;
        }
        m_AccountToChat.clear();
    }

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

    m_AccountToChat.clear();
}

void UsrInterface::Connect()
{
    connect(ui->tv_relation, &QTreeView::customContextMenuRequested,this, &UsrInterface::slot_showContextMenu);
    connect(ui->pb_search , &QPushButton::clicked , this , &UsrInterface::slot_showaddFriendInterface);
    connect(ui->tv_relation , &QTreeView::doubleClicked , this , &UsrInterface::slot_showChatInterface);
    connect(ui->tv_relation , &QTreeView::clicked , this , &UsrInterface::slot_clickedEvent);
}

void UsrInterface::setUsrName(QString name)
{
    ui->le_usrname->setText(name);
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
    // 发送添加请求
    emit SIG_AddFriend(account , ui->le_usrname->text());
    //AddFriend(account);
}

void UsrInterface::slot_showChatInterface(const QModelIndex &index)
{
    QString str = index.data().toString();
    //qDebug() << str;

    m_chatInterface = new ChatInterface();
    m_chatInterface->setTarUsr(str);
    m_chatInterface->setUsr(ui->le_usrname->text());
    m_chatInterface->show();
    connect(m_chatInterface,&ChatInterface::SIG_closeInterface,this,&UsrInterface::slot_closeChatInterface);
    connect(m_chatInterface , &ChatInterface::SIG_sendMessage,this,&UsrInterface::slot_sendChatMessage);
    m_AccountToChat.insert(std::make_pair(str,m_chatInterface));
}

void UsrInterface::slot_clickedEvent(const QModelIndex &index)
{
    m_currentClickedItem = index.data().toString();
    qDebug() << m_currentClickedItem;
}

void UsrInterface::slot_closeChatInterface(QString tarAccount)
{
#if 0
    ChatInterface* temp = qobject_cast<ChatInterface*>(sender());
    if(temp != nullptr)
    {
        for(auto ite = m_chat.begin() ; ite != m_chat.end() ; ite++)
        {
            if(*ite == temp)
            {
                delete (*ite);
                *ite = nullptr;
                m_chat.erase(ite);
                break;
            }
        }
    }
#endif
    auto ite = m_AccountToChat.find(tarAccount);
    if(ite != m_AccountToChat.end())
    {
        delete ite->second;
        ite->second = nullptr;

        m_AccountToChat.erase(ite);
    }
}

void UsrInterface::slot_sendChatMessage(QString message , QString tarAccount)
{
    qDebug() << "slot_sendChatMessage";
    emit SIG_SendMessage(message , ui->le_usrname->text(),tarAccount);
}

void UsrInterface::slot_recvChatMessage(QString account , QString message)
{
    // 接收到了消息
    auto ite = m_AccountToChat.find(account);
    if(ite != m_AccountToChat.end())
    {
        // 1. 当前聊天界面已经显示
        ite->second->recvMessage(message);
    }
    else
    {
        // 2. 当前聊天界面没有显示
        m_chatInterface = new ChatInterface();
        m_chatInterface->setTarUsr(account);
        m_chatInterface->setUsr(ui->le_usrname->text());
        m_chatInterface->recvMessage(message);
        m_chatInterface->show();
        connect(m_chatInterface,&ChatInterface::SIG_closeInterface,this,&UsrInterface::slot_closeChatInterface);
        connect(m_chatInterface , &ChatInterface::SIG_sendMessage,this,&UsrInterface::slot_sendChatMessage);
        m_AccountToChat.insert(std::make_pair(account,m_chatInterface));
    }
}

void UsrInterface::slot_showFriendREQ(QString tarAccount, QString sourceAccount)
{
    QString str = tarAccount + "请求加为好友";
    int ret1 = QMessageBox::question(this,"标题", str, QMessageBox::Ok|QMessageBox::Cancel);
    if(ret1 == QMessageBox::Ok)
    {
        // 同意添加好友
        slot_showFriendACK(1,tarAccount);
        emit SIG_FriendAgree(sourceAccount , tarAccount , 1);
    }
    else
    {
        // 不同意添加好友
        emit SIG_FriendAgree(sourceAccount , tarAccount , 0);
    }
}

void UsrInterface::slot_showFriendACK(int flag , QString tarAccount)
{
    switch(flag)
    {
    case 1:
        QMessageBox::information(this,"提示","对方同意添加");
        AddFriend(tarAccount);
        break;
    case 0:
        QMessageBox::information(this,"提示","对方不同意添加");
        break;
    case -1:
        QMessageBox::information(this,"提示","消息错误");
        break;
    }
}
