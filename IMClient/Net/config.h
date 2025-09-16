#pragma once
#include <string.h>

// UDP写的端口号
#define _DEF_UDP_PORT ( 1234 )
// TCP客户端的端口号
#define _DEF_TCP_PORT ( 12345 )

//手机号的长度
#define _DEF_TEL_LENGTH ( 12 )  //包括末尾的\0
//密码的最大长度
#define _DEF_PASSWORD_MAX_LENGTH ( 20 )
//昵称的最大长度
#define _DEF_NAME_MAX_LENGTH ( 20 )

//签名的最大长度
#define _DEF_FEELING_MAX_LENGTH ( 100 )

//聊天内容的最大长度
#define _DEF_CONTENT_MAX_LENGTH ( 8 * 1024 )

//函数指针数组的长度
#define _DEF_ARRRY_FUN_LENGTH ( 10 )

//定义结构体变量的宏
#define _DEF_PROTOCOL_BASE ( 1000 )

//注册请求
#define _DEF_REGISTER_RQ ( _DEF_PROTOCOL_BASE + 1 )
//注册回复
#define _DEF_REGISTER_RS ( _DEF_PROTOCOL_BASE + 2 )

//登录请求
#define _DEF_LOGIN_RQ ( _DEF_PROTOCOL_BASE + 3 )
//登录回复
#define _DEF_LOGIN_RS ( _DEF_PROTOCOL_BASE + 4 )

//添加好友请求
#define _DEF_ADD_FRIEND_RQ ( _DEF_PROTOCOL_BASE + 5 )
//添加好友回复
#define _DEF_ADD_FRIEND_RS ( _DEF_PROTOCOL_BASE + 6 )

//自己/好友信息
#define _DEF_USER_INFO ( _DEF_PROTOCOL_BASE + 7 )

// 聊天请求
#define _DEF_CHAT_RQ ( _DEF_PROTOCOL_BASE + 8 )
// 聊天回复
#define _DEF_CHAT_RS ( _DEF_PROTOCOL_BASE + 9 )

// 下线请求
#define _DEF_OFFLINE_RQ ( _DEF_PROTOCOL_BASE + 10 )

//定义请求结果
//注册结果
#define _def_rigister_success ( 0 )
#define _def_register_tel_exists ( 1 )
#define _def_register_name_exists ( 2 )
//登录结果
#define _def_login_success ( 0 )
#define _def_login_tel_not_register ( 1 )
#define _def_login_password_error ( 3 )
//添加好友结果
#define _def_add_friend_success ( 0 )
#define _def_add_friend_not_exists ( 1 )
#define _def_add_friend_offlien ( 2 )
#define _def_add_friend_refuse ( 3 )
//状态
#define _def_status_online ( 0 )
#define _def_status_offline ( 1 )
// 发送结果
#define _def_send_success ( 0 )
#define _def_send_error ( 1 )

//定义结构体
//注册请求（手机号、密码、昵称）
struct _STRU_REGISTER_RQ {
  _STRU_REGISTER_RQ() : type( _DEF_REGISTER_RQ ) {
    memset( tel, 0, _DEF_TEL_LENGTH );
    memset( password, 0, _DEF_PASSWORD_MAX_LENGTH );
    memset( name, 0, _DEF_NAME_MAX_LENGTH );
  }
  int  type;
  char tel[ _DEF_TEL_LENGTH ];
  char password[ _DEF_PASSWORD_MAX_LENGTH ];
  char name[ _DEF_NAME_MAX_LENGTH ];
};
// 注册回复(注册结果：成功、失败：手机号已被注册 、昵称已被使用)
struct _STRU_REGISTER_RS {
  _STRU_REGISTER_RS() : type( _DEF_REGISTER_RS ), result( _def_register_name_exists ) {}

  int type;
  int result;
};

//登录请求（手机号、密码）
struct _STRU_LOGIN_RQ {
  _STRU_LOGIN_RQ() : type( _DEF_LOGIN_RQ ) {
    memset( tel, 0, _DEF_TEL_LENGTH );
    memset( password, 0, _DEF_PASSWORD_MAX_LENGTH );
  }
  int  type;
  char tel[ _DEF_TEL_LENGTH ];
  char password[ _DEF_PASSWORD_MAX_LENGTH ];
};

//登录回复(成功、失败：手机号未注册、密码错误)
struct _STRU_LOGIN_RS {
  _STRU_LOGIN_RS() : type( _DEF_LOGIN_RS ), result( _def_login_password_error ), id( 0 ) {}
  int type;
  int result;
  int id;
};

//添加好友请求（好友的昵称、自己的id、自己的昵称）
struct _STRU_ADD_FRIEND_RQ {
  _STRU_ADD_FRIEND_RQ() : type( _DEF_ADD_FRIEND_RQ ), userId( 0 ) {
    memset( userName, 0, _DEF_NAME_MAX_LENGTH );
    memset( friendName, 0, _DEF_NAME_MAX_LENGTH );
  }
  int  type;
  int  userId;
  char userName[ _DEF_NAME_MAX_LENGTH ];
  char friendName[ _DEF_NAME_MAX_LENGTH ];
};
// 添加好友回复（成功、失败：好友不存在、好友拒绝、好友不在线，好友的昵称）
struct _STRU_ADD_FRIEND_RS {
  _STRU_ADD_FRIEND_RS() : type( _DEF_ADD_FRIEND_RS ), result( _def_add_friend_refuse ), friendId( 0 ), userId( 0 ) { memset( friendName, 0, _DEF_NAME_MAX_LENGTH ); }
  int  type;
  int  result;
  int  friendId;
  int  userId;
  char friendName[ _DEF_NAME_MAX_LENGTH ];
};
//显示好友列表：自己/好友信息（用户id、用户昵称、头像id、签名、状态）
struct _STRU_USER_INFO {
  _STRU_USER_INFO() : type( _DEF_USER_INFO ), id( 0 ), iconId( 0 ), status( _def_status_offline ) {
    memset( name, 0, _DEF_NAME_MAX_LENGTH );
    memset( feeling, 0, _DEF_FEELING_MAX_LENGTH );
  }
  int  type;
  int  id;
  int  iconId;
  int  status;
  char name[ _DEF_NAME_MAX_LENGTH ];
  char feeling[ _DEF_FEELING_MAX_LENGTH ];
};
// 聊天请求(自己的id、好友的id、聊天内容)
struct _STRU_CHAT_RQ {
  _STRU_CHAT_RQ() : type( _DEF_CHAT_RQ ), userId( 0 ), friendId( 0 ) { memset( content, 0, _DEF_CONTENT_MAX_LENGTH ); }
  int  type;
  int  userId;
  int  friendId;
  char content[ _DEF_CONTENT_MAX_LENGTH ];
};
// 聊天回复(发送结果：成功、失败)
struct _STRU_CHAT_RS {
  _STRU_CHAT_RS() : type( _DEF_CHAT_RS ), friendId( 0 ), result( _def_send_error ) {}
  int type;
  int friendId;
  int result;
};
// 下线请求（自己的id）
struct _STRU_OFFLINE_RQ {
  _STRU_OFFLINE_RQ() : type( _DEF_OFFLINE_RQ ), userId( 0 ) {}
  int type;
  int userId;
};
