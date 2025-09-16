// opusdecoder.h
#pragma once
#include <QAudioOutput>
#include <QIODevice>
#include <QObject>

#include "opus.h"

class OpusDecoderWrapper : public QObject {
    Q_OBJECT
public:
    OpusDecoderWrapper(QObject* parent = nullptr);
    ~OpusDecoderWrapper();

public slots:
    // 接收压缩包
    void playPacket(const QByteArray &packet);

private:
    QAudioOutput*    m_audioOut = nullptr;
    QIODevice*       m_outIo = nullptr;
    OpusDecoder*     m_decoder = nullptr;

    const int        m_sampleRate = 48000;
    const int        m_channels   = 1;
    const int        m_frameSize  = 960;
    QByteArray       m_pcmBuffer;    // 解码后 PCM
};
