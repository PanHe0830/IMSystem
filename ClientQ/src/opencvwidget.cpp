#include "opencvwidget.h"

#include <QThread>

OpenCVWidget::OpenCVWidget(QWidget *parent)
    : QWidget{parent}
{

}

void OpenCVWidget::setImage(const cv::Mat &mat)
{
    img = cvMatToQImage(mat);
    update(); // 触发重绘
    QThread::msleep(30);
}

void OpenCVWidget::setFriImage(const cv::Mat &mat)
{
    img = cvMatToQImage(mat);
    update(); // 触发重绘
    QThread::msleep(30);
}

void OpenCVWidget::paintEvent(QPaintEvent *)
{
    if (img.isNull())
        return;

    QPainter painter(this);
    painter.drawImage(rect(), img);
}

//void OpenCVWidget::closeEvent(QCloseEvent *event)
//{
//    emit SIG_CloseVideo();
//}

QImage OpenCVWidget::cvMatToQImage(const cv::Mat &mat)
{
    if (mat.empty())
        return QImage();

    switch (mat.type())
    {
    case CV_8UC1:
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
    case CV_8UC3:
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_BGR888);
    case CV_8UC4:
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
    default:
        return QImage();
    }
}
