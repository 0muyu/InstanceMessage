#include "Ckernel.h"
#include "config.h"


unique_ptr<Ckernel, Ckernel::deleter> Ckernel::m_uptr;
mutex Ckernel::m_mutex;  //�Լ�ִ�й���




Ckernel& Ckernel::instance() {
  // TODO: �ڴ˴����� return ���
  if (!m_uptr.get()) {
    lock_guard<mutex> lck(m_mutex);
    if (!m_uptr.get()) {  //��α�֤�̰߳�ȫ
      m_uptr.reset(new Ckernel());
    }
  }
  return *m_uptr.get();  //�ⲿʹ��ʹ������

}

// 1.�򿪷�����
bool Ckernel::startServer(IOCPTcpServer& netServer) {
  // 1.��ʼ����������ַ����
  initFunArrry();
  // 2.������
  m_iocpServer = &netServer;
  m_iocpServer->run();
 // return iocp_net->run();


  //// 3.TODO:�������ݿ�
  //// bool  ConnectMySql(char *host,char *user,char *pass,char *db,short nport =
  //// 3306);
  //char ip[] = "localhost";
  //char name[] = "root";
  //char password[] = "690403";
  //char db[] = "im_sql";

  //if (!.ConnectMySql(ip, name, password, db)) {
  //  cout << '\n';
  //  cout << "���ݿ�����ʧ��!" << '\n';
  //  return false;
  //}



  return true;
}



// 2.�رշ�����
void Ckernel::closeServer() {
  //// 1.�ر�����
  //if (m_iocpServer) {
  //  m_iocpServer->closeNet();
  //  delete  m_iocpServer;
  //   m_iocpServer = nullptr;
  //}

  // 2.TODO:�Ͽ����ݿ�����
  //DisConnect();
}



// 3.�����ַ����н��յ�������
void Ckernel::dealData(char* data, int len, unsigned long from) {
  cout << __func__ << '\n';

  //(1)��ȡ�����������еĴ���ṹ���Ӧ������
  int type = *(int*)data;
  //(2)����typeֵ���������±꣬�ҵ���Ӧ�Ĵ�����
  int index = type - _DEF_PROTOCOL_BASE - 1;
  //(3)�ж������±��Ƿ�����Ч
  if (index >= 0 && index < _DEF_ARRRY_FUN_LENGTH) {
    //�ж������ں�����ַ�Ƿ���Ч
    PFUN pf = m_arrFun[index];
    if (pf) {
      //���ô�����
      (this->*pf)(data, len, from);
    } else {
      // type���ԡ����ܵ�ԭ��:1���Զ˷��͵Ľṹ�岻��;2���ṹ��ԣ����ǽṹ���typeֵ����
      // 3�����������ĵ�ַ�Ƿ��Ѿ����浽��������
      cout << "����ָ��Ϊ�գ�type:" << type << '\n';
    }
  } else {
    // type����Ǹ��ر����������ܵ�ԭ��:
    // 1��offsetû����;2���ṹ������typeֵû��ʼ�����߶����λ��û�ڵ�һ��
    // 3����˶Զ�net���sendData�����ͱ���net��Ľ��պ���
    cout << "������ַ������±�Ƿ�,type��" << type << '\n';
  }

  //���������շ�������IOCPTcpServer�У�����ռ�
  //delete[] data;
  // data = nullptr;
  free(data);
}

// 4.�Ѵ������ĵ�ַ���浽������
void Ckernel::initFunArrry() {
  cout << __func__ << '\n';
  //������ռ��ʼ��Ϊ0
  memset(m_arrFun, 0, sizeof(m_arrFun));

  //�Ѵ������ĵ�ַ���浽������
  m_arrFun[_DEF_REGISTER_RQ - _DEF_PROTOCOL_BASE - 1] =
      &Ckernel::dealRegisterRq;
  m_arrFun[_DEF_LOGIN_RQ - _DEF_PROTOCOL_BASE - 1] = &Ckernel::dealLoginRq;
  m_arrFun[_DEF_OFFLINE_RQ - _DEF_PROTOCOL_BASE - 1] = &Ckernel::dealOfflineRq;
  m_arrFun[_DEF_CHAT_RQ - _DEF_PROTOCOL_BASE - 1] = &Ckernel::dealChatRq;
  m_arrFun[_DEF_ADD_FRIEND_RQ - _DEF_PROTOCOL_BASE - 1] =
      &Ckernel::dealAddFriendRq;
  m_arrFun[_DEF_ADD_FRIEND_RS - _DEF_PROTOCOL_BASE - 1] =
      &Ckernel::dealAddFriendRs;
}

