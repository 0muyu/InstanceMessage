#include "ckernel.h"
//为了弹出窗口
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QTextCodec>
#include <QThread>

#include "Mediator/TcpClientMediator.h"
#include "Net/config.h"
#include "friendform.h"
CKernel::CKernel( QObject* parent ) : QObject( parent ), m_userId( 0 ) {
  initFunArr();

  // new一个好友列表界面的对象
  m_pFriendList = new FriendList;

  //连接下线信号和槽函数
  QObject::connect( m_pFriendList, &FriendList::sig_offlien, this,
                    &CKernel::slot_offlien );

  //连接FriendList发送的添加好友信号和槽函数
  connect( m_pFriendList, &FriendList::sig_addFriend, this,
           &CKernel::slot_addFriend );
  // new一个登录&注册界面
  m_pLoginDlg = new LoginDialog;
  //显示界面
  m_pLoginDlg->show();
  // 1）连接注册的信号和槽函数
  QObject::connect( m_pLoginDlg, &LoginDialog::sig_registerMassage, this,
                    &CKernel::slot_registerMassage );
  // 2）连接登录的信号和槽函数
  QObject::connect( m_pLoginDlg, &LoginDialog::sig_loginMassage, this,
                    &CKernel::slot_loginMassage );
  // 3)连接退出的信号和槽函数
  QObject::connect( m_pLoginDlg, &LoginDialog::sig_closeProcess, this,
                    &CKernel::slot_closeProcess );

  // new一个中介者类对象
  m_pMediator = new TcpClientMediator;
  QObject::connect( m_pMediator, &TcpClientMediator::sig_transmitData, this,
                    &CKernel::slot_transmitData );

  //打开网络
  if ( !m_pMediator->openNet() ) {
    QMessageBox::about( m_pLoginDlg, "提示", "网络连接失败！" );
    exit( -1 );
  }

  //测试代码:客户端给服务器发送一个消息
  //  _STRU_REGISTER_RQ rq;
  //  strcpy_s( rq.name, sizeof( rq.name ), "Lilly" );
  //  strcpy_s( rq.password, sizeof( rq.password ), "111111" );
  //  strcpy_s( rq.tel, sizeof( rq.tel ), "18225041358" );
  //  m_pMediator->sendData( (char*) &rq, sizeof( rq ), 666 );
}

CKernel::~CKernel() {
  if ( m_pMediator ) {
    m_pMediator->closeNet();
    delete m_pMediator;
    m_pMediator = nullptr;
  }

  if ( m_pLoginDlg ) {
    m_pLoginDlg->hide();

    delete m_pLoginDlg;
    m_pLoginDlg = nullptr;
  }

  if ( m_pFriendList ) {
    m_pFriendList->hide();
    delete m_pFriendList;
    m_pFriendList = nullptr;
  }
}

//转码函数
// 1.gb2312转为utf-8
QString CKernel::gb2312ToUtf8(
    char*
        src ) {  //为什么这里不需要传入长度，因为这是一个有效字符串，识别到'\0'时自动结束
  //使用bg2312方式生成一个QTextForName的对象
  QTextCodec* dc = QTextCodec::codecForName( "gb2312" );
  //从gb2312转为unicode编码方式
  return dc->toUnicode( src );
}

// 2.utf-8转为gb2312
void CKernel::utf8ToGb2312( QString src, char* dst, int len ) {
  //使用gb2312编码方式生成一个QTextCodec的对象
  QTextCodec* dc = QTextCodec ::codecForName( "gb2312" );

  //转为gb2312
  QByteArray ba = dc->fromUnicode( src );
  strcpy_s( dst, len, ba.data() );
}

