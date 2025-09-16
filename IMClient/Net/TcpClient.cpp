#include"TcpClient.h"
//#include"../Mediator/TcpClientMediator.h"
#include <QDebug>
TcpClient::TcpClient(INetMediator* pTcpClientMediator)
    : m_TcpClientHandle(nullptr) {
  m_pMediator = pTcpClientMediator;
}
 TcpClient::~TcpClient() {}

// 1.初始化网络（加载库、创建套接字、绑定IP地址和端口号、创建接收数据的线程）
 bool TcpClient::initNet() {
   qDebug() << "TcpClient::" << __func__;
   int err;
   // 1.加载库
   WORD    Version = MAKEWORD( 2, 2 );
   WSADATA Data;
   err = WSAStartup( Version, &Data );
   if ( 0 != err ) {
     cout << "WSAStartup fail" << '\n';
     return false;
   }

   //检测版本信息
   if ( 2 != HIBYTE( Data.wVersion ) || 2 != LOBYTE( Data.wVersion ) ) {
     cout << "WSAStratup Version error" << '\n';
     return false;
   } else {
     cout << "WSAStratup success" << '\n';
   }

   // 2.创建套接字(Ipv4 Tcp协议)

   m_sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
   if ( INVALID_SOCKET == m_sock ) {
     cout << "TcpClient::initNet socket failed with" << WSAGetLastError() << '\n';
     return false;
   } else {
     cout << "TcpClient::initNet socket success" << '\n';
   }

   // 3.连接服务端

   sockaddr_in ServerAddr;
   ServerAddr.sin_family           = AF_INET;
   ServerAddr.sin_port             = htons( _DEF_TCP_PORT );
   ServerAddr.sin_addr.S_un.S_addr = inet_addr( "127.0.0.1" );  //连接到服务端

   err = connect( m_sock, (sockaddr*) &ServerAddr, sizeof( ServerAddr ) );
   if ( SOCKET_ERROR == err ) {
     cout << "TcpClient connect faied with:" << WSAGetLastError() << '\n';
     return false;
   } else {
     cout << "TcpClient connect success" << '\n';
   }

   // 4.创建接收数据的线程
   m_TcpClientHandle = (HANDLE) _beginthreadex( nullptr, 0, TcpClientRecvThread, this, 0, nullptr );

   return true;
 }

 //接收数据的线程函数
 unsigned _stdcall TcpClient::TcpClientRecvThread(void* IpVoid) {
   TcpClient* pThis = (TcpClient*)IpVoid;
   pThis->recvData();
   return 1;
 }
 
// 2.关闭网络（回收线程资源、关闭套接字 、卸载库）
 void TcpClient::unInitNet() {
   qDebug() << "TcpClient::" << __func__;
   // 1.回收线程资源
   // 1.1结束线程
   // recvData结束while 循环
   m_bRunning = false;
   //不会立刻执行到while循环，需要等待一段时间
   if (WAIT_TIMEOUT == WaitForSingleObject(m_TcpClientHandle, 4000)) {
       //等待超时强制杀死对应线程
        TerminateThread(m_TcpClientHandle,-1);
     }

     //1.2关闭句柄
     CloseHandle(m_TcpClientHandle);
     m_TcpClientHandle=nullptr;


     //2.关闭套接字
     if (m_sock&&INVALID_SOCKET != m_sock) {
       closesocket(m_sock);
     }
     //3.卸载库
     WSACleanup();
 }

// 3.发送数据
 bool TcpClient::sendData( char* data, int len, unsigned long to ) {
   qDebug() << "TcpClient::" << __func__;
   // 1.处理无效值
   if (len <= 0 || !data) {
       cout << "TcpClient::sendData paramater error" << '\n';
     return false;
   }

   //2.发送数据：由于tcp发包会产生粘包问题，根据 筛选选取先发长度再发数据的方式
    int err;
   //2.1先发长度
    err = send(m_sock, (char*)&len, sizeof(int), 0);
    if (SOCKET_ERROR == err) {
    
    cout << "TcpClient::sendData_len failed with:" << WSAGetLastError() << '\n';
      return false; 
    }

    //2.2再发数据
    err = send(m_sock, data, len, 0);
    if (SOCKET_ERROR == err) {
        cout << "TcpClient::sendData_data falined with:" << WSAGetLastError() << '\n';
      return false;
    }

   return true;
 }

// 4.接收数据
 void TcpClient::recvData() {
   cout << "TcpClient::recvData..." << '\n';

    //同样的需要先接受包的长度再接收数据
   int nRecvNum = 0;
   //保存包的长度
   int packSize = 0;
   //保存每接收一个应用层数据包的长度（由于应用层最大包长度为64K,如果服务器发送的数据过大就需要拆包）
   int offset = 0;

   while (m_bRunning) {
        //每接收一个数据包offset置零，避免累计越界访问
     offset = 0;

       //1.接收数据包的长度
         nRecvNum = recv(m_sock, (char*)&packSize, sizeof(int), 0);

       if (nRecvNum > 0) {//成功接收包长度
            //按照包长度申请包空间
         char* recvBuf = new char[packSize];

         //考虑到可能发生拆包，因此需要循环接收数据，直到recvBuf被填充满，由于每次接收后剩余空间，和下次开始地址都会被修改，需要"动态"参数随时记录更新
         //2.接收1个包的内容
         while (packSize > 0 /**packsize记录当前空间剩余大小，当packsize=0的时候，空间满*/) {
           nRecvNum = recv(m_sock, recvBuf + offset, packSize, 0);
           if (nRecvNum > 0) {
             //更新记录值
             packSize -= nRecvNum;
             offset += nRecvNum;
           } else {
             cout << "TcpClient::recvData_content failed with:" << WSAGetLastError()
                  << '\n';
             break;
           }
         }
         //一个包数据接收完成
         // TOD0:把接收到的数据传给中介者类
         m_pMediator->transmitData(recvBuf, offset, m_sock);//???????????????????????????????????

         //测试代码:打印接收到的数据和数据长度
         /*cout << "TcpClient::recvData:" << recvBuf << "  "
              << "len:" << offset << '\n';*/
       } else {
         cout << "TcpClient::recvData_len failed with:" << WSAGetLastError()
              << '\n';
         break;
       }
   }
 }

 
