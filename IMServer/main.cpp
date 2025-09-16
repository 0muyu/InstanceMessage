#include"IOCPTcpServer/IOCPTCPServer.h"
#include"LFThreadPool/LFThreadPool.h"
#include "./mysql/SQLManager.h"
#include "CKernel.h"
#include "config.h"

std::mutex g_lock;

int main() {
   //创建线程池
  LFThreadPool pool(4);  // 线程池线程数


  IOCPTcpServer server(12345, 4, pool);  // IOCP端口 IOCP线程数

  Ckernel::instance().startServer(server);

  

  // 稍等一下，让所有任务跑完
  //while (true) {
  //  cout << "running..." << '\n';
  //  Sleep(5000);
  //}

  std::this_thread::sleep_for(std::chrono::seconds(100));




 /* struct _STRU_USER_INFO {
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

  std::cout << sizeof(_STRU_USER_INFO) << '\n';*/

  return 0;
}