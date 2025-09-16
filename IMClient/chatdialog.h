#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
  Q_OBJECT
 signals:
  //把聊天内容发送给Kernel
  void sig_sendMessage( int friendId, QString content );

 public:
  explicit ChatDialog(QWidget *parent = nullptr);
  ~ChatDialog();

  // 1.保存并设置聊天窗口的属性
  void setDialogInfo( int friendId, QString name );

  // 2.将接收到的好友聊天内容显示到窗口上
  void setChatContent( QString content );

  // 3.显示好友不在线
  void showFriendOffline();

 private slots:
  void on_pb_send_clicked();

 private:
  Ui::ChatDialog *ui;
  int             m_id;
  QString         m_name;
};

#endif // CHATDIALOG_H
