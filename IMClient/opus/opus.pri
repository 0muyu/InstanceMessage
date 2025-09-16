
QT += multimedia

# ┌───────────────────────────────────────────┐
# │                opus.pri                   │
# └───────────────────────────────────────────┘

# 根目录就是当前 pri 所在的目录
OPUS_ROOT = $$PWD

# 1) include 搜索路径
INCLUDEPATH += $$OPUS_ROOT/include

# 2) 链接库目录
LIBS += -L$$OPUS_ROOT/lib -lopus

# （如果你更喜欢直接用 .a 文件，也可写成：）
#   LIBS += $$OPUS_ROOT/lib/libopus.a

# Windows 下还可能需要的系统库
#win32:LIBS += -lws2_32
win32 {
    # 把 stack-protector 运行时也拉进来
    LIBS += -lssp_nonshared -lssp
    LIBS += -lgcc_s
}

# 3) 自动添加 OpusEncoder/OpusDecoder 的源和头
HEADERS += \
    $$OPUS_ROOT/opusencoder.h \
    $$OPUS_ROOT/opusdecoder.h

SOURCES += \
    $$OPUS_ROOT/opusencoder.cpp \
    $$OPUS_ROOT/opusdecoder.cpp

# 4) （可选）如果你想把 include/lib 都装到 pkgconfig 里
# CONFIG += pkgconfig
# PKGCONFIG += $$OPUS_ROOT/lib/pkgconfig/opus


#QT += multimedia

#LIBS += $$PWD/lib/libopus.a
#LIBS += $$PWD/lib/libopus.la

## 如果是动态链接 .dll，改为：
## LIBS += -L$$PWD/opus/lib -lopus


##INCLUDEPATH += ./

#HEADERS += \
#    $$PWD/opusdecoder.h \
#    $$PWD/opusencoder.h

#SOURCES += \
#    $$PWD/opusdecoder.cpp \
#    $$PWD/opusencoder.cpp

