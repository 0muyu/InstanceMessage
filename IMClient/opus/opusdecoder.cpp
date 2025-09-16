// opusdecoder.cpp
#include "opusdecoder.h"
#include <QAudioFormat>
#include <QAudioDeviceInfo>

OpusDecoderWrapper::OpusDecoderWrapper(QObject* parent) : QObject(parent), m_pcmBuffer(m_frameSize * m_channels * sizeof(short), Qt::Uninitialized) {
  int err;
  m_decoder = opus_decoder_create(m_sampleRate, m_channels, &err);
  if (err != OPUS_OK) {
    qFatal("Opus decoder create failed: %s", opus_strerror(err));
  }

  QAudioFormat fmt;
  fmt.setSampleRate(m_sampleRate);
  fmt.setChannelCount(m_channels);
  fmt.setSampleSize(16);
  fmt.setCodec("audio/pcm");
  fmt.setByteOrder(QAudioFormat::LittleEndian);
  fmt.setSampleType(QAudioFormat::SignedInt);

  QAudioDeviceInfo info = QAudioDeviceInfo::defaultOutputDevice( );
  if (!info.isFormatSupported(fmt)) {
    fmt = info.nearestFormat(fmt);
  }

  m_audioOut = new QAudioOutput(info, fmt, this);
  m_outIo    = m_audioOut->start( );
}

OpusDecoderWrapper::~OpusDecoderWrapper() {
    if (m_audioOut) {
        m_audioOut->stop();
        delete m_audioOut;
    }
    if (m_decoder) opus_decoder_destroy(m_decoder);
}

void OpusDecoderWrapper::playPacket(const QByteArray& packet) {
  const unsigned char* data       = reinterpret_cast<const unsigned char*>(packet.constData( ));
  int                  outSamples = opus_decode(m_decoder, data, packet.size( ), reinterpret_cast<opus_int16*>(m_pcmBuffer.data( )), m_frameSize, 0);

  if (outSamples < 0) {
    qWarning("Opus decode failed: %s", opus_strerror(outSamples));
    return;
  }
  // 写入 QAudioOutput 播放
  m_outIo->write(m_pcmBuffer.constData( ), outSamples * m_channels * sizeof(short));
}
