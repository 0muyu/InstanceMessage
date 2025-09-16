#pragma once


#ifndef CONFIG_H
#define CONFIG_H

#include <atomic>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

// >============================================

extern std::mutex g_lock;

// !============= global var

// --------------------------- MySQL
#define DB_ADDR "localhost"
#define DB_USER "root"
#define DB_PASS "690403"
#define DB_NAME "im_sql"

// --------------------------- Leader Follower Thread Pool
#define NetWork 1

// $defines

// �������ݰ�
struct TaskData {
  char* data;
  int len;
  unsigned long from;
};

// ����ṹ��
struct Task {
  void (*func)(void*, int);
  void* data;
  int len;
};

// UDPд�Ķ˿ں�
#define _DEF_UDP_PORT (1234)
// TCP�ͻ��˵Ķ˿ں�
#define _DEF_TCP_PORT (12345)

//�ֻ��ŵĳ���
#define _DEF_TEL_LENGTH (12)  //����ĩβ��\0
//�������󳤶�
#define _DEF_PASSWORD_MAX_LENGTH (20)
//�ǳƵ���󳤶�
#define _DEF_NAME_MAX_LENGTH (20)

//ǩ������󳤶�
#define _DEF_FEELING_MAX_LENGTH (100)

//�������ݵ���󳤶�
#define _DEF_CONTENT_MAX_LENGTH (8 * 1024)

//����ָ������ĳ���
#define _DEF_ARRRY_FUN_LENGTH (10)

//����ṹ������ĺ�
#define _DEF_PROTOCOL_BASE (1000)

//ע������
#define _DEF_REGISTER_RQ (_DEF_PROTOCOL_BASE + 1)
//ע��ظ�
#define _DEF_REGISTER_RS (_DEF_PROTOCOL_BASE + 2)

//��¼����
#define _DEF_LOGIN_RQ (_DEF_PROTOCOL_BASE + 3)
//��¼�ظ�
#define _DEF_LOGIN_RS (_DEF_PROTOCOL_BASE + 4)

//��Ӻ�������
#define _DEF_ADD_FRIEND_RQ (_DEF_PROTOCOL_BASE + 5)
//��Ӻ��ѻظ�
#define _DEF_ADD_FRIEND_RS (_DEF_PROTOCOL_BASE + 6)

//�Լ�/������Ϣ
#define _DEF_USER_INFO (_DEF_PROTOCOL_BASE + 7)

// ��������
#define _DEF_CHAT_RQ (_DEF_PROTOCOL_BASE + 8)
// ����ظ�
#define _DEF_CHAT_RS (_DEF_PROTOCOL_BASE + 9)

// ��������
#define _DEF_OFFLINE_RQ (_DEF_PROTOCOL_BASE + 10)

//����������
//ע����
#define _def_rigister_success (0)
#define _def_register_tel_exists (1)
#define _def_register_name_exists (2)
//��¼���
#define _def_login_success (0)
#define _def_login_tel_not_register (1)
#define _def_login_password_error (3)
//��Ӻ��ѽ��
#define _def_add_friend_success (0)
#define _def_add_friend_not_exists (1)
#define _def_add_friend_offlien (2)
#define _def_add_friend_refuse (3)
//״̬
#define _def_status_online (0)
#define _def_status_offline (1)
// ���ͽ��
#define _def_send_success (0)
#define _def_send_error (1)

//����ṹ��
//ע�������ֻ��š����롢�ǳƣ�
struct _STRU_REGISTER_RQ {
  _STRU_REGISTER_RQ() : type(_DEF_REGISTER_RQ) {
    memset(tel, 0, _DEF_TEL_LENGTH);
    memset(password, 0, _DEF_PASSWORD_MAX_LENGTH);
    memset(name, 0, _DEF_NAME_MAX_LENGTH);
  }
  int type;
  char tel[_DEF_TEL_LENGTH];
  char password[_DEF_PASSWORD_MAX_LENGTH];
  char name[_DEF_NAME_MAX_LENGTH];
};
// ע��ظ�(ע�������ɹ���ʧ�ܣ��ֻ����ѱ�ע�� ���ǳ��ѱ�ʹ��)
struct _STRU_REGISTER_RS {
  _STRU_REGISTER_RS()
      : type(_DEF_REGISTER_RS), result(_def_register_name_exists) {}