// 1.初始化函数地址数组
void CKernel::initFunArr() {
  qDebug() << __func__;

  //把数组空间初始化为0
  memset( m_funArrry, 0, sizeof( m_funArrry ) );

  //将函数地址放入数组
  m_funArrry[ _DEF_REGISTER_RS - _DEF_PROTOCOL_BASE - 1 ] =
      &CKernel::dealRegisterRs;

  m_funArrry[ _DEF_LOGIN_RS - _DEF_PROTOCOL_BASE - 1 ] = &CKernel::dealLoginRs;
  m_funArrry[ _DEF_USER_INFO - _DEF_PROTOCOL_BASE - 1 ] =
      &CKernel::dealUserInfo;
  m_funArrry[ _DEF_OFFLINE_RQ - _DEF_PROTOCOL_BASE - 1 ] =
      &CKernel::dealOfflineRq;

  m_funArrry[ _DEF_CHAT_RQ - _DEF_PROTOCOL_BASE - 1 ] = &CKernel::dealChatRq;

  m_funArrry[ _DEF_CHAT_RS - _DEF_PROTOCOL_BASE - 1 ] = &CKernel::dealChatRs;

  m_funArrry[ _DEF_ADD_FRIEND_RQ - _DEF_PROTOCOL_BASE - 1 ] =
      &CKernel::dealAddFriendRq;

  m_funArrry[ _DEF_ADD_FRIEND_RS - _DEF_PROTOCOL_BASE - 1 ] =
      &CKernel::dealAddFriendRs;
}

// 1)处理注册回复
void CKernel::dealRegisterRs( char* data, int len, unsigned long from ) {
  qDebug() << __func__;
  // QThread::sleep( 5000 );
  // 1.拆包
  _STRU_REGISTER_RS* rs = (_STRU_REGISTER_RS*) data;

  // 2.根据注册结果提示用户
  switch ( rs->result ) {
    case _def_rigister_success:
      QMessageBox::about( m_pLoginDlg, "提示", "注册成功" );
      break;
    case _def_register_tel_exists:
      QMessageBox::about( m_pLoginDlg, "提示", "注册失败！该手机号已注册" );
      break;
    case _def_register_name_exists:
      QMessageBox::about( m_pLoginDlg, "提示", "注册失败！该昵称已存在" );
      break;
    default: break;
  }
}
// 2）处理登录回复
void CKernel::dealLoginRs( char* data, int len, unsigned long from ) {
  qDebug() << __func__;
  // 1.拆包
  _STRU_LOGIN_RS* rs = (_STRU_LOGIN_RS*) data;

  qDebug() << "id：" << rs->id;

  // 2.2、根据登录结果提示用户(成功的时候，隐藏登录界面)
  switch ( rs->result ) {
    case _def_login_tel_not_register:
      QMessageBox::about( m_pLoginDlg, "提示", "该手机号未注册！" );
      break;
    case _def_login_password_error:
      QMessageBox::about( m_pLoginDlg, "提示", "密码错误！" );
      break;
    case _def_login_success: {
      //赋值m_userId
      m_userId = rs->id;
      //页面跳转：隐藏登录界面，显示好友列表
      m_pLoginDlg->hide();
      m_pFriendList->show();
      //测试代码：
      //      for ( int i = 1; i < 6; i++ ) {
      //        // new 一个好友
      //        FriendForm* form = new FriendForm;
      //        //设置好友属性
      //        form->setFriendInfo( i, QString( "用户%1" ).arg( i ), "xixi", i,
      //                             _def_status_online );
      //        //把好友添加到列表
      //        m_pFriendList->addFriend( form );
      //      }
      //      for ( int i = 6; i < 11; i++ ) {
      //        // new 一个好友
      //        FriendForm* form = new FriendForm;
      //        //设置好友属性
      //        form->setFriendInfo( i, QString( "用户%1" ).arg( i ), "xixi", i,
      //                             _def_status_offline );
      //        //把好友添加到列表
      //        m_pFriendList->addFriend( form );
      //      }
    } break;
    default: break;
  }
}

