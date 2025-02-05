#include "opencvwidget.h"

#include <QThread>
#include <QTimer>

OpenCVWidget::OpenCVWidget(QWidget *parent)
    : QWidget{parent}
{

}

void OpenCVWidget::setImage(const cv::Mat &mat)
{
    usrimg = cvMatToQImage(mat);
    //QTimer::singleShot(0, this, SLOT(update()));  // 异步更新 UI
    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);
}

void OpenCVWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if (!usrimg.isNull())
    {
        QSize widgetSize = size();
        QSize imageSize = usrimg.size();
        QImage scaledImage = usrimg.scaled(widgetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        int x = (widgetSize.width() - scaledImage.width()) / 2;
        int y = (widgetSize.height() - scaledImage.height()) / 2;
        QRect targetRect(x, y, scaledImage.width(), scaledImage.height());

        painter.drawImage(targetRect, scaledImage);
    }
}

QImage OpenCVWidget::cvMatToQImage(const cv::Mat &mat)
{
    if (mat.empty()) return QImage();

    switch (mat.type())
    {
    case CV_8UC1:
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8).copy();
    case CV_8UC3:
    {
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        return QImage(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888).copy();
    }
    case CV_8UC4:
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32).copy();
    default:
        return QImage();
    }
}
