#include <QApplication>

#include "ckernel.h"

int main( int argc, char *argv[] ) {
  QApplication a( argc, argv );
  //  LoginDialog  w;
  //  w.show();

  CKernel kernel;
  return a.exec();
}

// QT的信号和槽机制(两个类之间，通知事件发生/传输数据)
// 1、两个类必须继承自Q0bject，并且有Q_OBJECT宏
// 2、在发送数据的类头文件中使用signals声明信号，信号不是函数，不需要实现，但是长的跟函数一样，返回值是void类型，参数就是要传输的数据,在cpp中需要传输数据的地方使用Q_EMIT/gemit
// 信号名(参数列表)来发送信号
// 3、在接收数据的类头文件中使用public/protected/private
// slots声明槽函数，槽函数的参数和返回值和信号一致，在cpp中实现槽函数
// 4、连接信号和槽函数，在接收数据的类中，发送信号的对象new出来以后进行连接
//信号和槽函数可以是多对多的关系，一个信号可以连接多个槽函数，一个槽函数也可以跟多个信号连接

//字符串类型:char*/char[]、std::string、Qstring
//基础类型可以直接给对象赋值
// std::string.c_str()转换成char*
// QString.tostdstring()转换成std::string.c_str()转换成char*

// VS使用gb2312编码方式，QT使用utf-8编码方式
//都在客户端转码，QT发给VS之前先转成gb2312，QT收到的数据转成utf-8
// 1、可能有中文的字符串 2、服务端需要使用这个字符串
// utf-8编码方式的字符串用QString保存
// gb2312编码方式的字符串用char*/char[]保存
