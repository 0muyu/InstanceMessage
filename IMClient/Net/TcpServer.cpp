#include"TcpServer.h"
//#include"../Mediator/TcpServerMediator.h"
#include <QDebug>

TcpServer ::TcpServer(INetMediator* pTcpServerMediator) {
  m_pMediator = pTcpServerMediator;
}

 TcpServer::~TcpServer() {}

// 1.初始化网络(加载库、创建套接字、绑定IP地址和端口号、创建接收数据的线程)
 bool TcpServer::initNet() {
   qDebug() << "TcpServer::" << __func__;
   int err;
   // 1.加载库
   WORD    Version = MAKEWORD( 2, 2 );
   WSADATA Data;
   err = WSAStartup( Version, &Data );
   if ( err != 0 ) {
     
     cout << "WSAStartup fail" << '\n';
       return false;
     }

     //检测版本信息
     if (HIBYTE(Data.wVersion) != 2 || LOBYTE(Data.wVersion) != 2) {
     
     cout << "WSAStratup version error:" << '\n'; 
     return false;
     } else {
     
     cout << "WSAStartup success" << '\n';
     }

     //2.创建套接字(Ipv4 tcp协议)
     m_sock= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
     if (INVALID_SOCKET == m_sock) {
     
     cout << "TcpServer listenSocket failed with:" << WSAGetLastError() << '\n';
       return false;
     } else {
       cout << "TcpServer listenSocket success" << '\n';
     }

     //3.绑定IP地址和端口号
     sockaddr_in serverAddr;
     serverAddr.sin_family = AF_INET;
     serverAddr.sin_port = htons(_DEF_TCP_PORT);
     serverAddr.sin_addr.S_un.S_addr = ADDR_ANY;
     err = bind(m_sock,(sockaddr*)&serverAddr,sizeof(serverAddr));

     if (SOCKET_ERROR == err) {
    cout << "TcpServer bind failed with: " << WSAGetLastError() << '\n';
    return false;
     }
     else {
    cout << "TcpServer bind success" << '\n';
     }

     //4.监听
     err=listen(m_sock,50);
     if (err == SOCKET_ERROR) {
        cout << "TcpServer listen failed with: " << WSAGetLastError() << '\n';
      return false;
     }
     else {
        cout << "TcpServer listen succes" << '\n';
     }


     //5.创建接受连接的线程(只有一个，只是后续还有接收数据的线程，放在一个链表中方便管理 )
     HANDLE handle=
         (HANDLE)_beginthreadex(nullptr, 0, acceptThread, this, 0, nullptr);
     if (handle) {
       m_listHandle.push_back(handle);
     }

     return true;
 }

 //(5)接受连接的线程的线程函数(循环接受连接，连接产生的多个socket保存起来)
 unsigned _stdcall TcpServer::acceptThread(void* Ipvoid) {
   TcpServer* pThis = (TcpServer*)Ipvoid;

   SOCKET clientSock = INVALID_SOCKET;
   sockaddr_in clientAddr = {};
   int addrLen = sizeof(clientAddr);

   //循环接收
   while (pThis->m_bRunning) {
     
     clientSock = accept(pThis->m_sock, (sockaddr*)&clientAddr, &addrLen);
     if (INVALID_SOCKET == clientSock) {
       cout << "TcpServer accept failed with:" << WSAGetLastError() << '\n';
       //等待接受下一次连接
       //return 0;
     } else {//连接成，收集套接字
         //1.连接成功，打印连接信息
       cout << "client Ip:" << inet_ntoa(clientAddr.sin_addr) << '\n';

       cout << "TcpServer collect clientSock..." << '\n';

         //2.给连接成功的客户端创建一个接收数据的线程
       unsigned int handId;
       HANDLE handle =(HANDLE)_beginthreadex(nullptr, 0, recvThread, pThis, 0, &handId);
       if (handle) {
        pThis-> m_listHandle.push_back(handle);
       }
       
        //3.保存连接成功的客户端的socket和线程id，线程id是key
         pThis->m_mapHandidToSocket[handId] = clientSock;
       
     }
   }
   return 1;
 }
 //接收数据的线程函数
 unsigned _stdcall TcpServer::recvThread(void* IpVoid) {
   TcpServer* pThis = (TcpServer*)IpVoid;
   pThis->recvData();
   return 1;
 }



