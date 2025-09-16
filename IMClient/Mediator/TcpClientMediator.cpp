#include"TcpClientMediator.h"
#include"../Net/TcpClient.h"

TcpClientMediator::TcpClientMediator() { 
    m_pNet = new TcpClient(this); }
TcpClientMediator::~TcpClientMediator() {
  if (m_pNet) {
    m_pNet->unInitNet();
    delete m_pNet;
    m_pNet = nullptr;
  }
}

// 1.打开网络
bool TcpClientMediator::openNet() { return m_pNet->initNet(); }

// 2.关闭网络
void TcpClientMediator::closeNet() { m_pNet->unInitNet(); }

// 3.发送数据
bool TcpClientMediator::sendData(char* data, int len, unsigned long to) {
  
  return m_pNet->sendData(data,len,to);
}

// 4.转发数据给核心处理类

void TcpClientMediator::transmitData(char* data, int len, unsigned long from) {
  // TOD0:转发数据给核心处理类
  Q_EMIT sig_transmitData( data, len, from );

  //测试代码:打印接收到的数据
  //  cout << "TcpClientMediator::transmitData:" << data << "  "
  //       << "len:" << len << '\n';
}
