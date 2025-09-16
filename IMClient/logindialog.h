#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginDialog; }
QT_END_NAMESPACE

class LoginDialog : public QDialog
{
  Q_OBJECT
 signals:
  //把注册信息发给Kernel
  void sig_registerMassage( QString name, QString tel, QString password );

  //把登录信息发给Kernel
  void sig_loginMassage( QString tel, QString password );

  //通知 Kernel退出进程
  void sig_closeProcess();

 public:
  LoginDialog(QWidget *parent = nullptr);
  ~LoginDialog();

  //重写父类的关闭事件
  void closeEvent( QCloseEvent *event );

 private slots:
  void on_pb_clear_login_clicked();

  void on_pb_commit_login_clicked();

  void on_pb_clear_register_clicked();

  void on_pb_commit_register_clicked();

 private:
  Ui::LoginDialog *ui;
};
#endif // LOGINDIALOG_H
