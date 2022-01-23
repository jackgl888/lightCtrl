
#include"server.h"




Server::Server(QObject *parent) :
    QTcpServer(parent)

{

    /* get current dialog object */
      m_Window = dynamic_cast<updateMainwindow *>(parent);
  //  qDebug()<<"parent:"<<this->parent();

}

Server::~Server()
{

}



void Server::incomingConnection(int sockDesc)
{

    QString  time =   QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ");
    QString  text = QString("%1%2%3").arg(time).arg(QString::number(sockDesc)).arg("客户端接入");
    m_Window->msgBrowser->append(text);

    serverThread *thread = new serverThread(sockDesc,0,0x01,0x00);//服务端等待客户端连入

    //线程管理
    connect(thread, SIGNAL(disconnectTCP(const QString &)), this, SLOT(clientDisconnected(const QString &)));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

   //发送数据

    connect(m_Window, SIGNAL(startSend(QString,int,uchar,ushort,ushort)),
            thread, SLOT( sendDataSlot(QString,int,uchar,ushort,ushort)));
    //同步数据到线程
    connect(m_Window,SIGNAL(sig(QVariant)),thread,SLOT(slotGetData(QVariant)));

    //界面更新来自串口的数据
    connect(thread,SIGNAL( bedUpdate(const uchar,const uchar,const QString &)),m_Window,
           SLOT(slotUpdateCmdData(const uchar,const uchar,const QString &)));

    connect(m_Window,SIGNAL(sigdatasent(uchar,uchar)),thread,
            SLOT(slotSendUpdateCmd(uchar,uchar))); //ui命令到线程

    //主线程将item状态改变传给线程更新msgmodle
    connect(m_Window,SIGNAL(sigItemChanged(QString,QString)),thread,SLOT(slotItemChange(QString,QString)));

    thread->start();

}



void Server::clientDisconnected(const QString &ip)
{

    QString text =QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ")+
            ip + " 掉线...";

}



