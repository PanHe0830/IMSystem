#ifndef VIDEOINTERFACE_H
#define VIDEOINTERFACE_H

#include <QWidget>

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

private:
    void ShowMyVideo();

    void ShowTarVideo();

private:
    IMSystemOpenCV* m_videoCV; // 视频类

private:
    Ui::VideoInterface *ui;
};

#endif // VIDEOINTERFACE_H
