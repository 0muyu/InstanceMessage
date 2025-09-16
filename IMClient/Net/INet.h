#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <process.h>

#include <iostream>
#include <list>

#include "config.h"

using namespace std;

//#pragma comment(lib, "Ws2_32.lib")
#include"../Mediator/INetMediator.h"

class INetMediator;
class INet {
 public:
   //构造函数可以在父类（抽象类）中实现是因为多态性，可以通过定义父类对象new一个子类对象
  INet() : m_sock(INVALID_SOCKET), m_bRunning(true),m_pMediator(nullptr) {}
   //析构函数对于不同的子类处理不同，需要在子类中自行实现
   virtual ~INet(){};

  //1.初始化网络
    virtual bool initNet() = 0;


  //2.关闭网络
    virtual void unInitNet() = 0;

  //3.发送数据
  // char* data:数据内容
  // int len:数据长度
  // unsigned long to:数据发给谁
  // TCP协议，SOCKET决定了发给谁，SOCKET实际是UINTPTR类型
  // UDP协议，IP地址决定了发给谁，IP是ULONG类型
   virtual bool sendData(char* data,int len,unsigned long to) = 0;


  //4.接收数据
  virtual void recvData() = 0;


  protected:
  SOCKET m_sock;
   bool m_bRunning;
  INetMediator* m_pMediator;
};
