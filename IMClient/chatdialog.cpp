#include "chatdialog.h"

#include <QTime>

#include "ui_chatdialog.h"
ChatDialog::ChatDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatDialog)
{
  ui->setupUi(this);
}

ChatDialog::~ChatDialog()
{
  delete ui;
}

// 1.保存并设置聊天窗口的属性
void ChatDialog::setDialogInfo( int friendId, QString name ) {
  //保存
  m_id   = friendId;
  m_name = name;

  //设置
  setWindowTitle( QString( "与【%1】的聊天窗口" ).arg( m_name ) );
}
// 2.将接收到的好友聊天内容显示到窗口上
void ChatDialog::setChatContent( QString content ) {
  // 把内容显示到上面的浏览窗口
  ui->tb_chat->append(
      QString( "【%1】%2" )
          .arg( m_name )
          .arg( QTime::currentTime().toString( "hh:mm:ss" ) ) );
  ui->tb_chat->append( content );
}
// 3.显示好友不在线
void ChatDialog::showFriendOffline() {
  ui->tb_chat->append(
      QString( "【%1】%2不在线" )
          .arg( m_name )
          .arg( QTime::currentTime().toString( "hh:mm:ss" ) ) );
}

void ChatDialog::on_pb_send_clicked() {
  // 1.获取用户输入内容（出文本格式）
  QString content = ui->te_chat->toPlainText();
  // 2.判断输入数据是否是空或全空格
  content.remove( " " );
  if ( content.isEmpty() ) {
    ui->te_chat->setText( "" );
    return;
  }
  // 3.获取用户输入的内容（带格式）
  content = ui->te_chat->toHtml();
  // 4.清空下面的编辑窗口
  ui->te_chat->setText( "" );
  // 5.把内容显示到上面的浏览窗口
  ui->tb_chat->append(
      QString( "【我】%1" )
          .arg( QTime::currentTime().toString( "hh:mm:ss" ) ) );
  ui->tb_chat->append( content );
  // 6.把聊天 内容发送给Kernel
  Q_EMIT sig_sendMessage( m_id, content );
}
