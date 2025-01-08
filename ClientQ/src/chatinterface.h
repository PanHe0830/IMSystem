#ifndef CHATINTERFACE_H
#define CHATINTERFACE_H

#include <QWidget>

class QScrollArea;
class QVBoxLayout;
class QDateTime;

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

    void slot_recvMessage(QString msg);

    void slot_textLength(QString str); // 判断新的文本是否超过规定的大小

signals:
    void SIG_closeInterface();

    void SIG_sendMessage(QString str);

private:
    QScrollArea* scrollArea;
    QVBoxLayout* messageLayout;
    QString m_currentUsrName;

    QDateTime timeCurrent;
private:
    Ui::ChatInterface *ui;
};

#endif // CHATINTERFACE_H
