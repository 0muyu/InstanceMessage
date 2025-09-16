// opusencoder.h
#pragma once
#include <QAudioInput>
#include <QIODevice>
#include <QObject>

#include "opus.h"

class OpusEncoderWrapper : public QObject {
    Q_OBJECT
public:
    OpusEncoderWrapper(QObject* parent = nullptr);
    ~OpusEncoderWrapper();

    // 启动音频捕获与编码
    void start();
    void stop();

signals:
    // 发射压缩好的 opus 包
    void packetReady(const QByteArray &packet);

private slots:
    void handleReadyRead();

private:
    QAudioInput*     m_audioIn = nullptr;
    QIODevice*       m_io = nullptr;
    OpusEncoder*     m_encoder = nullptr;

    const int        m_sampleRate = 48000;
    const int        m_channels   = 1;
    const int        m_frameSize  = 960;   // 20ms @ 48kHz
    const int        m_maxDataBytes = 4000; // 容量上限
    QByteArray       m_buffer;  // 用来读 PCM
};
