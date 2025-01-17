#include "chatinterface.h"
#include "ui_chatinterface.h"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QPalette>
#include <QListWidgetItem>
#include <QMessageBox>

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

void ChatInterface::setUsr(QString usrAccount)
{
    m_currentUsrName = usrAccount;
}

void ChatInterface::Init()
{
    m_currentUsrName = "";
    QListWidgetItem* listItem = new QListWidgetItem("123");
    ui->lw_showmes->addItem("123");
    listItem->setTextAlignment(Qt::AlignCenter);
}

void ChatInterface::Connect()
{
    connect(ui->pb_send,&QPushButton::clicked , this , &ChatInterface::slot_sendMessage);
    connect(ui->le_message , &QLineEdit::textEdited , this , &ChatInterface::slot_textLength);
}

void ChatInterface::closeEvent(QCloseEvent *event)
{
    emit SIG_closeInterface(ui->lb_tar->text());
}

QDateTime ChatInterface::showTime()
{
    // 获取系统当前时间
    //QDateTime currentDateTime = QDateTime::currentDateTime();
    //QString currentTime = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");

    return QDateTime::currentDateTime();
    // 输出当前时间
}

void ChatInterface::slot_sendMessage()
{
    /** 如果消息间隔大于5分钟显示时间 */
    int minutes = timeCurrent.secsTo(showTime());
    timeCurrent = showTime();

    if(minutes > 5)
    {
        ui->lw_showmes->addItem(timeCurrent.toString("yyyy-MM-dd hh:mm:ss"));
    }
    /** */

    QString message = m_currentUsrName + ":" + ui->le_message->text();

    QListWidgetItem* listItem = new QListWidgetItem(message);
    listItem->setTextAlignment(Qt::AlignRight);
    ui->lw_showmes->addItem(listItem);

    emit SIG_sendMessage(message , ui->lb_tar->text());
}

void ChatInterface::recvMessage(QString msg)
{
    /** 如果消息间隔大于5分钟显示时间 */
    int minutes = timeCurrent.secsTo(showTime());
    timeCurrent = showTime();

    if(minutes > 5)
    {
        ui->lw_showmes->addItem(timeCurrent.toString("yyyy-MM-dd hh:mm:ss"));
    }
    /** */

    //QString temp = ui->lb_tar->text() + ":" + msg;
    QListWidgetItem* listItem = new QListWidgetItem(msg);
    listItem->setTextAlignment(Qt::AlignLeft);
    ui->lw_showmes->addItem(listItem);
}

void ChatInterface::slot_textLength(QString str)
{
    QString temp = "";

    if(str.length() <= 200)
    {
        temp = str;
    }
    else
    {
        QMessageBox::information(this,"提示","消息超过规定限制");
    }
    ui->le_message->setText(temp);
}

QString ChatInterface::GetTarAccount()
{
    return ui->lb_tar->text();
}

