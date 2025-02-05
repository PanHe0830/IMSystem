#include "chatinterface.h"
#include "ui_chatinterface.h"

#include <QVBoxLayout>
#include <QScrollArea>
#include <QPalette>
#include <QListWidgetItem>
#include <QMessageBox>

#include "videointerface.h"

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

void ChatInterface::closeVideoThread()
{
    m_videoInterface->CloseVideoSend();
}

void ChatInterface::openVideoThread()
{
    m_videoInterface->OpenVideoSend();
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
    connect(ui->pb_video , &QPushButton::clicked , this , &ChatInterface::slot_showVideoInterface);
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

void ChatInterface::slot_showVideoInterface()
{
    m_videoInterface = new VideoInterface();
    emit SIG_chatInterfaceSendVideoREQ(m_currentUsrName,ui->lb_tar->text());
    SetVideoInterfacePosition(m_videoInterface);

    // TODO -- 显示自己的视频
    m_videoInterface->ShowMyVideo(m_currentUsrName,ui->lb_tar->text());
}

void ChatInterface::slot_chatInterfaceVideoREQ(QString usrAccount, QString tarAccount)
{
    QString temp = tarAccount + "请求和你通话";

    int ret = QMessageBox::information(this, "Title", temp,QMessageBox::Yes|QMessageBox::No);
    switch(ret)
    {
    case QMessageBox::No:
        emit SIG_VideoACK(false , usrAccount,tarAccount);
        break;
    case QMessageBox::Yes:
    {
        emit SIG_VideoACK(true , usrAccount,tarAccount);
        // 想通话
        if(m_videoInterface == nullptr)
        {
            m_videoInterface = new VideoInterface();
            SetVideoInterfacePosition(m_videoInterface);
            m_videoInterface->ShowMyVideo(m_currentUsrName,ui->lb_tar->text());
        }
        else
        {
            SetVideoInterfacePosition(m_videoInterface);
            m_videoInterface->ShowMyVideo(m_currentUsrName,ui->lb_tar->text());
            m_videoInterface->OpenVideoSend();
        }
    }
        break;
    }
}

void ChatInterface::slot_recvVideo(cv::Mat temp)
{
    m_videoInterface->ShowTarVideo(temp);
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

QString ChatInterface::GetTarAccount()
{
    return ui->lb_tar->text();
}

void ChatInterface::moveEvent(QMoveEvent *event)
{
    // 当聊天界面移动时视频界面跟着移动
    if(m_videoInterface != nullptr)
    {
        SetVideoInterfacePosition(m_videoInterface);
    }
}

void ChatInterface::SetVideoInterfacePosition(VideoInterface* video)
{
    ////窗口左上角的位置(含边框)
    //qDebug() << this->frameGeometry().x() << this->frameGeometry().y() << ;//1
    //qDebug() << this->x()  << this->y();//2
    //qDebug() << this->pos().x() << this->pos().y();//3
    ////窗口的宽度和高度(含边框)
    //qDebug() << this->frameGeometry().width() << this->frameGeometry().height();
    ////窗口左上角的位置(不含边框)
    //qDebug() << this->geometry().x() << this->geometry().y();
    ////窗口的宽度和高度(不含边框)
    //qDebug() << this->geometry().width() << this->geometry().height();//1
    //qDebug() << this->width() << this->height();//2
    //qDebug() << this->rect().width() << this->rect().height();//3
    //qDebug() << this->size().width() << this->size().height();//4

    int wid = width();
    //int heigh = height();
    int x = frameGeometry().x();
    int y = frameGeometry().y();
    video->move(wid + x , y);
    video->show();
}

