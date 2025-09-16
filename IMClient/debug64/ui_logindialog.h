/********************************************************************************
** Form generated from reading UI file 'logindialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINDIALOG_H
#define UI_LOGINDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginDialog
{
public:
    QTabWidget *tb_page;
    QWidget *pg_login;
    QLabel *lb_tel_login;
    QLabel *lb_password_login;
    QLineEdit *le_tel_login;
    QLineEdit *le_password_login;
    QPushButton *pb_clear_login;
    QPushButton *pb_commit_login;
    QWidget *pg_register;
    QLabel *lb_tel_register;
    QLabel *lb_password_register;
    QLabel *lb_nicname_register;
    QLineEdit *le_tel_register;
    QLineEdit *le_password_register;
    QLineEdit *le_nicname_register;
    QPushButton *pb_clear_register;
    QPushButton *pb_commit_register;
    QLabel *lb_icon;

    void setupUi(QDialog *LoginDialog)
    {
        if (LoginDialog->objectName().isEmpty())
            LoginDialog->setObjectName(QString::fromUtf8("LoginDialog"));
        LoginDialog->resize(368, 228);
        tb_page = new QTabWidget(LoginDialog);
        tb_page->setObjectName(QString::fromUtf8("tb_page"));
        tb_page->setGeometry(QRect(160, 0, 211, 231));
        pg_login = new QWidget();
        pg_login->setObjectName(QString::fromUtf8("pg_login"));
        lb_tel_login = new QLabel(pg_login);
        lb_tel_login->setObjectName(QString::fromUtf8("lb_tel_login"));
        lb_tel_login->setGeometry(QRect(10, 70, 31, 21));
        lb_password_login = new QLabel(pg_login);
        lb_password_login->setObjectName(QString::fromUtf8("lb_password_login"));
        lb_password_login->setGeometry(QRect(10, 100, 31, 21));
        le_tel_login = new QLineEdit(pg_login);
        le_tel_login->setObjectName(QString::fromUtf8("le_tel_login"));
        le_tel_login->setGeometry(QRect(50, 70, 113, 20));
        le_password_login = new QLineEdit(pg_login);
        le_password_login->setObjectName(QString::fromUtf8("le_password_login"));
        le_password_login->setGeometry(QRect(50, 100, 113, 20));
        le_password_login->setEchoMode(QLineEdit::Password);
        pb_clear_login = new QPushButton(pg_login);
        pb_clear_login->setObjectName(QString::fromUtf8("pb_clear_login"));
        pb_clear_login->setGeometry(QRect(49, 150, 51, 21));
        pb_commit_login = new QPushButton(pg_login);
        pb_commit_login->setObjectName(QString::fromUtf8("pb_commit_login"));
        pb_commit_login->setGeometry(QRect(110, 150, 51, 21));
        tb_page->addTab(pg_login, QString());
        pg_register = new QWidget();
        pg_register->setObjectName(QString::fromUtf8("pg_register"));
        lb_tel_register = new QLabel(pg_register);
        lb_tel_register->setObjectName(QString::fromUtf8("lb_tel_register"));
        lb_tel_register->setGeometry(QRect(30, 60, 31, 21));
        lb_password_register = new QLabel(pg_register);
        lb_password_register->setObjectName(QString::fromUtf8("lb_password_register"));
        lb_password_register->setGeometry(QRect(30, 90, 31, 20));
        lb_nicname_register = new QLabel(pg_register);
        lb_nicname_register->setObjectName(QString::fromUtf8("lb_nicname_register"));
        lb_nicname_register->setGeometry(QRect(30, 120, 31, 16));
        le_tel_register = new QLineEdit(pg_register);
        le_tel_register->setObjectName(QString::fromUtf8("le_tel_register"));
        le_tel_register->setGeometry(QRect(60, 60, 113, 20));
        le_password_register = new QLineEdit(pg_register);
        le_password_register->setObjectName(QString::fromUtf8("le_password_register"));
        le_password_register->setGeometry(QRect(60, 90, 113, 20));
        le_password_register->setEchoMode(QLineEdit::Password);
        le_nicname_register = new QLineEdit(pg_register);
        le_nicname_register->setObjectName(QString::fromUtf8("le_nicname_register"));
        le_nicname_register->setGeometry(QRect(60, 120, 113, 20));
        pb_clear_register = new QPushButton(pg_register);
        pb_clear_register->setObjectName(QString::fromUtf8("pb_clear_register"));
        pb_clear_register->setGeometry(QRect(60, 170, 51, 21));
        pb_commit_register = new QPushButton(pg_register);
        pb_commit_register->setObjectName(QString::fromUtf8("pb_commit_register"));
        pb_commit_register->setGeometry(QRect(120, 170, 51, 21));
        tb_page->addTab(pg_register, QString());
        lb_icon = new QLabel(LoginDialog);
        lb_icon->setObjectName(QString::fromUtf8("lb_icon"));
        lb_icon->setGeometry(QRect(0, 0, 161, 231));
        lb_icon->setPixmap(QPixmap(QString::fromUtf8(":/images/register.png")));
        lb_icon->setScaledContents(true);

        retranslateUi(LoginDialog);

        tb_page->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(LoginDialog);
    } // setupUi

    void retranslateUi(QDialog *LoginDialog)
    {
        LoginDialog->setWindowTitle(QApplication::translate("LoginDialog", "LoginDialog", nullptr));
#ifndef QT_NO_TOOLTIP
        tb_page->setToolTip(QApplication::translate("LoginDialog", "<html><head/><body><p>\347\231\273\345\275\225</p></body></html>", nullptr));
#endif // QT_NO_TOOLTIP
        lb_tel_login->setText(QApplication::translate("LoginDialog", "\346\211\213\346\234\272\345\217\267", nullptr));
        lb_password_login->setText(QApplication::translate("LoginDialog", "\345\257\206\347\240\201", nullptr));
        le_tel_login->setText(QApplication::translate("LoginDialog", "12345678912", nullptr));
        le_password_login->setText(QApplication::translate("LoginDialog", "222", nullptr));
        pb_clear_login->setText(QApplication::translate("LoginDialog", "\346\270\205\347\251\272", nullptr));
        pb_commit_login->setText(QApplication::translate("LoginDialog", "\346\217\220\344\272\244", nullptr));
        tb_page->setTabText(tb_page->indexOf(pg_login), QApplication::translate("LoginDialog", "\347\231\273\345\275\225", nullptr));
        lb_tel_register->setText(QApplication::translate("LoginDialog", "\346\211\213\346\234\272\345\217\267", nullptr));
        lb_password_register->setText(QApplication::translate("LoginDialog", "\345\257\206\347\240\201", nullptr));
        lb_nicname_register->setText(QApplication::translate("LoginDialog", "\346\230\265\347\247\260", nullptr));
        pb_clear_register->setText(QApplication::translate("LoginDialog", "\346\270\205\347\251\272", nullptr));
        pb_commit_register->setText(QApplication::translate("LoginDialog", "\346\217\220\344\272\244", nullptr));
        tb_page->setTabText(tb_page->indexOf(pg_register), QApplication::translate("LoginDialog", "\346\263\250\345\206\214", nullptr));
        lb_icon->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class LoginDialog: public Ui_LoginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINDIALOG_H
