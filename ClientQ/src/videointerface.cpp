#include "videointerface.h"
#include "ui_videointerface.h"

#include "imsystemopencv.h"
#include "opencvwidget.h"
#include <thread>

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

void VideoInterface::ShowMyVideo()
{
    if(!m_Video->openVideoStream())
    {
        qDebug() << "摄像头打开失败";
        return ;
    }

    std::thread thread(&VideoInterface::threadVideoShow , this , m_Video);
    thread.detach();
}

void VideoInterface::ShowTarVideo()
{
    //ui->wid_fri;
}

void VideoInterface::Connect()
{
    connect(ui->wid_usr,&OpenCVWidget::SIG_CloseVideo,this,&VideoInterface::slot_CloseVideo);
}

void VideoInterface::threadVideoShow(IMSystemOpenCV* video)
{
    // 显示到界面上
    while(m_bVideoflag)
    {
        cv::Mat temp = video->captureFrame();
        ui->wid_usr->setImage(temp);
    }
}

void VideoInterface::slot_CloseVideo()
{
    m_bVideoflag = false;
}