// 5.��ȡ��ǰ��¼�û�����Ϣ�ͺ�����Ϣ
void Ckernel::getUserInfoAndFriendInfo(int id) {
  cout << __func__ << '\n';

  //�����û���id��ѯ�û�����Ϣ
  _STRU_USER_INFO UserInfo;
  getInfoById(id, &UserInfo);

  // ���û�����Ϣ���ظ��ͻ���
  if (m_mapIdToSocket.count(id) > 0) {
     m_iocpServer->sendData((char*)&UserInfo, sizeof(UserInfo),
                          m_mapIdToSocket[id]);
  } else {
    std::cout << "mapIdToSocket is null" << '\n';
    exit(0);
  }

  //�����û�id��ѯ����id�б�
  char szSql[8 * 1024] = "";
  list<string> lstStr;
  sprintf_s(szSql, "select idB from t_friend where idA=%d;", id);

  if (!SQLselect(szSql, 1, lstStr)) {
    cout << "��ѯ���ݿ�ʧ�ܣ�sql:" << szSql << '\n';
    return;
  }

  //��������id�б�
  int friendId;
  _STRU_USER_INFO friendInfo;
  while (!lstStr.empty()) {
    // ȡ������id
    friendId = stoi(lstStr.front());
    lstStr.pop_front();

    //���ݺ���id��ѯ���ѵ���Ϣ
    getInfoById(friendId, &friendInfo);

    // �Ѻ��ѵ���Ϣ���ظ��ͻ���
    if (m_mapIdToSocket.count(id) > 0) {
       m_iocpServer->sendData((char*)&friendInfo, sizeof(friendInfo),
                            m_mapIdToSocket[id]);
    }

    //�жϵ�ǰ�����Ƿ�����
    if (m_mapIdToSocket.count(friendId) > 0) {
      //�������,֪ͨ�����Լ�������(���ǰ��Լ�����Ϣ��������)

       m_iocpServer->sendData((char*)&UserInfo, sizeof(UserInfo),
                            m_mapIdToSocket[friendId]);
    }
  }
}

// 6.����id��ѯ��Ϣ
void Ckernel::getInfoById(int id, _STRU_USER_INFO* info) {
  cout << __func__ << '\n';

  info->id = id;
  if (m_mapIdToSocket.count(id) > 0) {
    //����
    info->status = _def_status_online;

  } else {
    //������
    info->status = _def_status_offline;
  }

  //�����ݿ⣬����id��ѯ �û��ǳơ�ͷ��id��ǩ��
  char Sql[8 * 1024] = "";
  list<string> lstStr;
  sprintf_s(Sql, "select name,feeling,iconid from t_user where id=%d;", id);

  if (!SQLselect(Sql, 3, lstStr)) {
    cout << "��ѯ���ݿ�ʧ�ܣ���sql:" << Sql << '\n';
    return;
  }
  //�ж����ݸ����Ƿ���ȷ
  if (3 == lstStr.size()) {
    //ȡ���ǳƣ�����Info
    const char* name = lstStr.front().c_str();
    strcpy_s(info->name, sizeof(info->name), name);
    lstStr.pop_front();

    //ȡ��ǩ��������Info
    const char* feeling = lstStr.front().c_str();
    strcpy_s(info->feeling, sizeof(info->feeling), feeling);
    lstStr.pop_front();

    //ȡ��ͷ�񣬸���Info
    int iconId = stoi(lstStr.front());
    info->iconId = iconId;
    lstStr.pop_front();

  } else {
    cout << "�����ݿ��ѯ���ݴ���" << Sql << '\n';
    return;
  }
}

