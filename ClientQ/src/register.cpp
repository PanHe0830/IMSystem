#include "register.h"
#include "ui_register.h"

#include <QMessageBox>

Register::Register(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Register)
{
    ui->setupUi(this);
    Init();
    Connect();
}

Register::~Register()
{
    delete ui;
}

void Register::Init()
{
    this->setMinimumSize(315,146);
    this->setMaximumSize(315,146);
    ui->pb_sure->setEnabled(false);
}

void Register::Connect()
{
    connect(ui->pb_sure , &QPushButton::clicked , this , &Register::IsPassWordSame);
    connect(ui->le_repass , &QLineEdit::editingFinished , this , &Register::IsSame);
}

void Register::IsPassWordSame()
{
    QString password = ui->le_pass->text();
    QString repassword = ui->le_repass->text();
    QString name = ui->le_name->text();
    if(password != repassword)
    {
        QMessageBox::information(this,"提示" , "输入的密码不同");
        return;
    }

    emit SIG_NewPassWord(password , name);
}

void Register::IsSame()
{
    QString password = ui->le_pass->text();
    QString repassword = ui->le_repass->text();

    if(password == repassword)
    {
        QMessageBox::information(this,"提示" , "输入的密码相同");
        ui->pb_sure->setEnabled(true);
    }
    else
    {
        QMessageBox::information(this,"提示" , "输入的密码不同");
        ui->pb_sure->setEnabled(false);
    }
}
