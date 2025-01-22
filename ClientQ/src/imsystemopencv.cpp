#include "imsystemopencv.h"
#include <qdebug>

#include <QList>
#include <QCameraDevice>
#include <QMediaDevices>

IMSystemOpenCV::IMSystemOpenCV()
{
    //InitFFmpeg();

    //QList<QCameraDevice> cameras = QMediaDevices::videoInputs();
    //QCameraDevice cameraDevice = cameras.first();

    //// 获取摄像头设备的ID（可以是路径或其他标识符）
    //QString cameraId = cameraDevice.id();
    //qDebug() << "Selected Camera ID:" << cameraId;

    // 如果有可用的摄像头
    //if (!cameras.isEmpty()) {
    //    qDebug() << "Available cameras:";
    //    for (const QCameraDevice &camera : cameras) {
    //        qDebug() << "Camera name:" << camera.description();
    //        qDebug() << "Camera id:" << camera.id();
    //    }
    //} else {
    //    qDebug() << "No cameras available.";
    //}
}

IMSystemOpenCV::~IMSystemOpenCV()
{

}

bool IMSystemOpenCV::openVideoStream(int deviceIndex) {
    if (!capture.open(deviceIndex)) {
        std::cerr << "无法打开摄像头!" << std::endl;
        return false;
    }
    return true;
}

cv::Mat IMSystemOpenCV::captureFrame() {
    cv::Mat frame;
    capture >> frame;  // 从摄像头获取一帧
    if (frame.empty()) {
        std::cerr << "无法获取帧!" << std::endl;
    }
    return frame;
}

void IMSystemOpenCV::release() {
    capture.release();
}

void IMSystemOpenCV::InitFFmpeg()
{
    //capture.open(0);

    // 初始化FFmpeg设备库
    avdevice_register_all();

    // 在 Windows 上使用 dshow，Linux 使用 v4l2
    const char* input_format_name = "dshow";  // Windows
    // const char* input_format_name = "v4l2";  // Linux

    // 获取输入格式
    const AVInputFormat *input_format = av_find_input_format(input_format_name);
    if (!input_format) {
        qDebug() << "无法找到输入格式: " << input_format_name;
        return;
    }

    // 打开一个虚拟设备来列出所有设备
    AVFormatContext *format_context = nullptr;
    AVDictionary *options = nullptr;
    int ret = avformat_open_input(&format_context, "dummy", input_format, &options);

    //char buf[AV_ERROR_MAX_STRING_SIZE];
    //av_strerror(ret,&buf,sizeof(buf));
    //av_err2str(ret);

    char buf[64];  // 错误缓冲区
    av_strerror(ret, buf, sizeof(buf));  // 获取错误信息

    if (ret < 0) {
        qDebug() << "无法打开输入: " << buf;
        return;
    }

    // 输出所有设备信息
    qDebug() << "可用的摄像头设备：";;
    for (unsigned int i = 0; i < format_context->nb_streams; i++) {
        AVStream *stream = format_context->streams[i];
        if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            qDebug() << "设备 " << i << ": " << stream->codecpar->codec_id;
        }
    }

    // 关闭设备
    avformat_close_input(&format_context);
}
