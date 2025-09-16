#include "logindialog.h"

#include <QMessageBox>

#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
  ui->setupUi(this);
}

LoginDialog::~LoginDialog() { delete ui; }

//重写父类的关闭事件
void LoginDialog::closeEvent( QCloseEvent *event ) {
  //通知Kernel退出进程
  Q_EMIT sig_closeProcess();
}

//清空登录信息：电话号、密码
void LoginDialog::on_pb_clear_login_clicked() {
  ui->le_tel_login->setText( "" );
  ui->le_password_login->setText( "" );
}

//登录信息：电话号、密码
void LoginDialog::on_pb_commit_login_clicked() {
  // 1、从控件上获取用户输入的数据
  QString tel      = ui->le_tel_login->text();
  QString password = ui->le_password_login->text();

  QString telCpy      = tel;
  QString passwordCpy = password;

  // 2、校验字符串合法性
  // 2.1判断字符串是否为空或者是全空格
  telCpy.remove( " " );
  passwordCpy.remove( " " );
  if ( telCpy.isEmpty() || passwordCpy.isEmpty() ) {
    QMessageBox::about( this, "提示", "输入内容不能为空或全为空格!" );
    return;
  }
  // 2.2判断字符串长度是否合法(手机号必须是11位,密码必须小于20)
  if ( tel.length() != 11 || password.length() >= 20 ) {
    QMessageBox::about( this, "提示", "手机号必须是11位,密码必须小于20" );
    return;
  }
  // 2.3判断字符串内容是否合法(手机号必须是全数字，以1开头，昵称和密码必须是字母、数字、下划线)-正则表达式的时候写

  // 3、把登录信息发给kernel
  Q_EMIT sig_loginMassage( tel, password );
}

//清空注册信息：手机号、密码、昵称
void LoginDialog::on_pb_clear_register_clicked() {
  ui->le_tel_register->setText( "" );
  ui->le_password_register->setText( "" );
  ui->le_nicname_register->setText( "" );
}
//注册信息：手机号、密码、昵称
void LoginDialog::on_pb_commit_register_clicked() {
  // 1、从控件上获取用户输入的数据
  QString name     = ui->le_nicname_register->text();
  QString tel      = ui->le_tel_register->text();
  QString password = ui->le_password_register->text();

  QString nemeCpy     = name;
  QString telCpy      = tel;
  QString passwordCpy = password;

  // 2、校验字符串合法性
  // 2.1判断字符串是否为空或者是全空格
  //判断全空格就是先把字符串中的空格移除，然后判断是否是空字符串
  // remove的作用：remove会改变字符串的内容，例如"12 34",调用完remove("
  // ")数会变成"1234"，改变了原来的内容
  nemeCpy.remove( " " );
  telCpy.remove( " " );
  passwordCpy.remove( " " );
  if ( nemeCpy.isEmpty() || telCpy.isEmpty() || passwordCpy.isEmpty() ) {
    QMessageBox::about( this, "提示", "输入内容不能为空或者全为空格！" );
    return;
  }

  // 2.2判断字符串长度是否合法(手机号必须是11位，昵称必须小于20，密码必须小于20)
  if ( name.length() >= 20 || tel.length() != 11 || password.length() >= 20 ) {
    QMessageBox::about( this, "提示",
                        "机号必须是11位，昵称必须小于20，密码必须小于20" );
    return;
  }

  // 2.3判断字符串内容是否合法(手机号必须是全数字，以1开头，昵称和密码必须是字母、数字、下划线)-正则表达式的时候写

  // 3、把注册信息发给kernel
  Q_EMIT sig_registerMassage( name, tel, password );
}
