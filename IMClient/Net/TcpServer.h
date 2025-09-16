#pragma once
#include"INet.h"
#include<unordered_map>
class TcpServer:public INet {
 public:
  TcpServer(INetMediator* pTcpServerMediator);
  ~TcpServer();

  // 1.初始化网络
  bool initNet();

  // 2.关闭网络
  void unInitNet();

  // 3.发送数据
  bool sendData(char* data, int len, unsigned long to);

  // 4.接收数据
  void recvData();

  //(5)接受连接的线程函数
  static unsigned _stdcall acceptThread(void* IpVoid);

  //(6)接收数据的线程函数
  static unsigned _stdcall recvThread(void* IpVoid);

  private:

  //接收连接的线程（1个）+接收数据的线程（n个） ://保存所有的线程句柄
   list<HANDLE> m_listHandle;
   //保存连接成功的客户端的接收数据的线程id和socket
   unordered_map<unsigned int , SOCKET> m_mapHandidToSocket;
};

