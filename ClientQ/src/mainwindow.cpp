#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScreen>
#include <QImage>
#include <QDebug>
#include <QPushButton>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Init();
    Connent();

    // 测试代码
    ui->le_account->setText("852000388");
    ui->le_password->setText("123456");
    // 测试代码
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Init()
{
    this->setWindowTitle(u8"登录界面");
    // 获得屏幕的大小
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    // 设置显示的位置 -- 想让登录界面显示在屏幕的中间位置
    this->setGeometry(screenWidth/2 - 470/2,screenHeight/2 - 300/2,470,300);

    // 设置最大和最小让界面不能拖拽移动
    this->setMinimumSize(470,300);
    this->setMaximumSize(470,300);
    QImage image;
    if(!image.load("://Resource/picture.jpg"))
    {
        qDebug() << "加载图片失败";
    }
    ui->lb_picture->setPixmap(QPixmap::fromImage(image));
    // 自适应
    ui->lb_picture->setScaledContents(true);
}

void MainWindow::Connent()
{
    connect(ui->pb_commit , &QPushButton::clicked , this , &MainWindow::ButtonCommitClicked);
    connect(ui->pb_register , &QPushButton::clicked , this , &MainWindow::ButtonRegisterClicked);
}

void MainWindow::ButtonCommitClicked()
{
    // 点击登录按钮
    QString account = ui->le_account->text();
    QString password = ui->le_password->text();

    emit SIG_SendCommitREQ(account,password);

}

void MainWindow::ButtonRegisterClicked()
{
    // 点击注册按钮
    emit SIG_SendRegisterREQ();
}

void MainWindow::slot_setAccount(QString Account)
{
    ui->le_account->setText(Account);
}

QString MainWindow::GetUsrName()
{
    return ui->le_account->text();
}
