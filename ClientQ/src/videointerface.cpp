#include "videointerface.h"
#include "ui_videointerface.h"

#include "imsystemopencv.h"

VideoInterface::VideoInterface(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoInterface)
{
    ui->setupUi(this);
    ui->lb_down->setText("正在连接");
    ui->lb_up->setText("正在连接");

    m_videoCV = new IMSystemOpenCV();
}

VideoInterface::~VideoInterface()
{
    delete ui;
}

void VideoInterface::ShowMyVideo()
{

}
