#include "chatinterface.h"
#include "ui_chatinterface.h"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QPalette>
#include <QListWidgetItem>

ChatInterface::ChatInterface(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatInterface)
{
    ui->setupUi(this);
    Init();
    Connect();
}

ChatInterface::~ChatInterface()
{
    delete ui;
}

void ChatInterface::setTarUsr(QString tarAccount)
{
    ui->lb_tar->setText(tarAccount);
}

void ChatInterface::Init()
{
    QListWidgetItem* listItem = new QListWidgetItem("123");
    ui->lw_showmes->addItem("123");
    listItem->setTextAlignment(Qt::AlignCenter);
}

void ChatInterface::Connect()
{

}

