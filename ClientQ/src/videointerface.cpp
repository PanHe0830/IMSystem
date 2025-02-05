#include "videointerface.h"
#include "ui_videointerface.h"

#include <thread>
#include <vector>
#include <QThread>

#include "opencvwidget.h"
#include "Message.h"
#include "client.h"

VideoInterface::VideoInterface(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoInterface)
{
    ui->setupUi(this);
    ui->lb_down->setText("正在连接");
    ui->lb_up->setText("正在连接");
    m_Video = new IMSystemOpenCV();
    Connect();
}

VideoInterface::~VideoInterface()
{
    if(m_Video)
    {
        delete m_Video;
        m_Video = nullptr;
    }

    delete ui;
}

void VideoInterface::ShowMyVideo( QString usrAccount , QString tarAccount )
{
    if(!m_Video->openVideoStream())
    {
        qDebug() << "摄像头打开失败";
        return ;
    }

    std::thread thread(&VideoInterface::threadVideoShow , this , m_Video , usrAccount , tarAccount);
    thread.detach();
}

void VideoInterface::ShowTarVideo(cv::Mat video)
{
    //ui->wid_fri;
    if(video.empty())
    {
        return;
    }

    ui->wid_fri->setImage(video);
}

void VideoInterface::OpenVideoSend()
{
    m_bVideoSend = true;
}

void VideoInterface::CloseVideoSend()
{
    this->close();
    m_bVideoflag = false;
    m_bVideoSend = false;
}

void VideoInterface::Connect()
{
    connect(this,&VideoInterface::SIG_VideoClose,this,&VideoInterface::slot_CloseVideo);
}

void VideoInterface::threadVideoShow(IMSystemOpenCV* video , QString usrAccount , QString tarAccount)
{
    // 显示到界面上
    while(m_bVideoflag)
    {
        cv::Mat temp = video->captureFrame();
        if(temp.empty()) break;
        //ui->wid_usr->setImage(temp);
        QMetaObject::invokeMethod(ui->wid_usr, "setImage", Qt::QueuedConnection, Q_ARG(cv::Mat, temp));


        if(m_bVideoSend)
        {
            cv::imencode(".jpg", temp, buf);

            CVideo_Data videoMsg;
            memcpy(videoMsg.usrAccount,usrAccount.toStdString().c_str(),sizeof(videoMsg.usrAccount));
            //memcpy(videoMsg.tarAccount,usrAccount.toStdString().c_str(),sizeof(videoMsg.usrAccount));
            memcpy(videoMsg.tarAccount,tarAccount.toStdString().c_str(),sizeof(videoMsg.tarAccount));
            videoMsg.videoBuff = buf;
            videoMsg.head.nSize += videoMsg.videoBuff.size(); // 计算总数据大小

            //数据序列化
            std::vector<unsigned char> serializedData = videoMsg.serialize();

            bool bflag = Client::client_SendMessage(reinterpret_cast<char*>(serializedData.data()),serializedData.size());
            if(!bflag)
            {
                QThread::msleep(100);
                continue; // 继续尝试发送下一帧，而不是直接退出
            }
        }
    }
    qDebug() << "this thread is close";
}

void VideoInterface::closeEvent(QCloseEvent *event)
{
    emit SIG_VideoClose();
}

void VideoInterface::slot_CloseVideo()
{
    m_bVideoflag = false;
}


