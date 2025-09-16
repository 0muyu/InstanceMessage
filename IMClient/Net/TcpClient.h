#pragma once
#include"INet.h"


class TcpClient :public INet{
 public:

  TcpClient(INetMediator* pTcpClientMediator);
 ~TcpClient();

   // 1.初始化网络
 bool initNet();

 // 2.关闭网络
 void unInitNet();

 // 3.发送数据
bool sendData(char* data, int len, unsigned long to);

 // 4.接收数据
 void recvData();

 //（5）接收数据的线程函数
 static unsigned _stdcall TcpClientRecvThread(void* IpVoid);

 private:
 HANDLE m_TcpClientHandle;
};

