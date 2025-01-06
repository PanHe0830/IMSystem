#include "addfriendinterface.h"
#include "ui_addfriendinterface.h"

#include <QPushButton>

AddFriendInterface::AddFriendInterface(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddFriendInterface)
{
    ui->setupUi(this);
    Init();
    Connect();
}

AddFriendInterface::~AddFriendInterface()
{
    delete ui;
}

void AddFriendInterface::Init()
{

}

void AddFriendInterface::Connect()
{
    connect(ui->pb_search , &QPushButton::clicked , this , &AddFriendInterface::slot_search);
    connect(ui->pb_add , &QPushButton::clicked , this , &AddFriendInterface::slot_addFriend);
}

void AddFriendInterface::slot_search()
{
    tarAccount = ui->le_search->text();

    emit SIG_addFriendREQ(tarAccount);
}

void AddFriendInterface::slot_addFriend()
{
    emit SIG_addFriend(tarAccount);
}
