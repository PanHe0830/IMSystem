/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *lb_picture;
    QLabel *lb_account;
    QLabel *lb_password;
    QLineEdit *le_account;
    QLineEdit *le_password;
    QPushButton *pb_commit;
    QPushButton *pb_register;
    QMenuBar *menubar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(476, 293);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        lb_picture = new QLabel(centralwidget);
        lb_picture->setObjectName("lb_picture");
        lb_picture->setGeometry(QRect(10, 0, 171, 271));
        lb_account = new QLabel(centralwidget);
        lb_account->setObjectName("lb_account");
        lb_account->setGeometry(QRect(190, 70, 31, 16));
        lb_password = new QLabel(centralwidget);
        lb_password->setObjectName("lb_password");
        lb_password->setGeometry(QRect(190, 130, 31, 16));
        le_account = new QLineEdit(centralwidget);
        le_account->setObjectName("le_account");
        le_account->setGeometry(QRect(230, 70, 201, 23));
        le_password = new QLineEdit(centralwidget);
        le_password->setObjectName("le_password");
        le_password->setGeometry(QRect(230, 130, 201, 23));
        pb_commit = new QPushButton(centralwidget);
        pb_commit->setObjectName("pb_commit");
        pb_commit->setGeometry(QRect(210, 170, 80, 23));
        pb_register = new QPushButton(centralwidget);
        pb_register->setObjectName("pb_register");
        pb_register->setGeometry(QRect(330, 170, 80, 23));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 476, 20));
        MainWindow->setMenuBar(menubar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        lb_picture->setText(QCoreApplication::translate("MainWindow", "picture", nullptr));
        lb_account->setText(QCoreApplication::translate("MainWindow", "\350\264\246\345\217\267", nullptr));
        lb_password->setText(QCoreApplication::translate("MainWindow", "\345\257\206\347\240\201", nullptr));
        pb_commit->setText(QCoreApplication::translate("MainWindow", "\347\231\273\345\275\225", nullptr));
        pb_register->setText(QCoreApplication::translate("MainWindow", "\346\263\250\345\206\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
