#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // 初始化界面
    void Init();
    // 绑定信号和槽
    void Connent();

private slots:
    // 登录按钮
    void ButtonCommitClicked();
    // 注册按钮
    void ButtonRegisterClicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
