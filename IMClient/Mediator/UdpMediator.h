#pragma once
#include"INetMediator.h"

class UdpMediator:public INetMediator  {
 public:
  UdpMediator ();
  ~UdpMediator ();

  // 1.打开网络
  bool openNet();

  // 2.关闭网络
  void closeNet();

  // 3.发送数据
  bool sendData(char* data, int len, unsigned long to);

  // 4.转发数据给核心处理类

  void transmitData(char* data, int len, unsigned long from);
 private:
};

