#include "Ckernel.h"
#include "config.h"


unique_ptr<Ckernel, Ckernel::deleter> Ckernel::m_uptr;
mutex Ckernel::m_mutex;  //自己执行构造




Ckernel& Ckernel::instance() {
  // TODO: 在此处插入 return 语句
  if (!m_uptr.get()) {
    lock_guard<mutex> lck(m_mutex);
    if (!m_uptr.get()) {  //如何保证线程安全
      m_uptr.reset(new Ckernel());
    }
  }
  return *m_uptr.get();  //外部使用使用引用

}

// 1.打开服务器
bool Ckernel::startServer(IOCPTcpServer& netServer) {
  // 1.初始化处理函数地址数组
  initFunArrry();
  // 2.打开网络
  m_iocpServer = &netServer;
  m_iocpServer->run();
 // return iocp_net->run();


  //// 3.TODO:连接数据库
  //// bool  ConnectMySql(char *host,char *user,char *pass,char *db,short nport =
  //// 3306);
  //char ip[] = "localhost";
  //char name[] = "root";
  //char password[] = "690403";
  //char db[] = "im_sql";

  //if (!.ConnectMySql(ip, name, password, db)) {
  //  cout << '\n';
  //  cout << "数据库连接失败!" << '\n';
  //  return false;
  //}



  return true;
}



// 2.关闭服务器
void Ckernel::closeServer() {
  //// 1.关闭网络
  //if (m_iocpServer) {
  //  m_iocpServer->closeNet();
  //  delete  m_iocpServer;
  //   m_iocpServer = nullptr;
  //}

  // 2.TODO:断开数据库连接
  //DisConnect();
}



// 3.处理、分发所有接收到的数据
void Ckernel::dealData(char* data, int len, unsigned long from) {
  cout << __func__ << '\n';

  //(1)先取出接收数据中的处理结构体对应的类型
  int type = *(int*)data;
  //(2)根据type值计算数组下标，找到对应的处理函数
  int index = type - _DEF_PROTOCOL_BASE - 1;
  //(3)判断数组下标是否有有效
  if (index >= 0 && index < _DEF_ARRRY_FUN_LENGTH) {
    //判断数组内函数地址是否有效
    PFUN pf = m_arrFun[index];
    if (pf) {
      //调用处理函数
      (this->*pf)(data, len, from);
    } else {
      // type不对。可能的原因:1、对端发送的结构体不对;2、结构体对，但是结构体的type值不对
      // 3、看处理函数的地址是否已经保存到数组中了
      cout << "函数指针为空，type:" << type << '\n';
    }
  } else {
    // type如果是个特别大的数，可能的原因:
    // 1、offset没清零;2、结构体里面type值没初始化或者定义的位置没在第一个
    // 3、检査对端net层的sendData函数和本端net层的接收函数
    cout << "函数地址数组的下标非法,type：" << type << '\n';
  }

  //处理完后回收服务器（IOCPTcpServer中）申请空间
  //delete[] data;
  // data = nullptr;
  free(data);
}

