// IOCPTcpServer.h
#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <unordered_map>

// 前向声明
class LFThreadPool;

typedef enum { OP_RECV_HEADER,
               OP_RECV_BODY,
               OP_SEND } OP_TYPE;


class IOCPTcpServer {
 public:
  // port: 监听端口
  // workerThreads: IOCP 工作线程数
  // pool: 用户传入的线程池引用
  IOCPTcpServer(unsigned short port, int workerThreads, LFThreadPool& pool);
  ~IOCPTcpServer();

  // 启动并进入 accept 循环（阻塞）
  bool run();

  // 在用户线程池里处理完业务后，如需发数据调用此接口
  // clientId 即 TaskData.from
  void sendData(const char* data, int len, unsigned long clientId);

 private:
  // 每次一个 IO 操作的上下文
  struct PER_IO {
    OVERLAPPED overlapped;
    OP_TYPE    op_type;
    SOCKET     sock;
    WSABUF     wbuf[2];
    UINT32     body_len;
    char       header_buf[sizeof(UINT32)];
    char*      body_buf;
    bool       sendOnly;  // true 表示仅用于发送，完成后释放
  };

  void postRecvHeader(PER_IO* pio);
  void closeClient(PER_IO* pio);
  static DWORD WINAPI workerThread(LPVOID arg);

  unsigned short m_port;
  int            m_workerThreads;
  LFThreadPool&  m_pool;
  SOCKET         m_listenSock;
  HANDLE         m_iocp;
  std::mutex     m_clientsMutex;

  std::unordered_map<unsigned long, SOCKET> m_clients;
};