// 3)处理用户信息
void CKernel::dealUserInfo( char* data, int len, unsigned long from ) {
  qDebug() << __func__;
  // 1.拆包
  _STRU_USER_INFO* info = (_STRU_USER_INFO*) data;
  QString          name    = gb2312ToUtf8( info->name );
  QString          feeling = gb2312ToUtf8( info->feeling );

  qDebug() << "name: " << name << "id：" << info->id;

  // 2.判断是否为当前登录用户
  if ( info->id == m_userId ) {
    //保存当前登录用户的昵称
    m_Username = name;
    //是当前登录用户,设置到用户控件上
    m_pFriendList->setUserInfo( name, feeling, info->iconId );
    return;
  }

  // 3.是好友的信息，判断是否再好友列表上
  if ( m_mapIdtoForm.count( info->id ) > 0 ) {
    //已经在好友列表上，取出好友信息FriendForm
    FriendForm* form = m_mapIdtoForm[ info->id ];
    //更新好友属性
    form->setFriendInfo( info->id, name, feeling, info->iconId, info->status );
  } else {
    //不在好友列表上，new一个新FriendForm
    FriendForm* form = new FriendForm;
    //设置好友属性
    form->setFriendInfo( info->id, name, feeling, info->iconId, info->status );
    //添加到好友列表上
    m_pFriendList->addFriend( form );
    //把好友保存到map中，key是id
    m_mapIdtoForm[ info->id ] = form;
    //连接显示与当前好友的聊天窗口的信号和槽函数
    QObject::connect( form, &FriendForm::sig_showChatDialog, this,
                      &CKernel::slot_showChatDialog );

    // new一个与该好友的聊天窗口
    ChatDialog* dialog = new ChatDialog;
    //设置聊天窗口属性
    dialog->setDialogInfo( info->id, name );
    //保存聊天窗口到map中，key是好友id
    m_mapIdToDialog[ info->id ] = dialog;
    //连接发送聊天内容的信号和槽函数
    QObject::connect( dialog, &ChatDialog::sig_sendMessage, this,
                      &CKernel::slot_sendMessage );
  }
}

// 4）处理好友下线请求
void CKernel::dealOfflineRq( char* data, int len, unsigned long from ) {
  qDebug() << __func__;

  // 1.拆包
  _STRU_OFFLINE_RQ* rq = (_STRU_OFFLINE_RQ*) data;

  // 2.找到好友的对象
  if ( m_mapIdtoForm.count( rq->userId ) > 0 ) {
    //取出好友对象
    FriendForm* form = m_mapIdtoForm[ rq->userId ];
    //设置好友下线
    form->setFriendOffline();
  }
}
// 5）处理聊天请求（当前是用户B,收到好友A的聊天请求）
void CKernel::dealChatRq( char* data, int len, unsigned long from ) {
  qDebug() << __func__;

  // 1.拆包
  _STRU_CHAT_RQ* rq = (_STRU_CHAT_RQ*) data;

  // 2.根据好友A的id找到与好友A的聊天框
  if ( m_mapIdToDialog.count( rq->userId )
       > 0 ) {  //服务器是直接转发客户端的请求的，userId就是原发送客户端的id
    //取出与好友A的聊天窗口
    ChatDialog* dialog = m_mapIdToDialog[ rq->userId ];
    //把聊天内容显示到窗口上
    dialog->setChatContent( rq->content );
    //显示窗口
    dialog->show();
  }
}
// 6）处理聊天回复（当前是用户A,好友B不在线）
void CKernel::dealChatRs( char* data, int len, unsigned long from ) {
  qDebug() << __func__;

  // 1.拆包
  _STRU_CHAT_RS* rs = (_STRU_CHAT_RS*) data;

  // 2.根据用户B的id找到好友的聊天窗口
  if ( m_mapIdToDialog.count( rs->friendId ) > 0 ) {
    //取出与好友B的聊天窗口
    ChatDialog* dialog = m_mapIdToDialog[ rs->friendId ];
    //显示好友B不在线
    dialog->showFriendOffline();
    //显示聊天窗口
    dialog->show();
  }
}
// 7）处理添加好友请求（当前是用户B,收到好友A的添加好友请求）
void CKernel::dealAddFriendRq( char* data, int len, unsigned long from ) {
  qDebug() << __func__;

  // 1.拆包
  _STRU_ADD_FRIEND_RQ* rq = (_STRU_ADD_FRIEND_RQ*) data;

  // 2.客户端B弹出询问窗口
  _STRU_ADD_FRIEND_RS rs;
  rs.friendId = rq->userId;
  rs.userId   = m_userId;
  strcpy_s( rs.friendName, sizeof( rs.friendName ), rq->friendName );

  if ( QMessageBox::Yes
       == QMessageBox::question(
           m_pFriendList, "添加好友",
           QString( "【%1】请求添加好友，是否同意" ).arg( rq->userName ) ) ) {
    //同意

    rs.result = _def_add_friend_success;
  } else {
    //不同意
    rs.result = _def_add_friend_refuse;
  }

  // 3.将添加请求的好友回复发送给服务器
  m_pMediator->sendData( (char*) &rs, sizeof( rs ), 12 );
}
// 8）处理添加好友回复（当前是用户A,用户B不存在或不在线）
void CKernel::dealAddFriendRs( char* data, int len, unsigned long from ) {
  qDebug() << __func__;

  // 1.拆包
  _STRU_ADD_FRIEND_RS* rs   = (_STRU_ADD_FRIEND_RS*) data;
  QString              name = gb2312ToUtf8( rs->friendName );

  //判断添加结果
  switch ( rs->result ) {
    case _def_add_friend_success:
      QMessageBox::about( m_pFriendList, "添加好友",
                          QString( "添加好友【%1】成功" ).arg( name ) );
      break;
    case _def_add_friend_not_exists:
      QMessageBox::about(
          m_pFriendList, "添加好友",
          QString( "添加好友【%1】为好友失败，好友不存在" ).arg( name ) );
      break;
    case _def_add_friend_offlien:
      QMessageBox::about(
          m_pFriendList, "添加好友",
          QString( "添加好友【%1】为好友失败，好友不在线" ).arg( name ) );
      break;
    case _def_add_friend_refuse:
      QMessageBox::about(
          m_pFriendList, "添加好友",
          QString( "添加好友【%1】为好友失败，好友拒绝" ).arg( name ) );
      break;
    default: break;
  }
}

