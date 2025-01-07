#ifndef CHATINTERFACE_H
#define CHATINTERFACE_H

#include <QWidget>

class QScrollArea;
class QVBoxLayout;

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

private:
    void Init();

    void Connect();

    void updateBackground();

private:
    QScrollArea* scrollArea;
    QVBoxLayout* messageLayout;

private:
    Ui::ChatInterface *ui;
};

#endif // CHATINTERFACE_H
