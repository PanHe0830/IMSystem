#include "videointerface.h"
#include "ui_videointerface.h"

VideoInterface::VideoInterface(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::VideoInterface)
{
    ui->setupUi(this);
    ui->lb_down->setText("正在连接");
    ui->lb_up->setText("正在连接");
}

VideoInterface::~VideoInterface()
{
    delete ui;
}
