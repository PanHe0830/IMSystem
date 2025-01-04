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
}

void UserInterface::Connect()
{
    connect(ui->lb_usr , &MyQLabel::clicked,this,&UserInterface::slot_SendClieckedEvents);
}

void UserInterface::slot_SendClieckedEvents()
{
    emit SIG_clicked();
}
