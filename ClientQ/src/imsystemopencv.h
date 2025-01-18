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
private:
    //cv::VideoCapture capture;
};

#endif // IMSYSTEMOPENCV_H