// 1)ע������Ĵ�����
void Ckernel::dealRegisterRq(char* data, int len, unsigned long from) {
  cout << __func__ << '\n';

  // 1.���
  _STRU_REGISTER_RQ* rq = (_STRU_REGISTER_RQ*)data;

  // 2.�����ݿ��ѯ,���ݵ绰�Ų�ѯ�绰��
  char szSql[2064] = "";
  list<string> listStr;
  sprintf_s(szSql, "select tel from t_user where tel='%s';", rq->tel);
  //�������ݿ�ʧ��ԭ��:1��û���������ݿ�������Ӵ�Ŀ�;2��sq1����д���(����־�е�sq1��俽����workbanch��ִ��)
  if (!SQLselect(szSql, 1, listStr)) {
    cout << '\n';
    cout << "tel��ѯ���ݿ�ʧ��,sql" << szSql << '\n';
    return;
  }

  //��ʼ���ش��������ͻ���
  _STRU_REGISTER_RS rs;
  //�жϸ��ݵ绰�Ų�ѯ����Ƿ�Ϊ��
  if (0 != listStr.size()) {
    // 3����ѯ�����Ϊ�գ��绰���Ѿ���ע�ᣬע��ʧ��_def_register_tel_exists
    rs.result = _def_register_tel_exists;

  } else {
    //˵���绰��û�б�ע�ᣬ����ǳ��Ƿ�ע��
    // 4.�����ݿ��ѯ,�����ǳƲ�ѯ�ǳ�
    sprintf_s(szSql, "select name from t_user where name='%s';", rq->name);

    if (!SQLselect(szSql, 1, listStr)) {
      //�������ݿ�ʧ��ԭ��:1��û���������ݿ�������Ӵ�Ŀ�;2��sq1����д���(����־�е�sq1��俽����workbanch��ִ��)
      cout << '\n';
      cout << "name��ѯ���ݿ�ʧ��,sql" << szSql << '\n';
      return;
    }

    //�ж��ǳƲ�ѯ����Ƿ�Ϊ��
    if (0 != listStr.size()) {
      // 5����ѯ�����Ϊ�գ��ǳ��Ѿ���ע�ᣬע��ʧ��_def_register_name_exists
      rs.result = _def_register_name_exists;

    } else {
      //˵���ǳ�û�б�ע�ᣬ��ʼע��
      // 6.���û���Ϣд�����ݿ�
      sprintf_s(szSql,
                "insert into t_user (name,tel,pass,feeling,iconid) values "
                "('%s','%s','%s','���ڳԷ�',4);",
                rq->name, rq->tel, rq->password);

      if (!SQLupdate(szSql)) {
        //�������ݿ�ʧ��ԭ��:1��û���������ݿ�������Ӵ�Ŀ�;2��sql����д���(����־�е�sq1��俽����workbanch��ִ��)
        cout << '\n';
        cout << "�������ݿ�ʧ��,sql" << szSql << '\n';
        return;
      }

      // 7.ע��ɹ�_def_rigister_success
      rs.result = _def_rigister_success;
    }
  }
  // 8.��ע�������ظ��ͻ���
   m_iocpServer->sendData((char*)&rs, sizeof(rs), from);
}

