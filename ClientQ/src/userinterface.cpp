#include "userinterface.h"
#include "myqlabel.h"
#include "ui_userinterface.h"

#include <QScreen>
#include <QLabel>

UserInterface::UserInterface(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserInterface)
{
    ui->setupUi(this);
    Init();
    Connect();
}

UserInterface::~UserInterface()
{
    delete ui;
}

void UserInterface::Init()
{
    // 获得屏幕的大小
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    // 设置显示的位置 -- 想让登录界面显示在屏幕的中间位置
    this->setGeometry(screenWidth/2 - 650/2,screenHeight/2 - 500/2,650,500);
    this->setMinimumSize(650,500);
    this->setMaximumSize(650,500);

    m_addFriendInterface = new AddFriendInterface();
    m_addFriendInterface->close();

    // 当前界面是界面朋友按钮点击后显示的界面
    m_FriendRelationInterface = new FriendRelationInterface(ui->wid_middle);
    m_FriendRelationInterface->close();
}

void UserInterface::Connect()
{
    connect(ui->lb_usr , &MyQLabel::clicked,this,&UserInterface::slot_SendClieckedEvents);
    connect(ui->pb_addFriend , &QPushButton::clicked , this , &UserInterface::slot_showAddInterface);
    connect(m_addFriendInterface , &AddFriendInterface::SIG_addFriendREQ , this , &UserInterface::slot_SendSearchFriend);
    connect(m_addFriendInterface , &AddFriendInterface::SIG_addFriend , this , &UserInterface::slot_AddFriend);
    connect(ui->pb_friend , &QPushButton::clicked , this , &UserInterface::slot_showFriendrelation);
}

void UserInterface::slot_SendClieckedEvents()
{
    emit SIG_clicked();
}

void UserInterface::slot_showAddInterface()
{
    m_addFriendInterface->show();
}

void UserInterface::slot_SendSearchFriend(QString tarAccount)
{
    m_Account = tarAccount;
    emit SIG_SearchFriend(tarAccount);
}

void UserInterface::slot_AddFriend()
{
    // 将朋友添加到界面上
    //  m_Account;
    m_FriendRelationInterface->AddFriend(m_Account);
}

void UserInterface::slot_showFriendrelation()
{
    QRect rect = ui->wid_middle->geometry();
    m_FriendRelationInterface->InitInterface(rect);
    m_FriendRelationInterface->show();
}
