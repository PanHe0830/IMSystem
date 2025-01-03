#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

private:
    void Init();

    void Connect();

private slots:
    void IsPassWordSame();

    void IsSame();

signals:
    void SIG_NewPassWord(QString passWord);

private:
    Ui::Register *ui;
};

#endif // REGISTER_H
