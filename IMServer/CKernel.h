#pragma once

#include "IOCPTcpServer/IOCPTcpServer.h"
#include "MySQL/SqlManager.h"
#include "config.h"



class Ckernel {
    using PFUN=void (Ckernel::*)(char*, int, unsigned long);
 public:
  //static Ckernel* pKernel;  //Ϊ�����н������е��ú��Ĵ�����ĺ���
  //�ṩ��̬�ġ����еĻ�ȡ���еķ���
  static Ckernel& instance();

   //����ɾ����
  struct deleter {
    void operator()(Ckernel* kernel) { delete kernel; }
  };


  private:
   //��������������˽�л�
  Ckernel()=default;
  ~Ckernel()=default;
  //���ÿ�����ֵ �Ϳ�����ֵ������
  Ckernel(const Ckernel& obj)=delete;
  Ckernel& operator=(const Ckernel& obj)=delete; 




  public:

  // 1.�򿪷�����
   bool startServer(IOCPTcpServer& netServer);

  // 2.�رշ�����
  void closeServer();

  // 3.����ַ����н��յ�������
  void dealData(char* data, int len, unsigned long from);

  // 4.�Ѵ������ĵ�ַ���浽������
  void initFunArrry();

  // 5.��ȡ��ǰ��¼�û�����Ϣ�ͺ�����Ϣ
  void getUserInfoAndFriendInfo(int id);

  // 6.����id��ѯ��Ϣ
  // info����Ϊ�������
  void getInfoById(int id, _STRU_USER_INFO* info);

  // 1)ע������Ĵ�����
  void dealRegisterRq(char* data, int len, unsigned long from);

  // 2)��¼����Ĵ�����
  void dealLoginRq(char* data, int len, unsigned long from);

  // 3��������������
  void dealOfflineRq(char* data, int len, unsigned long from);

  // 4��������������
  void dealChatRq(char* data, int len, unsigned long from);

  // 5��������Ӻ�������
  void dealAddFriendRq(char* data, int len, unsigned long from);

  // 6)������Ӻ��ѵĻظ�
  void dealAddFriendRs(char* data, int len, unsigned long from);

 private:
  static unique_ptr<Ckernel, deleter> m_uptr;//unique_ptr
  static std::mutex m_mutex;//��

  IOCPTcpServer* m_iocpServer;

  //���洦�����ĵ�ַ
  PFUN m_arrFun[_DEF_ARRRY_FUN_LENGTH];


  //�洢��¼�ɹ��û���id��SOCKET(��¼�ɹ�ʱ��Ҫ��ӣ�����ʱ��Ҫɾ��)
  unordered_map<int, SOCKET> m_mapIdToSocket;
};
