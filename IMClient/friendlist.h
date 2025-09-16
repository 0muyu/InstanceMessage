#ifndef FRIENDLIST_H
#define FRIENDLIST_H

#include <QDialog>
#include <QMenu>
#include <QVBoxLayout>

#include "friendform.h"
namespace Ui {
class FriendList;
}

class FriendList : public QDialog
{
  Q_OBJECT
 signals:
  //通知Kernel下线
  void sig_offlien();
  //通知Kernel添加好友
  void sig_addFriend();

 public:
  explicit FriendList(QWidget *parent = nullptr);
  ~FriendList();

  //增加一个好友
  void addFriend( FriendForm *form );

  //设置登录用户信息
  void setUserInfo( QString name, QString feeling, int iconId );

  //重写父类的关闭事件
  void closeEvent( QCloseEvent *event );

 private slots:
  //在鼠标的点击的位置向上显示一个菜单栏
  void on_pb_menu_clicked();

  //点击菜单栏项目后进行处理的槽函数
  void slot_triggered( QAction *action );

 private:
  Ui::FriendList *ui;

  //定义垂直布局的层
  QVBoxLayout *m_pLayout;

  //定义菜单栏
  QMenu *m_pMenu;
};

#endif // FRIENDLIST_H
