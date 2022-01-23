#include "serverthread.h"
#include<QDebug>






serverThread::serverThread(int sockDesc,QString comeName, QObject *parent) :
    QThread(parent),
    m_sockDesc(sockDesc)
{

  serialName = comeName;
}


serverThread::~serverThread()
{
    // qDebug()<<"XGID:"<<this->currentThreadId();
#if  0
    delete   m_socket;

    if(Single::m_Instance()->AjlCnected== 1)
        delete  Ajlensocket;
    else
    {
        m_serial->clear();
        m_serial->close();
        m_serial->deleteLater();
    }
#endif

}


void serverThread::run(void)
{

        m_serial = new Myserial(serialName);
        m_serial->moveToThread(this);

        //串口发送数据
        connect(this, SIGNAL(sendData(QString,int,uchar,ushort,ushort)),
                m_serial, SLOT(sendData(QString,int,uchar,ushort,ushort)),Qt::  QueuedConnection);

        //线程退出时先退出串口
        connect(m_serial,SIGNAL(serialDiscnct(const QString &)),this,SLOT(serialDisconnect(const QString &)),
                Qt::DirectConnection);
        //停止串口
        connect(this,SIGNAL(stopSerial(const QString &)),m_serial,SLOT( stopSerialSlot(const QString & ))
                ,Qt::  QueuedConnection);

        //串口发送数据到界面
        connect(m_serial,SIGNAL(updateBedUi(uchar,uchar,QString)),this,
                SLOT( bedCmdDataSlot(uchar,uchar,QString)),Qt::QueuedConnection);

        //ui传数据到工作线程
        connect(this,SIGNAL(sigto(QVariant, uchar )),m_serial,SLOT(slotGetData(QVariant,uchar)));

        //数据传给ui
        connect(m_serial,SIGNAL(sig(QVariant)),this,SLOT(dataSendUi(QVariant)));




       this->exec();

}


//主线路item状态改变
void serverThread::slotItemChange(QString ip,QString device)
{
    emit sigItemChange(ip,device);
}


void serverThread:: sendDataSlot(const QString  &ip, const  int traynum,const uchar cmd,ushort num,ushort value)
{

    emit  sendData(ip, traynum,cmd,num,value) ;

}


//
void serverThread::slotSendUpdateCmd(uchar cmd ,uchar type)
{
    emit sendUpdateCmd(cmd,type);
}


//串口掉线处理
void serverThread::serialDisconnect(const QString & ip)
{

    Single::m_Instance()->serialPara.clear();  //清除当前升级串口的所有参数
    Single::m_Instance()->portNamelist.removeOne(ip);
    //for(ushort i=0;i<UPDATENUM ;i++)
    // Single::m_Instance()->messageRefresh(ip,i,NULL,0,true,true,true);
    emit  updateUiSerial(ip);
    this->quit();
    this->wait();
}


void serverThread:: bedCmdDataSlot(const uchar cmd,const uchar traynum,const QString &str)
{
    emit  bedUpdate(cmd,traynum,str);
}


void serverThread::dataSendUi(QVariant para)
{
    emit sigtu(para);
}



void serverThread::slotReadData()
{
    emit  readData();
}

void serverThread::startSwitchSlot()
{
    emit  startSwitch();
}


//传送数据到工作线程
void serverThread::slotGetData(QVariant para,uchar device)
{
    emit sigto(para,device);
}



//返回温度
void serverThread::slotTempBack(QVariant para)
{
    emit  sigTempBack(para);
}




void serverThread::disconnectToHost(void)
{
    QString  ip;
    //    for(int i=0;i<ports.length();i++)
    //    {
    //        if(ports.at(i) == m_sockDesc)
    //        {

    //            ip =ipList.at(i);
    //            ipList.removeAt(i);
    //            ports.removeAt(i);
    //            break;
    //        }
    //    }
    //  for(ushort i=0;i<UPDATENUM ;i++)
    //  Single::m_Instance()->messageRefresh(ip,i,NULL,0,true,true,true);

    //Ajlensocket->disconnectFromHost();
    emit disconnectTCP( ip);
    this->quit();
    this->wait();

}









void serverThread::stopSerialSlot(const QString &ip)
{
    emit  stopSerial(ip);
}