// 4.把处理函数的地址保存到数组中
void Ckernel::initFunArrry() {
  cout << __func__ << '\n';
  //把数组空间初始化为0
  memset(m_arrFun, 0, sizeof(m_arrFun));

  //把处理函数的地址保存到数组中
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

// 5.获取当前登录用户的信息和好友信息
void Ckernel::getUserInfoAndFriendInfo(int id) {
  cout << __func__ << '\n';

  //根据用户的id查询用户的信息
  _STRU_USER_INFO UserInfo;
  getInfoById(id, &UserInfo);

  // 把用户的信息发回给客户端
  if (m_mapIdToSocket.count(id) > 0) {
     m_iocpServer->sendData((char*)&UserInfo, sizeof(UserInfo),
                          m_mapIdToSocket[id]);
  } else {
    std::cout << "mapIdToSocket is null" << '\n';
    exit(0);
  }

  //根据用户id查询好友id列表
  char szSql[8 * 1024] = "";
  list<string> lstStr;
  sprintf_s(szSql, "select idB from t_friend where idA=%d;", id);

  if (!SQLselect(szSql, 1, lstStr)) {
    cout << "查询数据库失败，sql:" << szSql << '\n';
    return;
  }

  //遍历好友id列表
  int friendId;
  _STRU_USER_INFO friendInfo;
  while (!lstStr.empty()) {
    // 取出好友id
    friendId = stoi(lstStr.front());
    lstStr.pop_front();

    //根据好友id查询好友的信息
    getInfoById(friendId, &friendInfo);

    // 把好友的信息发回给客户端
    if (m_mapIdToSocket.count(id) > 0) {
       m_iocpServer->sendData((char*)&friendInfo, sizeof(friendInfo),
                            m_mapIdToSocket[id]);
    }

    //判断当前好友是否在线
    if (m_mapIdToSocket.count(friendId) > 0) {
      //如果在线,通知好友自己上线了(就是把自己的信息发给好友)

       m_iocpServer->sendData((char*)&UserInfo, sizeof(UserInfo),
                            m_mapIdToSocket[friendId]);
    }
  }
}

// 6.根据id查询信息
void Ckernel::getInfoById(int id, _STRU_USER_INFO* info) {
  cout << __func__ << '\n';

  info->id = id;
  if (m_mapIdToSocket.count(id) > 0) {
    //在线
    info->status = _def_status_online;

  } else {
    //不在线
    info->status = _def_status_offline;
  }

  //从数据库，根据id查询 用户昵称、头像id、签名
  char Sql[8 * 1024] = "";
  list<string> lstStr;
  sprintf_s(Sql, "select name,feeling,iconid from t_user where id=%d;", id);

  if (!SQLselect(Sql, 3, lstStr)) {
    cout << "查询数据库失败！，sql:" << Sql << '\n';
    return;
  }
  //判断数据个数是否正确
  if (3 == lstStr.size()) {
    //取出昵称，赋给Info
    const char* name = lstStr.front().c_str();
    strcpy_s(info->name, sizeof(info->name), name);
    lstStr.pop_front();

    //取出签名，赋给Info
    const char* feeling = lstStr.front().c_str();
    strcpy_s(info->feeling, sizeof(info->feeling), feeling);
    lstStr.pop_front();

    //取出头像，赋给Info
    int iconId = stoi(lstStr.front());
    info->iconId = iconId;
    lstStr.pop_front();

  } else {
    cout << "从数据库查询数据错误：" << Sql << '\n';
    return;
  }
}

// 1)注册请求的处理函数
void Ckernel::dealRegisterRq(char* data, int len, unsigned long from) {
  cout << __func__ << '\n';

  // 1.拆包
  _STRU_REGISTER_RQ* rq = (_STRU_REGISTER_RQ*)data;

  // 2.从数据库查询,根据电话号查询电话号
  char szSql[2064] = "";
  list<string> listStr;
  sprintf_s(szSql, "select tel from t_user where tel='%s';", rq->tel);
  //操作数据库失败原因:1、没有连接数据库或者连接错的库;2、sq1语句有错误(把日志中的sq1语句拷贝到workbanch中执行)
  if (!SQLselect(szSql, 1, listStr)) {
    cout << '\n';
    cout << "tel查询数据库失败,sql" << szSql << '\n';
    return;
  }

  //开始返回处理结果给客户端
  _STRU_REGISTER_RS rs;
  //判断根据电话号查询结果是否为空
  if (0 != listStr.size()) {
    // 3、查询结果不为空，电话号已经被注册，注册失败_def_register_tel_exists
    rs.result = _def_register_tel_exists;

  } else {
    //说明电话号没有被注册，检查昵称是否被注册
    // 4.从数据库查询,根据昵称查询昵称
    sprintf_s(szSql, "select name from t_user where name='%s';", rq->name);

    if (!SQLselect(szSql, 1, listStr)) {
      //操作数据库失败原因:1、没有连接数据库或者连接错的库;2、sq1语句有错误(把日志中的sq1语句拷贝到workbanch中执行)
      cout << '\n';
      cout << "name查询数据库失败,sql" << szSql << '\n';
      return;
    }

    //判断昵称查询结果是否为空
    if (0 != listStr.size()) {
      // 5、查询结果不为空，昵称已经被注册，注册失败_def_register_name_exists
      rs.result = _def_register_name_exists;

    } else {
      //说明昵称没有被注册，开始注册
      // 6.把用户信息写入数据库
      sprintf_s(szSql,
                "insert into t_user (name,tel,pass,feeling,iconid) values "
                "('%s','%s','%s','正在吃饭',4);",
                rq->name, rq->tel, rq->password);

      if (!SQLupdate(szSql)) {
        //操作数据库失败原因:1、没有连接数据库或者连接错的库;2、sql语句有错误(把日志中的sq1语句拷贝到workbanch中执行)
        cout << '\n';
        cout << "插入数据库失败,sql" << szSql << '\n';
        return;
      }

      // 7.注册成功_def_rigister_success
      rs.result = _def_rigister_success;
    }
  }
  // 8.把注册结果返回给客户端
   m_iocpServer->sendData((char*)&rs, sizeof(rs), from);
}

// 2)登录请求的处理函数
void Ckernel::dealLoginRq(char* data, int len, unsigned long from) {
  cout << __func__ << '\n';
  // 1、拆包
  _STRU_LOGIN_RQ* rq = (_STRU_LOGIN_RQ*)data;

  // 2、从数据库，根据电话号码查询密码
  // 返回值代表查询语句是否执行成功
  // bool  SQLselect(char* szSql/*查询要执行的sql语句*/,int
  // nColumn/*查询的列的个数*/ ,list<string>& lstStr/*sql语句的查询结果*/);
  std::cout << rq->tel << ' ' << rq->password << '\n';

  char szSql[2 * 1024] = "";
  list<string> lstStr;
  sprintf_s(szSql, "select pass,id from t_user where tel='%s';", rq->tel);

  if (!SQLselect(szSql, 2, lstStr)) {
    cout << "数据库查询失败，sql:" << szSql << '\n';
    return;
  }

  //向客户端返回处理结果
  _STRU_LOGIN_RS rs;
  // 3、判断查询结果是否为空
  if (0 == lstStr.size()) {
    // 4、查询结果为空，说明电话号码没有被注册过，登录失败
    rs.result = _def_login_tel_not_register;
  } else {
    // 5、(1)从查询结果中取出注册时的密码
    string password = lstStr.front();
    lstStr.pop_front();
    //(2)从查询结果中取出注册时的id:注意查询和获取的顺序是一致的
    int id = stoi(lstStr.front());
    lstStr.pop_front();

    // 6、比较注册时的密码和登录的密码是否相等
    if (password == rq->password) {
      // 7、如果相等，就是登录成功_def_login_successfelse
      rs.result = _def_login_success;
      rs.id = id;

      //无论登录结果如何，都把登录结果返回给客户端  恢复
       m_iocpServer->sendData((char*)&rs, sizeof(rs), from);
      Sleep(2);

      //保存当前登录用户id和SOCKET
      m_mapIdToSocket[id] = from;

      // TODO:获取当前登录用户的信息和好友信息   恢复
       getUserInfoAndFriendInfo(id);//id不是用户输入的而是从数据库中查询获取的，为了减少对数据库的查询次数与密码一起查询

       return;
    } else {
      // 8、不相等，登录失败 def login password error
      rs.result = _def_login_password_error;
    }
  }

  //  9、无论登录结果如何，都把登录结果返回给客户端  恢复
   m_iocpServer->sendData((char*)&rs, sizeof(rs), from);
}

// 3）处理下线请求
void Ckernel::dealOfflineRq(char* data, int len, unsigned long from) {
  cout << __func__ << '\n';
  // 1、拆包
  _STRU_OFFLINE_RQ* rq = (_STRU_OFFLINE_RQ*)data;

  // 2.根据用户id查询好友id列表
  char szSql[2 * 1024] = "";
  list<string> lstStr;
  sprintf_s(szSql, "select idB from t_friend where idA=%d", rq->userId);
  if (!SQLselect(szSql, 1, lstStr)) {
    cout << "数据库查询失败，sql:" << szSql << '\n';
    return;
  }

  // 3.变量好友id列表
  int friendId;
  while (!lstStr.empty()) {
    //取出好友id
    friendId = stoi(lstStr.front());
    lstStr.pop_front();

    //根据好友id判断好友是否在线
    if (m_mapIdToSocket.count(friendId) > 0) {
      //通知在线好友，自己已下线（把下线请求发送给好友）
       m_iocpServer->sendData(data, len, m_mapIdToSocket[friendId]);
    }
  }

  // 4.从在线用户map中删除对应元素
  if (m_mapIdToSocket.count(rq->userId) > 0) {
    //关闭SOCKET
    closesocket(m_mapIdToSocket[rq->userId]);
    //把节点从map中移除

    m_mapIdToSocket.erase(rq->userId);
  }
}

// 4）处理聊天请求
void Ckernel::dealChatRq(char* data, int len, unsigned long from) {
  cout << __func__ << '\n';
  // 1、拆包
  _STRU_CHAT_RQ* rq = (_STRU_CHAT_RQ*)data;

  // 2.判断好友是否在线
  if (m_mapIdToSocket.count(rq->friendId) > 0) {  //在线
    //把聊天请求直接转发给好友 B
     m_iocpServer->sendData(data, len, m_mapIdToSocket[rq->friendId]);

  } else {  //不在线
    //告诉客户端A(原来发送请求的)发送失败
    _STRU_CHAT_RS rs;
    rs.result = _def_send_error;
    rs.friendId = rq->friendId;
     m_iocpServer->sendData((char*)&rs, sizeof(rs), from);

    // TODO:实际:应该把聊天的内容保存到数据中，在数据库中建一个新的表，保存发送用户的id、接收用)的id、发送时间、发送内容
    //每个用户上线成功的时候，从保存聊请求的表中查询，是否有发给当前上线用户的聊天请求
    //如果找到了把取出数据把这些聊天请发给上线用户，并从数据库中删除
  }
}

// 5）处理添加好友请求
void Ckernel::dealAddFriendRq(char* data, int len, unsigned long from) {
  cout << __func__ << '\n';
  // 1、拆包
  _STRU_ADD_FRIEND_RQ* rq = (_STRU_ADD_FRIEND_RQ*)data;

  // 2.根据好友的昵称查询好友的id
  char szSql[1024] = "";
  list<string> lstStr;
  sprintf_s(szSql, "select id from t_user where name='%s';", rq->friendName);
  if (!SQLselect(szSql, 1, lstStr)) {
    cout << "查询数据失败，sql:" << szSql << '\n';
    return;
  }
  // 3.判断查询结果
  _STRU_ADD_FRIEND_RS rs;
  if (0 == lstStr.size()) {  //为空
    //添加失败，好友不存在_def_add_friend_not_exists
    rs.result = _def_add_friend_not_exists;

    strcpy_s(rs.friendName, sizeof(rs.friendName), rq->friendName);
    //将添加结果发送回客户端A
     m_iocpServer->sendData((char*)&rs, sizeof(rs), from);

  } else {  //不为空
    //取出好友id
    int friendId = stoi(lstStr.front());
    lstStr.pop_front();
    // 4.判断好友是否在线
    if (m_mapIdToSocket.count(friendId) > 0) {  //在线
      //将添加好友请求直接转发给用户B
       m_iocpServer->sendData(data, len, m_mapIdToSocket[friendId]);
    } else {  //不在线
      //好友不在线，添加失败_def_add_friend_offlien
      rs.result = _def_add_friend_offlien;
      sprintf_s(rs.friendName, sizeof(rs.friendName), rq->friendName);
      //正常情况:把添加好友请求保存到数据库中，创建一个新的表保存用户A的id、A的昵称、B的昵称、添加时间
      //等B用户上线的时候，查询这个表，把添加B用户的这些信息写成添加好友结构体，发给B用户
      //然后，从数据库中删除添加B用户的这些信息

      //把添加结果发送回客户端A
       m_iocpServer->sendData((char*)&rs, sizeof(rs), from);
    }
  }
}
// 6)处理添加好友的回复
void Ckernel::dealAddFriendRs(char* data, int len, unsigned long from) {
  cout << __func__ << '\n';
  // 1、拆包
  _STRU_ADD_FRIEND_RS* rs = (_STRU_ADD_FRIEND_RS*)data;

  // 2.判断添加结果是否同意
  if (_def_add_friend_success == rs->result) {  //同意
    //把好友关系 写进数据库中
    char szSql[1024 * 2] = "";

    sprintf_s(szSql, "insert into t_friend values (%d,%d);", rs->friendId,
              rs->userId);

    if (!SQLupdate(szSql)) {
      cout << "插入数据库失败，sql:" << szSql << '\n';
      return;
    }

    sprintf_s(szSql, "insert into t_friend values (%d,%d);", rs->userId,
              rs->friendId);

    if (!SQLupdate(szSql)) {
      cout << "插入数据库失败，sql:" << szSql << '\n';
      return;
    }
    //刷新双端（客户端A、B）好友列表
    getUserInfoAndFriendInfo(rs->friendId);
  }

  // 3.把添加结果转发给客户端A
  if (m_mapIdToSocket.count(rs->friendId) > 0) {
     m_iocpServer->sendData(data, len, m_mapIdToSocket[rs->friendId]);
  }
}
