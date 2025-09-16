QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS+=-lWs2_32

# Opus include 路径
INCLUDEPATH += $$PWD/opus/

include(./opus/opus.pri)


SOURCES += \
    Mediator/INetMediator.cpp \
    Mediator/TcpClientMediator.cpp \
    Mediator/TcpServerMediator.cpp \
    Mediator/UdpMediator.cpp \
    Net/TcpClient.cpp \
    Net/TcpServer.cpp \
    Net/Udp.cpp \
    chatdialog.cpp \
    friendlist.cpp \
    ckernel.cpp \
    friendform.cpp \
    main.cpp \
    logindialog.cpp

HEADERS += \
    Mediator/INetMediator.h \
    Mediator/TcpClientMediator.h \
    Mediator/TcpServerMediator.h \
    Mediator/UdpMediator.h \
    Net/INet.h \
    Net/TcpClient.h \
    Net/TcpServer.h \
    Net/Udp.h \
    Net/config.h \
    chatdialog.h \
    friendlist.h \
    ckernel.h \
    friendform.h \
    logindialog.h

FORMS += \
    chatdialog.ui \
    friendlist.ui \
    friendform.ui \
    logindialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
