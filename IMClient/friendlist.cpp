#include "friendlist.h"

#include <QCloseEvent>
#include <QDebug>
#include <QMessageBox>

#include "ui_friendlist.h"
FriendList::FriendList( QWidget *parent ) :
    QDialog( parent ), ui( new Ui::FriendList ) {
  ui->setupUi(this);
  // new一个垂直布局层的对象
  m_pLayout = new QVBoxLayout;

  //设置小控件之间的间隙
  m_pLayout->setSpacing( 3 );
  //设置小控件和大控件之间的间隙
  m_pLayout->setContentsMargins( 0, 0, 0, 0 );
  //把层设置到大控件上
  ui->wdg_list->setLayout( m_pLayout );

  // new一个菜单栏对象
  m_pMenu = new QMenu;
  m_pMenu->addAction( "添加好友" );
  m_pMenu->addAction( "系统设置" );

  //连接点击菜单栏项的信号和槽函数
  connect( m_pMenu, &QMenu::triggered, this, &FriendList::slot_triggered );
}

FriendList::~FriendList()
{
  delete ui;
}

//增加一个好友
void FriendList::addFriend( FriendForm *form ) { m_pLayout->addWidget( form ); }

//设置登录用户信息
void FriendList::setUserInfo( QString name, QString feeling, int iconId ) {
  ui->lb_name->setText( name );
  ui->lb_feeling->setText( feeling );

  //拼接头像路径
  QString path = QString( ":/tx/%1.png" ).arg( iconId );
  QIcon   icon = QIcon( path );
  ui->pb_icon->setIcon( icon );
}

//重写父类的关闭事件
void FriendList::closeEvent( QCloseEvent *event ) {
  // 1.忽略事件
  event->ignore();

  // 2.通知Kernel下线
  if ( QMessageBox::Yes
       == QMessageBox::question( this, "询问", "是否确定退出程序" ) ) {
    Q_EMIT sig_offlien();
  }
}
//在鼠标的点击的位置向上显示一个菜单栏
void FriendList::on_pb_menu_clicked() {
  //获取鼠标的点击位置
  QPoint pos = QCursor::pos();
  //获取菜单栏的绝对大小
  QSize size = m_pMenu->sizeHint();
  //在点击位置向上显示菜栏
  m_pMenu->exec( QPoint( pos.x(), pos.y() - size.height() ) );
}
//点击菜单栏项目后进行处理的槽函数
void FriendList::slot_triggered( QAction *action ) {
  //判断用户点击的是哪一个菜单项
  if ( "添加好友" == action->text() ) {
    //通知Lernel添加好友
    Q_EMIT sig_addFriend();

  } else if ( "系统设置" == action->text() ) {
    qDebug() << "系统设置";
  }
}