// 2.关闭网络(关闭套接字、卸载库)
 void TcpServer::unInitNet() {
   qDebug() << "TcpServer::" << __func__;
   // 1、回收线程资源
   if ( m_bRunning ) { m_bRunning = false; }
   for ( HANDLE handle : m_listHandle ) {
     if ( handle ) {
       // 1.关闭线程
       if ( WAIT_TIMEOUT == WaitForSingleObject( handle, 6000 ) ) {
         //强制关闭
         TerminateThread( handle, -1 );
       }
       // 1.2关闭 句柄
       CloseHandle( handle );
       handle = nullptr;
     }
   }

   // 2、关闭套接字
   // 2.1m_sock
   if (m_sock&& m_sock != INVALID_SOCKET) {
     closesocket(m_sock);
   }

   //2.2
   for (pair<unsigned int, SOCKET> p : m_mapHandidToSocket) {
     closesocket(p.second);
    m_mapHandidToSocket.erase(p.first);
   }
   // 3、卸载库
   WSACleanup();
 }

// 3.发送数据
// char* data: 数据内容
// int len:数据长度
// unsigned long to:数据发给谁
// TCP协议，SOCKET决定了发给谁，SOCKET实际是UINT_PTR类型
// UDP协议，IP地址决定了发给谁，IP是ULONG类型
  bool TcpServer::sendData(char* data, int len, unsigned long to) {
    qDebug() << "TcpServer::" << __func__;
    // 1、校验参数合法性
    if ( len <= 0 || !data ) {
      cout << "TcpServer::sendData paramater error" << '\n';
      return false;
   }

   // 2、发送数据
   int err;
   // 2.1发送包长度len
   err = send(to, (char*)&len, sizeof(int), 0);
   if (SOCKET_ERROR == err) {
     cout << "TcpServer::sendData_Len failed with:" << WSAGetLastError()
          << '\n';
     return false;
   }

     // 2.2、发送包内容data
     err = send(to, data, len, 0);
     if (SOCKET_ERROR == err) {
       cout << "TcpServer::sendData_Data failed with:" << WSAGetLastError()
            << '\n';
       return false;
     }
      return true;
  }

// 4.接收数据
 void TcpServer::recvData() {
   qDebug() << "TcpServer::" << __func__;
   //休眠一会，recvThread是创建即运行的线程，
   //如果不休眠一会，acceptThread中保存socket到map的代码可能还没有运行到
   // recvThread中线程id获取当前线程对应的socket的代码就不能取出正确的socket
   Sleep(5);

   //获取当前线程id
   unsigned int threadId = GetCurrentThreadId();

   //根据线程id取出当前线程对应的socket
      SOCKET sock = INVALID_SOCKET;
      //获取socket前需要确定当前map中是否存在对应键值，或者键值对应的socket内容是否有效
      if (m_mapHandidToSocket.count(threadId) > 0) {
        sock = m_mapHandidToSocket[threadId];
      }
      
      //判断获取到的socket合法性
      if (!sock || INVALID_SOCKET == sock) {
        cout << "TcpServer::recvData error " << '\n';
      }


      //接收数据
      int nRecvNum = 0;
      int packSize = 0;
      int offset = 0;
      while (m_bRunning) {
            offset = 0;

          //1.数据包长度
         nRecvNum = recv(sock,(char*)&packSize,sizeof(int),0);
         
          if (nRecvNum > 0) {//成功接收到长度
              //按照接收到的长度申请数据空间
            char* recvBuf = new char[packSize];
            //接收一个数据包
            while (packSize) {//packSize:剩余接收空间
              nRecvNum = recv(sock, recvBuf + offset, packSize, 0);
              if (nRecvNum > 0) {//成功接收数据
                   //更新偏移量和剩余空间
                    offset += nRecvNum;
                   packSize -= nRecvNum;
              } else {//
                cout << "TcpServer::recvData_Data failed with:"
                     << WSAGetLastError() << '\n';
                break;
              }
            }

            //接收完一个包的数据
            // TOD0:把接收到的数据传给中介者类
            m_pMediator->transmitData(recvBuf, offset, sock);

            //测试代码:打印接收到的数据和数据长度
            /*cout << "TcpServer::recvData :"
                 << recvBuf << "  " << "len:" << offset << '\n';*/

            //给客户端回消息
            char s[] = "This is TcpServer:haha";
            sendData(s,sizeof(s),sock);

          } else {//接收长度失败
            cout << "TcpServer::sendData_len failed with:" << WSAGetLastError()
                 << '\n';
            break;
          }
      }
 }
