/********************************************************************************
** Form generated from reading UI file 'friendlist.ui'
**
** Created by: Qt User Interface Compiler version 5.12.11
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIENDLIST_H
#define UI_FRIENDLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FriendList
{
public:
    QPushButton *pb_icon;
    QLabel *lb_name;
    QLabel *lb_feeling;
    QPushButton *pb_menu;
    QTabWidget *tw_page;
    QWidget *page_message;
    QWidget *page_friend;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout;
    QWidget *wdg_list;
    QSpacerItem *verticalSpacer;
    QWidget *page_sapce;
    QPushButton *pb_tool1;
    QPushButton *pb_tool2;
    QPushButton *pb_tool3;

    void setupUi(QDialog *FriendList)
    {
        if (FriendList->objectName().isEmpty())
            FriendList->setObjectName(QString::fromUtf8("FriendList"));
        FriendList->resize(307, 456);
        FriendList->setMinimumSize(QSize(60, 60));
        pb_icon = new QPushButton(FriendList);
        pb_icon->setObjectName(QString::fromUtf8("pb_icon"));
        pb_icon->setGeometry(QRect(0, 0, 60, 60));
        pb_icon->setMinimumSize(QSize(60, 60));
        pb_icon->setIconSize(QSize(60, 60));
        pb_icon->setAutoDefault(false);
        pb_icon->setFlat(true);
        lb_name = new QLabel(FriendList);
        lb_name->setObjectName(QString::fromUtf8("lb_name"));
        lb_name->setGeometry(QRect(70, 0, 101, 21));
        lb_name->setMinimumSize(QSize(30, 20));
        QFont font;
        font.setPointSize(15);
        lb_name->setFont(font);
        lb_feeling = new QLabel(FriendList);
        lb_feeling->setObjectName(QString::fromUtf8("lb_feeling"));
        lb_feeling->setGeometry(QRect(70, 40, 91, 20));
        lb_feeling->setMinimumSize(QSize(30, 20));
        QFont font1;
        font1.setPointSize(9);
        lb_feeling->setFont(font1);
        pb_menu = new QPushButton(FriendList);
        pb_menu->setObjectName(QString::fromUtf8("pb_menu"));
        pb_menu->setGeometry(QRect(10, 410, 40, 40));
        pb_menu->setMinimumSize(QSize(40, 40));
        tw_page = new QTabWidget(FriendList);
        tw_page->setObjectName(QString::fromUtf8("tw_page"));
        tw_page->setGeometry(QRect(0, 60, 301, 341));
        page_message = new QWidget();
        page_message->setObjectName(QString::fromUtf8("page_message"));
        tw_page->addTab(page_message, QString());
        page_friend = new QWidget();
        page_friend->setObjectName(QString::fromUtf8("page_friend"));
        scrollArea = new QScrollArea(page_friend);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(0, 0, 301, 321));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 299, 319));
        verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        wdg_list = new QWidget(scrollAreaWidgetContents);
        wdg_list->setObjectName(QString::fromUtf8("wdg_list"));

        verticalLayout->addWidget(wdg_list);

        verticalSpacer = new QSpacerItem(58, 280, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        scrollArea->setWidget(scrollAreaWidgetContents);
        tw_page->addTab(page_friend, QString());
        page_sapce = new QWidget();
        page_sapce->setObjectName(QString::fromUtf8("page_sapce"));
        tw_page->addTab(page_sapce, QString());
        pb_tool1 = new QPushButton(FriendList);
        pb_tool1->setObjectName(QString::fromUtf8("pb_tool1"));
        pb_tool1->setGeometry(QRect(150, 410, 40, 40));
        pb_tool1->setMinimumSize(QSize(40, 40));
        pb_tool2 = new QPushButton(FriendList);
        pb_tool2->setObjectName(QString::fromUtf8("pb_tool2"));
        pb_tool2->setGeometry(QRect(210, 410, 40, 40));
        pb_tool2->setMinimumSize(QSize(40, 40));
        pb_tool3 = new QPushButton(FriendList);
        pb_tool3->setObjectName(QString::fromUtf8("pb_tool3"));
        pb_tool3->setGeometry(QRect(260, 410, 41, 40));
        pb_tool3->setMinimumSize(QSize(40, 40));

        retranslateUi(FriendList);

        tw_page->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(FriendList);
    } // setupUi

    void retranslateUi(QDialog *FriendList)
    {
        FriendList->setWindowTitle(QApplication::translate("FriendList", "Dialog", nullptr));
        pb_icon->setText(QString());
        lb_name->setText(QApplication::translate("FriendList", "muyu", nullptr));
        lb_feeling->setText(QApplication::translate("FriendList", "\345\271\262\351\245\255\344\270\255...", nullptr));
        pb_menu->setText(QString());
        tw_page->setTabText(tw_page->indexOf(page_message), QApplication::translate("FriendList", "\346\266\210\346\201\257", nullptr));
        tw_page->setTabText(tw_page->indexOf(page_friend), QApplication::translate("FriendList", "\350\201\224\347\263\273\344\272\272", nullptr));
        tw_page->setTabText(tw_page->indexOf(page_sapce), QApplication::translate("FriendList", "\347\251\272\351\227\264", nullptr));
        pb_tool1->setText(QString());
        pb_tool2->setText(QString());
        pb_tool3->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class FriendList: public Ui_FriendList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIENDLIST_H
