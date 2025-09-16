#ifndef CKERNEL_H
#define CKERNEL_H

#include <QMap>
#include <QObject>

#include "Mediator/INetMediator.h"
#include "Net/config.h"
#include "chatdialog.h"
#include "friendlist.h"
#include "logindialog.h"
class CKernel;
typedef void ( CKernel::*PFUN )( char*, int, unsigned long );

class CKernel : public QObject
{
  Q_OBJECT

 public:
  explicit CKernel(QObject *parent = nullptr);
  ~CKernel();
  //转码函数
  // 1.gb2312转为utf-8
  QString gb2312ToUtf8( char* src );

  // 2.utf-8转为gb2312
  void utf8ToGb2312( QString src, char* dst, int len );

  // 1.初始化函数地址数组
  void initFunArr();

  // 1)处理注册回复
  void dealRegisterRs( char* data, int len, unsigned long from );

  // 2）处理登录回复
  void dealLoginRs( char* data, int len, unsigned long from );

  // 3)处理用户信息
  void dealUserInfo( char* data, int len, unsigned long from );

  // 4）处理好友下线请求
  void dealOfflineRq( char* data, int len, unsigned long from );

  // 5）处理聊天请求（当前是用户B,收到好友A的聊天请求）
  void dealChatRq( char* data, int len, unsigned long from );

  // 6）处理聊天回复（当前是用户A,好友B不在线）
  void dealChatRs( char* data, int len, unsigned long from );

  // 7）处理添加好友请求（当前是用户B,收到好友A的添加好友请求）
  void dealAddFriendRq( char* data, int len, unsigned long from );

  // 8）处理添加好友回复（当前是用户A,用户B不存在或不在线）
  void dealAddFriendRs( char* data, int len, unsigned long from );

 signals:
 public slots:
  //处理分发所有接收到的数据
  void slot_transmitData( char* data, int len, unsigned long from );

  // 1)处理注册信息，发送给服务端
  void slot_registerMassage( QString name, QString tel, QString password );

  // 2)处理登录信息，发送给服务端
  void slot_loginMassage( QString tel, QString password );

  // 3）处理LoginDiolog退出进程的信号
  void slot_closeProcess();

  // 4)处理下线信号
  void slot_offlien();

  // 5) 处理显示与当前好友的聊天窗口
  void slot_showChatDialog( int friendId );

  // 6)处理由Dialog发送来的聊天内容,将聊天内容发送给服务器
  void slot_sendMessage( int friendId, QString content );

  // 7)处理添加由FriendList发送的好友的信号
  void slot_addFriend();

 private:
  INetMediator* m_pMediator;

  //登录&注册界面
  LoginDialog* m_pLoginDlg;

  //好友列表界面
  FriendList* m_pFriendList;

  //保存函数地址的数组
  PFUN m_funArrry[ _DEF_ARRRY_FUN_LENGTH ];

  //当前登录用户的id
  int m_userId;

  //当前登录用户的昵称
  QString m_Username;

  //保存好友的id和好友对象
  QMap<int, FriendForm*> m_mapIdtoForm;

  //保存好友id和与好友的聊天窗口
  QMap<int, ChatDialog*> m_mapIdToDialog;
};

#endif // CKERNEL_H
