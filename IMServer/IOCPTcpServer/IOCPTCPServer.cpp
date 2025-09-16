// IOCPTcpServer.cpp
#include "IOCPTcpServer.h"
#include"../LFThreadPool/LFThreadPool.h"

#pragma comment(lib, "Ws2_32.lib")

IOCPTcpServer::IOCPTcpServer(unsigned short port, int workerThreads, LFThreadPool& pool)
    : m_port(port),
      m_workerThreads(workerThreads),
      m_pool(pool),
      m_listenSock(INVALID_SOCKET),
      m_iocp(NULL) {
  WSADATA wsa;
  WSAStartup(MAKEWORD(2, 2), &wsa);
}

IOCPTcpServer::~IOCPTcpServer() {
  if (m_listenSock != INVALID_SOCKET) closesocket(m_listenSock);
  if (m_iocp) CloseHandle(m_iocp);
  WSACleanup();
}

bool IOCPTcpServer::run() {
  // 1) 创建 IOCP
  m_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
  if (!m_iocp) return false;

  // 2) 创建工作线程
  for (int i = 0; i < m_workerThreads; i++) {
    HANDLE h = CreateThread(NULL, 0, workerThread, this, 0, NULL);
    CloseHandle(h);//这里不是直接关闭线程，因为主线程没有对IOCP线程的特殊处理，因此不需要保留这些线程的句柄，只需要线程执行完后由内核回收；如果不关闭句柄可能会导致句柄泄露；
  }

  // 3) 创建监听 socket
  m_listenSock          = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in addr     = {};
  addr.sin_family      = AF_INET;
  addr.sin_port        = htons(m_port);//
  addr.sin_addr.s_addr = INADDR_ANY;
  bind(m_listenSock, (sockaddr*)&addr, sizeof(addr));
  listen(m_listenSock, SOMAXCONN);

  printf("IOCPTcpServer listening on port %u\n", m_port);

  // 4) accept 循环
  while (true) {
    std::cout << "run..." << '\n';
    //1.接受连接
    SOCKET client = accept(m_listenSock, NULL, NULL);
    if (client == INVALID_SOCKET) continue;

    // 2.连接成功，关联到IOCP（可u将多个套接字关联到同一个IOCP上）
     CreateIoCompletionPort((HANDLE)client, m_iocp, 0, 0);
     
    // 3.保存 client 到 map中
    {
      std::lock_guard<std::mutex> lk(m_clientsMutex);
      m_clients[(unsigned long)client] = client;
    }

    
    //4.为接收头部  创建 PER_IO，开始接收
    PER_IO* pio = new PER_IO();
    ZeroMemory(&pio->overlapped, sizeof(OVERLAPPED));
    pio->sock        = client;
    pio->op_type     = OP_RECV_HEADER;
    pio->body_buf    = nullptr;
    pio->sendOnly    = false;
    pio->wbuf[0].buf = pio->header_buf;
    pio->wbuf[0].len = sizeof(UINT32);


    // 2.连接成功，关联到IOCP（可u将多个套接字关联到同一个IOCP上）
    //CreateIoCompletionPort((HANDLE)client, m_iocp, (ULONG_PTR)pio, 0);

    DWORD flags = 0;
    WSARecv(client, &pio->wbuf[0], 1, NULL, &flags, &pio->overlapped, NULL);
    printf("Client connected: %u\n", (unsigned long)client);
  }

  return true;
}

void IOCPTcpServer::postRecvHeader(PER_IO* pio) {
  ZeroMemory(&pio->overlapped, sizeof(OVERLAPPED));
  pio->op_type     = OP_RECV_HEADER;
  pio->wbuf[0].buf = pio->header_buf;
  pio->wbuf[0].len = sizeof(UINT32);
  DWORD flags      = 0;
  WSARecv(pio->sock, &pio->wbuf[0], 1, NULL, &flags, &pio->overlapped, NULL);
}