//处理、分发所有接收到的数据
void CKernel::slot_transmitData( char* data, int len, unsigned long from ) {
  qDebug() << __func__;

  //  _STRU_REGISTER_RS* rs = (_STRU_REGISTER_RS*) data;
  //  qDebug() << rs->result;

  //（1）先取出接收数据中的处理结构体对应的类型
  int type = *(int*) data;
  //(2)根据类型找到对应的下标值
  int index = type - _DEF_PROTOCOL_BASE - 1;
  //(3)判断下标值的合法性
  if ( index >= 0 && index < _DEF_ARRRY_FUN_LENGTH ) {
    //取出对应函数地址
    PFUN pf = m_funArrry[ index ];
    //判断函数地址短的有效性
    if ( pf ) {
      //调用对应函数
      ( this->*pf )( data, len, from );
    } else {
      qDebug() << "函数指针为空，type:" << type;
    }

  } else {
    qDebug() << "函数地址数组的下标非法，type:" << type;
  }

  //回收客户端申请的空间
  delete[] data;
  data = nullptr;
}

// 1)处理注册信息，发送给服务端
void CKernel::slot_registerMassage( QString name, QString tel,
                                    QString password ) {
  qDebug() << __func__;

  // 1.打包
  _STRU_REGISTER_RQ rq;

  // strcpy_s( rq.name, sizeof( rq.name ), name.toStdString().c_str() );
  utf8ToGb2312( name, rq.name, sizeof( rq.name ) );
  strcpy_s( rq.tel, sizeof( rq.tel ), tel.toStdString().c_str() );
  strcpy_s( rq.password, sizeof( rq.password ),
            password.toStdString().c_str() );

  // 2.通过中介者类发送给服务器
  m_pMediator->sendData( (char*) &rq, sizeof( rq ), 1235 );
}

// 2)处理登录信息，发送给服务端
void CKernel::slot_loginMassage( QString tel, QString password ) {
  qDebug() << __func__;
  // 1.打包
  _STRU_LOGIN_RQ rq;
  strcpy_s( rq.tel, sizeof( rq.tel ), tel.toStdString().c_str() );
  strcpy_s( rq.password, sizeof( rq.password ),
            password.toStdString().c_str() );

  // 2.通过中介者类发送给服务器
  m_pMediator->sendData( (char*) &rq, sizeof( rq ), 1111 );
}

