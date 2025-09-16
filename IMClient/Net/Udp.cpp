#include"Udp.h"
//#include"../Mediator/UdpMediator.h"

  Udp::Udp(INetMediator* pUdpMediator):m_handle(nullptr){
  m_pMediator = pUdpMediator;
  };
  Udp::~Udp() {
 };
    // 1.初始化网络(加载库、创建套接字、绑定IP地址和端口号、创建接收数据的线程)
bool Udp::initNet() {
   int err;
    //1.加载库
  WORD Version=MAKEWORD(2,2);
    WSADATA Data;
  err = WSAStartup(Version, &Data);

  if (err != 0) {
    cout << "WSAStratup fali" << '\n';
    return false;
  }

  //判断版本号
  if (HIBYTE(Data.wVersion) != 2 || LOBYTE(Data.wVersion) != 2) {
    cout << "WSAStartup  Version error" << '\n';
    return false;
  } else {
    cout << "WSAStartup  success" << '\n';
  }
    //2.创建套接字
  m_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (m_sock == INVALID_SOCKET) {
    cout << "socket failed wiht: " << WSAGetLastError() << '\n';
    return false;
  
  } else {
    cout << "socket success " << '\n';
  }
    //3.绑定IP地址和端口号
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(_DEF_UDP_PORT);
  addr.sin_addr.S_un.S_addr = ADDR_ANY;
  err = bind(m_sock, (sockaddr * ) & addr, sizeof(addr));
  if (err == SOCKET_ERROR) {
    cout << "Udp bind failed wiht: " << WSAGetLastError() << '\n';
    return false;
  } else {
    cout << "Udp bind success " << '\n';
  }

    
  //4.创建接收数据的线程
  //因为CreateThread()和ExitThread()是一对，如果使用Createlhread()创建线程，那么线程结束的时候，操作系统会自动调用ExitThreadO)退出线程
  //如果线程中使用了C++运行时库的函数(例如strcpy)，这些函数会申请空间但是不释放，ExitThread()退出线程的时候也不会回收空间，就会造成内存泄漏
  //_beginthreadex()和_endthreadex()是一对，_endthreadex()会先回收空间，再调用ExitThread()
    
  m_handle=(HANDLE)_beginthreadex(nullptr /*线程安全级别,nullptr代表使用默认安全级别*/,
                 0 /*线程堆栈大小，0——代表使用默认大小1M*/,
                 recvThread/*线程函数的起始地址 */,
     this /*线程函数的参数列表*/,
      0 /*初始化标志，0——创建即运行，CREATE_SUSPENDED——挂起线程，调用ResumeThread()函数的时候才运行*/
      ,
      nullptr/*操作系统 分配的线程id,是输出参数，如果需要可定义变量接收，否者传入nullptr*/);

    return true; }

//(5)接收数据的线程函数
unsigned _stdcall Udp::recvThread(void* IpVoid) {
  Udp* pThis = (Udp*)IpVoid;
  pThis->recvData();

  return 1;
}


// 2.关闭网络(回收线程资源 、关闭套接字、卸载库)
void Udp::unInitNet() {
    //1.回收线程资源
    //创建线程的时候，操作系统会给每个线程分配3个资源:线程id，线程句柄，内核对象，引用计数器是2
    //回收线程资源需要使引用计数器变成0，结東线程工作会使引用计数器-1，关闭句柄会使引用计数器-1
  if (m_handle) {
      // 1.1、结束线程
      //让recvData退出while循环
      m_bRunning = false;
         // bRunning变成false以后，循环并不一定立刻就能执行到判断变量值的那行代码，所以需要等待一段时间，再判断线程是否结束

      if (WAIT_TIMEOUT /*等待超时*/ ==
          WaitForSingleObject(m_handle /*等待的句柄*/,
                              5000 /*等待多长时间,单位毫秒*/)) {
        ////如果线程没有结束，就强制杀死线程，但是不要一开始就强制杀死
        TerminateThread(m_handle /*杀死哪个线程*/, -1 /*退出码*/);
      }

      // 1.2、关闭句柄
      CloseHandle(m_handle);
      m_handle = nullptr;
  }

    //2.关闭套接字
  if (m_sock && m_sock != INVALID_SOCKET) {
      closesocket(m_sock);
  }
    //3.卸载库
  WSACleanup();
}



// 3.发送数据
bool Udp::sendData(char* data, int len, unsigned long to) { 
    cout << "Udp::sendData..." << '\n';
   //1.处理无效值
  if (len <= 0 || !data) {
     return false;
  }

  //2.发送数据
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(_DEF_UDP_PORT);
  addr.sin_addr.S_un.S_addr = to;
  if (sendto(m_sock, data, len, 0, (sockaddr*)&addr, sizeof(addr))<0) {
    cout << "Udp::sendData failed with:" << WSAGetLastError() << '\n';
    return false;
  }

    return true; }

// 4.接收数据
void Udp::recvData() { 

    cout << "Udp::recvData..." << '\n'; 
    int nRecvNum = 0;
    char recvBuf[8 * 1024];

    //输出参数
    sockaddr_in addrFrom = {};
    int addrFromSize=sizeof(addrFrom);
    
    while (m_bRunning) {
      cout << "Udp::recvData while...." << '\n';
      nRecvNum = recvfrom(m_sock, recvBuf, sizeof(recvBuf), 0,
                          (sockaddr*)&addrFrom, &addrFromSize);
      if (nRecvNum > 0) {
        //接收数据成功，需要处接收到的数据

        // 1.new一个新空间，将新空间的内容传给处理数据函数，以免有新数据来将原空间的数据覆盖，并在处理数据的函数中回收这个空间
        char* temp = new char[nRecvNum];

        // 2.将数据拷贝到新空间
        memcpy(temp, recvBuf, nRecvNum);

        // TODO:把接收到的数据传给中介者类
        m_pMediator->transmitData(temp,nRecvNum,addrFrom.sin_addr.S_un.S_addr);
        //测试代码：打印接收到的数据——一般在某些功能目前无法实现的情况下，通过这种方式测试代码的正确性，在实现完对应功能后，将测试代码删除
        //cout << "Udp::recvData :" << temp << '\n';
      } else {
        cout << "Udp::recvData failed with:" << WSAGetLastError() << '\n';
        break;
      }
    }
    }