DWORD WINAPI IOCPTcpServer::workerThread(LPVOID arg) {
  // void IOCPTcpServer::workerThread(LPVOID arg) {
  IOCPTcpServer*  self = (IOCPTcpServer*)arg;
  DWORD       bytes;
  ULONG_PTR   key;
  OVERLAPPED* ov;

  while (GetQueuedCompletionStatus(self->m_iocp, &bytes, &key, &ov, INFINITE)) {
    PER_IO* pio = CONTAINING_RECORD(ov, PER_IO, overlapped);//ov 是某个 PER_IO 结构体中 overlapped 成员的地址。请帮我计算出这个 PER_IO 结构体的起始地址是多少，并把指针赋给 pio
    if (bytes == 0) {
      // 客户端断开
      self->closeClient(pio);
      continue;
    }
    if (pio->op_type == OP_RECV_HEADER) {
      // 收完 4 字节头
      if (bytes != sizeof(UINT32)) {
        self->closeClient(pio);
        continue;
      }

      //1.1 设置IO上下文
      pio->body_len = *(int*)pio->header_buf;

      // TODO ^aka
      // UINT32 netlen; 
      // memcpy(&netlen, pio->header_buf, sizeof(UINT32));
      // pio->body_len = ntohl(netlen);
      if (pio->body_len == 0 || pio->body_len > 10 * 1024 * 1024) {
        self->closeClient(pio);
        continue;
      }
      // 准备接收 body
      pio->body_buf = (char*)malloc(pio->body_len);
      ZeroMemory(&pio->overlapped, sizeof(OVERLAPPED));
      pio->op_type     = OP_RECV_BODY;
      pio->wbuf[0].buf = pio->body_buf;
      pio->wbuf[0].len = pio->body_len;

      //1.2 接收数据包内容
      DWORD flags      = 0;
      int   r          = WSARecv(pio->sock, &pio->wbuf[0], 1, NULL, &flags, &pio->overlapped, NULL);
      if (r == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
        self->closeClient(pio);
      }
    } 
    else if (pio->op_type == OP_RECV_BODY) {

      //2.1 收完 body，打包任务提交线程池
      if ((DWORD)bytes != pio->body_len) {
        self->closeClient(pio);
        continue;
      }
      // 深拷贝一份给任务
      char* userBuf = (char*)malloc(pio->body_len);
      memcpy(userBuf, pio->body_buf, pio->body_len);
      TaskData task{userBuf, (int)pio->body_len, (unsigned long)pio->sock};
      self->m_pool.submit(task);

      //2.2 继续获取新数据包
      // 释放 IO 缓冲，并继续接头
      free(pio->body_buf);
      pio->body_buf = nullptr;
      self->postRecvHeader(pio);
    } 
    else if (pio->op_type == OP_SEND && pio->sendOnly) {
      // 发送完成，释放内存
      free(pio->body_buf);
      delete pio;
    }
  }

  return -1;
}

void IOCPTcpServer::closeClient(PER_IO* pio) {
  printf("Client disconnected: %u\n", (unsigned long)pio->sock);
  closesocket(pio->sock);
  {
    std::lock_guard<std::mutex> lk(m_clientsMutex);
    m_clients.erase((unsigned long)pio->sock);
  }
  if (pio->body_buf) free(pio->body_buf);
  delete pio;
}

void IOCPTcpServer::sendData( const char* data, int len,
                             unsigned long clientId) {
  std::lock_guard<std::mutex> lk(m_clientsMutex);

  //1.取出客户端socket
  auto it = m_clients.find(clientId);
  if (it == m_clients.end()) return;  // 客户端已断开
  SOCKET s = it->second;

  //2.新建一个 send-only 的 PER_IO
  PER_IO* pio = new PER_IO();
  ZeroMemory(&pio->overlapped, sizeof(OVERLAPPED));
  pio->sock     = s;
  pio->op_type  = OP_SEND;
  pio->sendOnly = true;
  pio->body_len = len;
  // header !TODO
  //UINT32 netlen = htonl(len);
  //memcpy(pio->header_buf, &netlen, sizeof(UINT32));
  memcpy(pio->header_buf, &len, sizeof(UINT32));

  // body
  pio->body_buf = (char*)malloc(len);
  memcpy(pio->body_buf, data, len);
  // WSABUF
  pio->wbuf[0].buf = pio->header_buf;
  pio->wbuf[0].len = sizeof(UINT32);
  pio->wbuf[1].buf = pio->body_buf;
  pio->wbuf[1].len = len;

  //3.异步发送
  DWORD sent = 0;
  int   r    = WSASend(s, pio->wbuf, 2, &sent, 0, &pio->overlapped, NULL);
  if (r == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
    // 发送失败，马上清理
    free(pio->body_buf);
    delete pio;
  }
}