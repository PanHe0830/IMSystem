#ifndef CHATINTERFACE_H
#define CHATINTERFACE_H

#include <QWidget>
#include <QDateTime>

class QScrollArea;
class QVBoxLayout;
class VideoInterface;

namespace Ui {
class ChatInterface;
}

class ChatInterface : public QWidget
{
    Q_OBJECT

public:
    explicit ChatInterface(QWidget *parent = nullptr);
    ~ChatInterface();

    void setTarUsr(QString tarAccount);

    void setUsr(QString usrAccount);

private:
    void Init();

    void Connect();

    void updateBackground();

protected:
    void closeEvent(QCloseEvent *event) override;

    QDateTime showTime(); // 聊天时显示当前时间

private slots:
    void slot_sendMessage();

    void slot_textLength(QString str); // 判断新的文本是否超过规定的大小

    void slot_showVideoInterface(); // 显示视频通讯界面

public slots:
    void slot_chatInterfaceVideoREQ(QString usrAccount , QString tarAccount);

public:
    void recvMessage(QString msg);

public:
    QString GetTarAccount();

signals:
    void SIG_closeInterface(QString tarAccount);

    void SIG_sendMessage(QString str , QString tarAccount);

    void SIG_chatInterfaceSendVideoREQ(QString usrAccount , QString tarAccount);

    void SIG_VideoACK(bool bflag);

protected:
    virtual void moveEvent(QMoveEvent *event) override;

private:
    void SetVideoInterfacePosition(VideoInterface* video); // 设置视频界面的位置

private:
    QScrollArea* scrollArea;
    QVBoxLayout* messageLayout;
    QString m_currentUsrName;

    QDateTime timeCurrent;

    VideoInterface* m_videoInterface = nullptr; // 视频聊天界面
private:
    Ui::ChatInterface *ui;
};

#endif // CHATINTERFACE_H
