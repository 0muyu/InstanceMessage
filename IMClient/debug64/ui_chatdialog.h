/********************************************************************************
** Form generated from reading UI file 'chatdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATDIALOG_H
#define UI_CHATDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_ChatDialog
{
public:
    QTextBrowser *tb_chat;
    QGroupBox *groupBox;
    QPushButton *pb_tool1;
    QPushButton *pb_tool2;
    QPushButton *pb_tool3;
    QPushButton *pb_send;
    QTextEdit *te_chat;

    void setupUi(QDialog *ChatDialog)
    {
        if (ChatDialog->objectName().isEmpty())
            ChatDialog->setObjectName(QString::fromUtf8("ChatDialog"));
        ChatDialog->resize(405, 375);
        ChatDialog->setMinimumSize(QSize(30, 0));
        tb_chat = new QTextBrowser(ChatDialog);
        tb_chat->setObjectName(QString::fromUtf8("tb_chat"));
        tb_chat->setGeometry(QRect(0, 0, 401, 201));
        groupBox = new QGroupBox(ChatDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(0, 200, 131, 41));
        groupBox->setMinimumSize(QSize(30, 0));
        groupBox->setMaximumSize(QSize(1677215, 16777215));
        pb_tool1 = new QPushButton(groupBox);
        pb_tool1->setObjectName(QString::fromUtf8("pb_tool1"));
        pb_tool1->setGeometry(QRect(0, 10, 30, 30));
        pb_tool1->setMinimumSize(QSize(30, 30));
        pb_tool1->setMaximumSize(QSize(30, 30));
        pb_tool2 = new QPushButton(groupBox);
        pb_tool2->setObjectName(QString::fromUtf8("pb_tool2"));
        pb_tool2->setGeometry(QRect(50, 10, 30, 30));
        pb_tool2->setMinimumSize(QSize(30, 30));
        pb_tool2->setMaximumSize(QSize(30, 30));
        pb_tool3 = new QPushButton(groupBox);
        pb_tool3->setObjectName(QString::fromUtf8("pb_tool3"));
        pb_tool3->setGeometry(QRect(100, 10, 30, 30));
        pb_tool3->setMinimumSize(QSize(30, 0));
        pb_send = new QPushButton(ChatDialog);
        pb_send->setObjectName(QString::fromUtf8("pb_send"));
        pb_send->setGeometry(QRect(340, 350, 61, 21));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        pb_send->setFont(font);
        te_chat = new QTextEdit(ChatDialog);
        te_chat->setObjectName(QString::fromUtf8("te_chat"));
        te_chat->setGeometry(QRect(0, 240, 401, 91));

        retranslateUi(ChatDialog);

        QMetaObject::connectSlotsByName(ChatDialog);
    } // setupUi

    void retranslateUi(QDialog *ChatDialog)
    {
        ChatDialog->setWindowTitle(QApplication::translate("ChatDialog", "Dialog", nullptr));
        groupBox->setTitle(QString());
        pb_tool1->setText(QString());
        pb_tool2->setText(QString());
        pb_tool3->setText(QString());
        pb_send->setText(QApplication::translate("ChatDialog", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChatDialog: public Ui_ChatDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATDIALOG_H