// 2)��¼����Ĵ�����
void Ckernel::dealLoginRq(char* data, int len, unsigned long from) {
  cout << __func__ << '\n';
  // 1�����
  _STRU_LOGIN_RQ* rq = (_STRU_LOGIN_RQ*)data;

  // 2�������ݿ⣬���ݵ绰�����ѯ����
  // ����ֵ�����ѯ����Ƿ�ִ�гɹ�
  // bool  SQLselect(char* szSql/*��ѯҪִ�е�sql���*/,int
  // nColumn/*��ѯ���еĸ���*/ ,list<string>& lstStr/*sql���Ĳ�ѯ���*/);
  std::cout << rq->tel << ' ' << rq->password << '\n';

  char szSql[2 * 1024] = "";
  list<string> lstStr;
  sprintf_s(szSql, "select pass,id from t_user where tel='%s';", rq->tel);

  if (!SQLselect(szSql, 2, lstStr)) {
    cout << "���ݿ��ѯʧ�ܣ�sql:" << szSql << '\n';
    return;
  }

  //��ͻ��˷��ش�����
  _STRU_LOGIN_RS rs;
  // 3���жϲ�ѯ����Ƿ�Ϊ��
  if (0 == lstStr.size()) {
    // 4����ѯ���Ϊ�գ�˵���绰����û�б�ע�������¼ʧ��
    rs.result = _def_login_tel_not_register;
  } else {
    // 5��(1)�Ӳ�ѯ�����ȡ��ע��ʱ������
    string password = lstStr.front();
    lstStr.pop_front();
    //(2)�Ӳ�ѯ�����ȡ��ע��ʱ��id:ע���ѯ�ͻ�ȡ��˳����һ�µ�
    int id = stoi(lstStr.front());
    lstStr.pop_front();

    // 6���Ƚ�ע��ʱ������͵�¼�������Ƿ����
    if (password == rq->password) {
      // 7�������ȣ����ǵ�¼�ɹ�_def_login_successfelse
      rs.result = _def_login_success;
      rs.id = id;

      //���۵�¼�����Σ����ѵ�¼������ظ��ͻ���  �ָ�
       m_iocpServer->sendData((char*)&rs, sizeof(rs), from);
      Sleep(2);

      //���浱ǰ��¼�û�id��SOCKET
      m_mapIdToSocket[id] = from;

      // TODO:��ȡ��ǰ��¼�û�����Ϣ�ͺ�����Ϣ   �ָ�
       getUserInfoAndFriendInfo(id);//id�����û�����Ķ��Ǵ����ݿ��в�ѯ��ȡ�ģ�Ϊ�˼��ٶ����ݿ�Ĳ�ѯ����������һ���ѯ

       return;
    } else {
      // 8������ȣ���¼ʧ�� def login password error
      rs.result = _def_login_password_error;
    }
  }

  //  9�����۵�¼�����Σ����ѵ�¼������ظ��ͻ���  �ָ�
   m_iocpServer->sendData((char*)&rs, sizeof(rs), from);
}

// 3��������������
void Ckernel::dealOfflineRq(char* data, int len, unsigned long from) {
  cout << __func__ << '\n';
  // 1�����
  _STRU_OFFLINE_RQ* rq = (_STRU_OFFLINE_RQ*)data;

  // 2.�����û�id��ѯ����id�б�
  char szSql[2 * 1024] = "";
  list<string> lstStr;
  sprintf_s(szSql, "select idB from t_friend where idA=%d", rq->userId);
  if (!SQLselect(szSql, 1, lstStr)) {
    cout << "���ݿ��ѯʧ�ܣ�sql:" << szSql << '\n';
    return;
  }

  // 3.��������id�б�
  int friendId;
  while (!lstStr.empty()) {
    //ȡ������id
    friendId = stoi(lstStr.front());
    lstStr.pop_front();

    //���ݺ���id�жϺ����Ƿ�����
    if (m_mapIdToSocket.count(friendId) > 0) {
      //֪ͨ���ߺ��ѣ��Լ������ߣ������������͸����ѣ�
       m_iocpServer->sendData(data, len, m_mapIdToSocket[friendId]);
    }
  }

  // 4.�������û�map��ɾ����ӦԪ��
  if (m_mapIdToSocket.count(rq->userId) > 0) {
    //�ر�SOCKET
    closesocket(m_mapIdToSocket[rq->userId]);
    //�ѽڵ��map���Ƴ�

    m_mapIdToSocket.erase(rq->userId);
  }
}

// 4��������������
void Ckernel::dealChatRq(char* data, int len, unsigned long from) {
  cout << __func__ << '\n';
  // 1�����
  _STRU_CHAT_RQ* rq = (_STRU_CHAT_RQ*)data;

  // 2.�жϺ����Ƿ�����
  if (m_mapIdToSocket.count(rq->friendId) > 0) {  //����
    //����������ֱ��ת�������� B
     m_iocpServer->sendData(data, len, m_mapIdToSocket[rq->friendId]);

  } else {  //������
    //���߿ͻ���A(ԭ�����������)����ʧ��
    _STRU_CHAT_RS rs;
    rs.result = _def_send_error;
    rs.friendId = rq->friendId;
     m_iocpServer->sendData((char*)&rs, sizeof(rs), from);

    // TODO:ʵ��:Ӧ�ð���������ݱ��浽�����У������ݿ��н�һ���µı����淢���û���id��������)��id������ʱ�䡢��������
    //ÿ���û����߳ɹ���ʱ�򣬴ӱ���������ı��в�ѯ���Ƿ��з�����ǰ�����û�����������
    //����ҵ��˰�ȡ�����ݰ���Щ�����뷢�������û����������ݿ���ɾ��
  }
}

