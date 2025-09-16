#include "friendform.h"

#include <QBitmap>

#include "Net/config.h"
#include "ui_friendform.h"
FriendForm::FriendForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendForm)
{
  ui->setupUi(this);
}

FriendForm::~FriendForm() { delete ui; }
//设置并保存好友属性
void FriendForm::setFriendInfo( int id, QString name, QString feeling,

                                int iconId, int status ) {
  // 1.设置好友属性
  m_id      = id;
  m_name    = name;
  m_feeling = feeling;
  m_iconId  = iconId;
  m_status  = status;

  // 2.保存好友属性
  ui->lb_name->setText( m_name );
  ui->lb_feeling->setText( m_feeling );
  //拼接头像图片路径
  QString path = QString( ":/tx/%1.png" ).arg( m_iconId );
  //根据状态显示头像
  if ( _def_status_online == m_status ) {
    //在线，亮显示
    QIcon icon = QIcon( path );
    ui->pb_icon->setIcon( icon );
  } else {
    //不在线，暗显示
    QBitmap bp;
    bp.load( path );
    ui->pb_icon->setIcon( bp );
  }
}

//设置好友下线
void FriendForm::setFriendOffline() {
  //设置下线状态
  m_status = _def_status_offline;

  //头像变暗
  //拼接头像图片路径
  QString path = QString( ":/tx/%1.png" ).arg( m_iconId );
  //不在线，暗显示
  QBitmap bp;
  bp.load( path );
  ui->pb_icon->setIcon( bp );

  //重绘
  repaint();
}

//点击头像（按下按钮），跳转到与好友的聊天框
//聊天框的对象在Ckernel里，因此需要发送信号到Kernel里进行处理
void FriendForm::on_pb_icon_clicked() {
  //通知Kernel显示与当前好友的聊天窗口
  Q_EMIT sig_showChatDialog( m_id );
}

//获取成员变量m_name
const QString &FriendForm::name() const { return m_name; }
