#ifndef OPENCVWIDGET_H
#define OPENCVWIDGET_H

#include <QWidget>

#include <QPainter>
#include <QImage>
#include <opencv2/opencv.hpp>

class OpenCVWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OpenCVWidget(QWidget *parent = nullptr);

    Q_INVOKABLE void setImage(const cv::Mat &mat);

    //void setFriImage(const cv::Mat &mat);

protected:
    void paintEvent(QPaintEvent *) override;

    //void closeEvent(QCloseEvent *event) override;

//signals:
    //void SIG_CloseVideo();

private:
    QImage usrimg;
    //QImage friimg;

    QImage cvMatToQImage(const cv::Mat &mat);
    //QImage cvMatQImage(const cv::Mat &mat);
};

#endif // OPENCVWIDGET_H
