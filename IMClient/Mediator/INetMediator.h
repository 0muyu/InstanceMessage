#pragma once
#include <QObject>
/*中介者类(转发数据给核心处理类、发送数据、打开网络、关闭网络)
*/
// 先声明有INet这个类，先让编译器用这个类去定义指针，一会就编译到了
class INet;
class INetMediator : public QObject {
  Q_OBJECT
 signals:
  void sig_transmitData( char* data, int len, unsigned long from );

 public:
  INetMediator();

  virtual ~INetMediator();

  // 1.打开网络
  virtual bool openNet() = 0;

  // 2.关闭网络
  virtual void closeNet() = 0;

  // 3.发送数据
  // char* data:数据内容
  // int len:数据长度
  // unsigned long
  // to:数据发给谁(TCP协议时to装的是socket，UDP协议的时候to装的是ip)//
  // TCP协议，SOCKET决定了发给谁，SOCKET实际是UINTPTR类型
  // UDP协议，IP地址决定了发给谁，IP是ULONG类型
  virtual bool sendData( char* data, int len, unsigned long to ) = 0;

  // 4.转发数据给核心处理类
  // char* data:数据内容
  // int len:数据长度
  // unsigned long
  // from:数据是从哪来的(TCP协议时to装的是socket，UDP协议的时候to装的是ip)
  virtual void transmitData( char* data, int len, unsigned long from ) = 0;

 protected:
  INet* m_pNet;
};
