#ifndef IMSYSTEMOPENCV_H
#define IMSYSTEMOPENCV_H

extern "C" {
#include <libavdevice/avdevice.h>
#include <libavformat/avformat.h>
#include <libavutil/log.h>
}

#include <opencv2/opencv.hpp>

class IMSystemOpenCV
{
public:
    IMSystemOpenCV();
    ~IMSystemOpenCV();

public:
    bool openVideoStream(int deviceIndex = 0); // 开启视频流

    cv::Mat captureFrame(); // 捕获视频帧

    void release(); // 释放资源

    void setIsRelease(bool bflag); // 标志是否释放了资源

    bool getReleaseflag(); // 返回是否释放了资源的标志位

private:
    void InitFFmpeg();

private:
    cv::VideoCapture capture;

    bool m_bflag; // 是否释放资源标志位
};

#endif // IMSYSTEMOPENCV_H
