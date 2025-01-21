#ifndef VIDEOINTERFACE_H
#define VIDEOINTERFACE_H

#include <QWidget>

namespace Ui {
class VideoInterface;
}

class VideoInterface : public QWidget
{
    Q_OBJECT

public:
    explicit VideoInterface(QWidget *parent = nullptr);
    ~VideoInterface();

private:
    Ui::VideoInterface *ui;
};

#endif // VIDEOINTERFACE_H
