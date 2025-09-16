// opusencoder.cpp
#include "opusencoder.h"

#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QDebug>

OpusEncoderWrapper::OpusEncoderWrapper(QObject* parent) : QObject(parent), m_buffer(m_frameSize * m_channels * sizeof(short), Qt::Uninitialized) {
  int err;
  m_encoder = opus_encoder_create(m_sampleRate, m_channels, OPUS_APPLICATION_AUDIO, &err);
  if (err != OPUS_OK) {
    qFatal("Opus encoder create failed: %s", opus_strerror(err));
  }
}

OpusEncoderWrapper::~OpusEncoderWrapper() {
    stop();
    if (m_encoder) opus_encoder_destroy(m_encoder);
}

void OpusEncoderWrapper::start() {
    QAudioFormat fmt;
    fmt.setSampleRate(m_sampleRate);
    fmt.setChannelCount(m_channels);
    fmt.setSampleSize(16);
    fmt.setCodec("audio/pcm");
    fmt.setByteOrder(QAudioFormat::LittleEndian);
    fmt.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(fmt)) {
        fmt = info.nearestFormat(fmt);
    }

    m_audioIn = new QAudioInput(info, fmt, this);
    m_io = m_audioIn->start();
    connect(m_io, &QIODevice::readyRead, this, &OpusEncoderWrapper::handleReadyRead);
}

void OpusEncoderWrapper::stop() {
    if (m_audioIn) {
        m_audioIn->stop();
        delete m_audioIn;
        m_audioIn = nullptr;
    }
    m_io = nullptr;
}

void OpusEncoderWrapper::handleReadyRead() {
  while (m_audioIn->bytesReady( ) >= m_buffer.size( )) {
    qint64 len = m_io->read(m_buffer.data( ), m_buffer.size( ));
    if (len != m_buffer.size( ))
      break;

    // PCM 数据指针
    const opus_int16* pcm = reinterpret_cast<const opus_int16*>(m_buffer.constData( ));
    unsigned char     outbuf[4000];
    int               nbBytes = opus_encode(m_encoder, pcm, m_frameSize, outbuf, m_maxDataBytes);
    if (nbBytes < 0) {
      qWarning("Opus encoding failed: %s", opus_strerror(nbBytes));
      continue;
    }
    // 发射压缩包
    QByteArray packet(reinterpret_cast<char*>(outbuf), nbBytes);
    emit       packetReady(packet);
  }
}