// 3）处理LoginDiolog退出进程的信号
void CKernel::slot_closeProcess() {
  qDebug() << __func__;
  // 1.回收资源
  if ( m_pMediator ) {
    m_pMediator->closeNet();
    delete m_pMediator;
    m_pMediator = nullptr;
  }

  if ( m_pLoginDlg ) {
    m_pLoginDlg->hide();
    delete m_pLoginDlg;
    m_pLoginDlg = nullptr;
  }
  if ( m_pFriendList ) {
    m_pFriendList->hide();
    delete m_pFriendList;
    m_pFriendList = nullptr;
  }

  //退出进程
  exit( 0 );
}

// 4)处理下线信号
void CKernel::slot_offlien() {
  qDebug() << __func__;

  // 1.给服务器发送下线请求结构体数据
  _STRU_OFFLINE_RQ rq;
  rq.userId = m_userId;
  m_pMediator->sendData( (char*) &rq, sizeof( rq ), 222 );

  // 2.回收资源退出线程
  slot_closeProcess();
}
// 5) 处理显示与当前好友的聊天窗口
void CKernel::slot_showChatDialog( int friendId ) {
  qDebug() << __func__;

  // 判断是否有当前好友的聊天窗口
  if ( m_mapIdToDialog.count( friendId ) > 0 ) {
    //存在，就取出来显示
    ChatDialog* dialog = m_mapIdToDialog[ friendId ];

    dialog->show();
  }
}
// 6)处理由Dialog发送来的聊天内容,将聊天内容发送给服务器
void CKernel::slot_sendMessage( int friendId, QString content ) {
  qDebug() << __func__;

  // 1.打包
  _STRU_CHAT_RQ rq;
  rq.friendId = friendId;
  rq.userId   = m_userId;
  // utf8ToGb2312( content, rq.content, sizeof( rq.content ) );
  strcpy_s( rq.content, sizeof( rq.content ), content.toStdString().c_str() );
  // 2.把聊天请求发送给服务器
  m_pMediator->sendData( (char*) &rq, sizeof( rq ), 77 );
}
// 7)处理添加由FriendList发送的好友的信号
void CKernel::slot_addFriend() {
  qDebug() << __func__;
  // 1.弹出输入窗口，让用户输入好友昵称
  QString name =
      QInputDialog::getText( m_pFriendList, "添加好友", "请输入好友昵称:" );
  QString nameCpy = name;
  // 2.校验输入内容是否为空或者全为空格
  nameCpy.remove( " " );
  if ( nameCpy.isEmpty() ) {
    QMessageBox::about( m_pFriendList, "添加好友",
                        "昵称不能为空或者全为空空格" );
    return;
  }
  // 3.判断输入的昵称是否为自己的昵称
  if ( name == m_Username ) {
    QMessageBox::about( m_pFriendList, "添加好友", "不能添加自己为好友" );
    return;
  }
  // 4.判断输入的昵称是否为已存在好友的昵称
  //遍历
  for ( auto ite = m_mapIdtoForm.begin(); ite != m_mapIdtoForm.end(); ite++ ) {
    //取出每个好友对象
    FriendForm* form = ite.value();

    //判断好友昵称是否与输入昵称相等
    if ( name == form->name() ) {
      QMessageBox::about( m_pFriendList, "添加好友", "已经是好友" );
      return;
    }
  }

  // 5.给服务端发送添加好友请求
  //打包
  _STRU_ADD_FRIEND_RQ rq;
  rq.userId = m_userId;
  //  strcpy_s( rq.friendName, sizeof( rq.friendName ),
  //            name.toStdString().c_str() );
  utf8ToGb2312( name, rq.friendName, sizeof( rq.friendName ) );

  strcpy_s( rq.userName, sizeof( rq.userName ),
            m_Username.toStdString().c_str() );

  //发送给服务器
  m_pMediator->sendData( (char*) &rq, sizeof( rq ), 55 );
}
