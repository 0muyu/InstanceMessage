#ifndef FRIENDFORM_H
#define FRIENDFORM_H

#include <QWidget>

namespace Ui {
class FriendForm;
}

class FriendForm : public QWidget
{
  Q_OBJECT
 signals:
  //通知Kernel显示与当前好友的聊天窗口
  void sig_showChatDialog( int friendId );

 public:
  explicit FriendForm(QWidget *parent = nullptr);
  ~FriendForm();
  //设置并保存好友属性
  void setFriendInfo( int id, QString name, QString feeling, int iconId,
                      int status );
  //设置好友下线
  void setFriendOffline();

  const QString &name() const;

 private slots:
  //点击头像（按下按钮），跳转到与好友的聊天框
  void on_pb_icon_clicked();

 private:
  Ui::FriendForm *ui;
  int             m_id;
  QString         m_name;
  QString         m_feeling;
  int             m_iconId;
  int             m_status;
};

#endif // FRIENDFORM_H
