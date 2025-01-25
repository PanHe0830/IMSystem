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

    void ShowMyVideo();

    void ShowTarVideo();

private:
    void Connect();

    void threadVideoShow(IMSystemOpenCV* video);

private slots:
    void slot_CloseVideo();

private:
    IMSystemOpenCV* m_Video;

    bool m_bVideoflag = true;

private:
    Ui::VideoInterface *ui;
};

#endif // VIDEOINTERFACE_H
