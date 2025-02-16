﻿#ifndef VIDEOINTERFACE_H
#define VIDEOINTERFACE_H

#include <QWidget>
#include "imsystemopencv.h"

class vector;

namespace Ui {
class VideoInterface;
}

class IMSystemOpenCV;

class VideoInterface : public QWidget
{
    Q_OBJECT

public:
    explicit VideoInterface(QWidget *parent = nullptr);
    ~VideoInterface();

    void ShowMyVideo( QString usrAccount , QString tarAccount );

    void ShowTarVideo(cv::Mat video);

    void OpenVideoSend();

    void CloseVideoSend();

private:
    void Connect();

    void threadVideoShow(IMSystemOpenCV* video, QString usrAccount , QString tarAccount);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void slot_CloseVideo();

signals:
    void SIG_VideoClose();

private:
    IMSystemOpenCV* m_Video;

    std::atomic<bool> m_bVideoflag = true;
    //bool m_bVideoflag = true;

    bool m_bVideoSend = false;

    std::vector<unsigned char> buf;

private:
    Ui::VideoInterface *ui;
};

#endif // VIDEOINTERFACE_H
