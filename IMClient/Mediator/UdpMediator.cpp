#include"UdpMediator.h"
#include"../Net/Udp.h"

UdpMediator ::UdpMediator() { 
    m_pNet = new Udp(this); }

UdpMediator ::~UdpMediator() {
  if (m_pNet) {
    m_pNet->unInitNet();
    delete m_pNet;
    m_pNet = nullptr;
  }
}

// 1.打开网络
bool UdpMediator ::openNet() { return m_pNet->initNet(); }

// 2.关闭网络
void UdpMediator ::closeNet() { m_pNet->unInitNet(); }

// 3.发送数据
bool UdpMediator ::sendData(char* data, int len, unsigned long to) {
   
  return m_pNet->sendData(data,len,to);
}

// 4.转发数据给核心处理类
void UdpMediator ::transmitData(char* data, int len, unsigned long from) {
    // TOD0:转发数据给核心处理类
    //测试代码:打印接收到的数据

  cout << "UdpMediator::transmitData:" << data << '\n';

}