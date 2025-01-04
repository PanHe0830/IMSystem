#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QWidget>

namespace Ui {
class UserInterface;
}

class UserInterface : public QWidget
{
    Q_OBJECT

public:
    explicit UserInterface(QWidget *parent = nullptr);
    ~UserInterface();

private:
    void Init();

    void Connect();

private slots:
    void slot_SendClieckedEvents();

signals:
    void SIG_clicked();

private:
    Ui::UserInterface *ui;
};

#endif // USERINTERFACE_H
