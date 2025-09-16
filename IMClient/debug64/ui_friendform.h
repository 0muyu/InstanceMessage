/********************************************************************************
** Form generated from reading UI file 'friendform.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIENDFORM_H
#define UI_FRIENDFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FriendForm
{
public:
    QPushButton *pb_icon;
    QLabel *lb_name;
    QLabel *lb_feeling;

    void setupUi(QWidget *FriendForm)
    {
        if (FriendForm->objectName().isEmpty())
            FriendForm->setObjectName(QString::fromUtf8("FriendForm"));
        FriendForm->resize(340, 80);
        FriendForm->setMinimumSize(QSize(340, 80));
        FriendForm->setMaximumSize(QSize(340, 80));
        pb_icon = new QPushButton(FriendForm);
        pb_icon->setObjectName(QString::fromUtf8("pb_icon"));
        pb_icon->setGeometry(QRect(0, 10, 60, 60));
        pb_icon->setMinimumSize(QSize(60, 0));
        pb_icon->setIconSize(QSize(60, 60));
        pb_icon->setFlat(true);
        lb_name = new QLabel(FriendForm);
        lb_name->setObjectName(QString::fromUtf8("lb_name"));
        lb_name->setGeometry(QRect(80, 6, 100, 30));
        QFont font;
        font.setPointSize(12);
        lb_name->setFont(font);
        lb_feeling = new QLabel(FriendForm);
        lb_feeling->setObjectName(QString::fromUtf8("lb_feeling"));
        lb_feeling->setGeometry(QRect(80, 40, 120, 30));
        lb_feeling->setFont(font);

        retranslateUi(FriendForm);

        QMetaObject::connectSlotsByName(FriendForm);
    } // setupUi

    void retranslateUi(QWidget *FriendForm)
    {
        FriendForm->setWindowTitle(QApplication::translate("FriendForm", "Form", nullptr));
        pb_icon->setText(QString());
        lb_name->setText(QApplication::translate("FriendForm", "mmm", nullptr));
        lb_feeling->setText(QApplication::translate("FriendForm", "12", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FriendForm: public Ui_FriendForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIENDFORM_H