// 5��������Ӻ�������
void Ckernel::dealAddFriendRq(char* data, int len, unsigned long from) {
  cout << __func__ << '\n';
  // 1�����
  _STRU_ADD_FRIEND_RQ* rq = (_STRU_ADD_FRIEND_RQ*)data;

  // 2.���ݺ��ѵ��ǳƲ�ѯ���ѵ�id
  char szSql[1024] = "";
  list<string> lstStr;
  sprintf_s(szSql, "select id from t_user where name='%s';", rq->friendName);
  if (!SQLselect(szSql, 1, lstStr)) {
    cout << "��ѯ����ʧ�ܣ�sql:" << szSql << '\n';
    return;
  }
  // 3.�жϲ�ѯ���
  _STRU_ADD_FRIEND_RS rs;
  if (0 == lstStr.size()) {  //Ϊ��
    //���ʧ�ܣ����Ѳ�����_def_add_friend_not_exists
    rs.result = _def_add_friend_not_exists;

    strcpy_s(rs.friendName, sizeof(rs.friendName), rq->friendName);
    //����ӽ�����ͻؿͻ���A
     m_iocpServer->sendData((char*)&rs, sizeof(rs), from);

  } else {  //��Ϊ��
    //ȡ������id
    int friendId = stoi(lstStr.front());
    lstStr.pop_front();
    // 4.�жϺ����Ƿ�����
    if (m_mapIdToSocket.count(friendId) > 0) {  //����
      //����Ӻ�������ֱ��ת�����û�B
       m_iocpServer->sendData(data, len, m_mapIdToSocket[friendId]);
    } else {  //������
      //���Ѳ����ߣ����ʧ��_def_add_friend_offlien
      rs.result = _def_add_friend_offlien;
      sprintf_s(rs.friendName, sizeof(rs.friendName), rq->friendName);
      //�������:����Ӻ������󱣴浽���ݿ��У�����һ���µı����û�A��id��A���ǳơ�B���ǳơ����ʱ��
      //��B�û����ߵ�ʱ�򣬲�ѯ����������B�û�����Щ��Ϣд����Ӻ��ѽṹ�壬����B�û�
      //Ȼ�󣬴����ݿ���ɾ�����B�û�����Щ��Ϣ

      //����ӽ�����ͻؿͻ���A
       m_iocpServer->sendData((char*)&rs, sizeof(rs), from);
    }
  }
}
// 6)������Ӻ��ѵĻظ�
void Ckernel::dealAddFriendRs(char* data, int len, unsigned long from) {
  cout << __func__ << '\n';
  // 1�����
  _STRU_ADD_FRIEND_RS* rs = (_STRU_ADD_FRIEND_RS*)data;

  // 2.�ж���ӽ���Ƿ�ͬ��
  if (_def_add_friend_success == rs->result) {  //ͬ��
    //�Ѻ��ѹ�ϵ д�����ݿ���
    char szSql[1024 * 2] = "";

    sprintf_s(szSql, "insert into t_friend values (%d,%d);", rs->friendId,
              rs->userId);

    if (!SQLupdate(szSql)) {
      cout << "�������ݿ�ʧ�ܣ�sql:" << szSql << '\n';
      return;
    }

    sprintf_s(szSql, "insert into t_friend values (%d,%d);", rs->userId,
              rs->friendId);

    if (!SQLupdate(szSql)) {
      cout << "�������ݿ�ʧ�ܣ�sql:" << szSql << '\n';
      return;
    }
    //ˢ��˫�ˣ��ͻ���A��B�������б�
    getUserInfoAndFriendInfo(rs->friendId);
  }

  // 3.����ӽ��ת�����ͻ���A
  if (m_mapIdToSocket.count(rs->friendId) > 0) {
     m_iocpServer->sendData(data, len, m_mapIdToSocket[rs->friendId]);
  }
}
