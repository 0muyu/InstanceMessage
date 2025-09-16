#include"TcpServerMediator.h"
#include"../Net/TcpServer.h"

TcpServerMediator::TcpServerMediator() { m_pNet = new TcpServer(this); }

TcpServerMediator::~TcpServerMediator() {
  if (m_pNet) {
    m_pNet->unInitNet();
    delete m_pNet;
    m_pNet = nullptr;
  }
}

  // 1.打开网络
bool TcpServerMediator::openNet() { return m_pNet->initNet(); }

// 2.关闭网络
void TcpServerMediator::closeNet() { m_pNet->unInitNet(); }

// 3.发送数据
bool TcpServerMediator::sendData(char* data, int len, unsigned long to) {
 
  return m_pNet->sendData(data, len, to);
}

// 4.转发数据给核心处理类
void TcpServerMediator::transmitData(char* data, int len, unsigned long from) {
  // TOD0:转发数据给核心处理类
  //测试代码:打印接收到的数据
  cout << "TcpServerMediator::transmitData:" << data << "  "
       << "len:" << len << '\n';
}