#pragma once

#include "IOCPTcpServer/IOCPTcpServer.h"
#include "MySQL/SqlManager.h"
#include "config.h"



class Ckernel {
    using PFUN=void (Ckernel::*)(char*, int, unsigned long);
 public:
  //static Ckernel* pKernel;  //为了在中介者类中调用核心处理类的函数
  //提供静态的、公有的获取单列的方法
  static Ckernel& instance();

   //定义删除器
  struct deleter {
    void operator()(Ckernel* kernel) { delete kernel; }
  };


  private:
   //将构造析构函数私有化
  Ckernel()=default;
  ~Ckernel()=default;
  //禁用拷贝赋值 和拷贝赋值操作符
  Ckernel(const Ckernel& obj)=delete;
  Ckernel& operator=(const Ckernel& obj)=delete; 




  public:

  // 1.打开服务器
   bool startServer(IOCPTcpServer& netServer);

  // 2.关闭服务器
  void closeServer();

  // 3.处理分发所有接收到的数据
  void dealData(char* data, int len, unsigned long from);

  // 4.把处理函数的地址保存到数组中
  void initFunArrry();

  // 5.获取当前登录用户的信息和好友信息
  void getUserInfoAndFriendInfo(int id);

  // 6.根据id查询信息
  // info是作为输出参数
  void getInfoById(int id, _STRU_USER_INFO* info);

  // 1)注册请求的处理函数
  void dealRegisterRq(char* data, int len, unsigned long from);

  // 2)登录请求的处理函数
  void dealLoginRq(char* data, int len, unsigned long from);

  // 3）处理下线请求
  void dealOfflineRq(char* data, int len, unsigned long from);

  // 4）处理聊天请求
  void dealChatRq(char* data, int len, unsigned long from);

  // 5）处理添加好友请求
  void dealAddFriendRq(char* data, int len, unsigned long from);

  // 6)处理添加好友的回复
  void dealAddFriendRs(char* data, int len, unsigned long from);

 private:
  static unique_ptr<Ckernel, deleter> m_uptr;//unique_ptr
  static std::mutex m_mutex;//锁

  IOCPTcpServer* m_iocpServer;

  //保存处理函数的地址
  PFUN m_arrFun[_DEF_ARRRY_FUN_LENGTH];


  //存储登录成功用户的id和SOCKET(登录成功时需要添加，下线时需要删除)
  unordered_map<int, SOCKET> m_mapIdToSocket;
};