  int type;
  int result;
};

//��¼�����ֻ��š����룩
struct _STRU_LOGIN_RQ {
  _STRU_LOGIN_RQ() : type(_DEF_LOGIN_RQ) {
    memset(tel, 0, _DEF_TEL_LENGTH);
    memset(password, 0, _DEF_PASSWORD_MAX_LENGTH);
  }
  int type;
  char tel[_DEF_TEL_LENGTH];
  char password[_DEF_PASSWORD_MAX_LENGTH];
};

//��¼�ظ�(�ɹ���ʧ�ܣ��ֻ���δע�ᡢ�������)
struct _STRU_LOGIN_RS {
  _STRU_LOGIN_RS()
      : type(_DEF_LOGIN_RS), result(_def_login_password_error), id(0) {}
  int type;
  int result;
  int id;
};
//��Ӻ������󣨺��ѵ��ǳơ��Լ���id���Լ����ǳƣ�
struct _STRU_ADD_FRIEND_RQ {
  _STRU_ADD_FRIEND_RQ() : type(_DEF_ADD_FRIEND_RQ), userId(0) {
    memset(userName, 0, _DEF_NAME_MAX_LENGTH);
    memset(friendName, 0, _DEF_NAME_MAX_LENGTH);
  }
  int type;
  int userId;
  char userName[_DEF_NAME_MAX_LENGTH];
  char friendName[_DEF_NAME_MAX_LENGTH];
};
// ��Ӻ��ѻظ����ɹ���ʧ�ܣ����Ѳ����ڡ����Ѿܾ������Ѳ����ߣ����ѵ��ǳƣ�
struct _STRU_ADD_FRIEND_RS {
  _STRU_ADD_FRIEND_RS()
      : type(_DEF_ADD_FRIEND_RS),
        result(_def_add_friend_refuse),
        friendId(0),
        userId(0) {
    memset(friendName, 0, _DEF_NAME_MAX_LENGTH);
  }
  int type;
  int result;
  int friendId;
  int userId;
  char friendName[_DEF_NAME_MAX_LENGTH];
};
//��ʾ�����б��Լ�/������Ϣ���û�id���û��ǳơ�ͷ��id��ǩ����״̬��
struct _STRU_USER_INFO {
  _STRU_USER_INFO()
      : type(_DEF_USER_INFO), id(0), iconId(0), status(_def_status_offline) {
    memset(name, 0, _DEF_NAME_MAX_LENGTH);
    memset(feeling, 0, _DEF_FEELING_MAX_LENGTH);
  }
  int type;
  int id;
  int iconId;
  int status;
  char name[_DEF_NAME_MAX_LENGTH];
  char feeling[_DEF_FEELING_MAX_LENGTH];
};
// ��������(�Լ���id�����ѵ�id����������)
struct _STRU_CHAT_RQ {
  _STRU_CHAT_RQ() : type(_DEF_CHAT_RQ), userId(0), friendId(0) {
    memset(content, 0, _DEF_CONTENT_MAX_LENGTH);
  }
  int type;
  int userId;
  int friendId;
  char content[_DEF_CONTENT_MAX_LENGTH];
};
// ����ظ�(���ͽ�����ɹ���ʧ��)
struct _STRU_CHAT_RS {
  _STRU_CHAT_RS() : type(_DEF_CHAT_RS), friendId(0), result(_def_send_error) {}
  int type;
  int friendId;
  int result;
};
// ���������Լ���id��
struct _STRU_OFFLINE_RQ {
  _STRU_OFFLINE_RQ() : type(_DEF_OFFLINE_RQ), userId(0) {}
  int type;
  int userId;
};

#endif  // !CONFIG_H
