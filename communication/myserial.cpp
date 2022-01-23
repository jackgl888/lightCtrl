#include "myserial.h"
#include <QDateTime>
#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QDebug>
#include <QDebug>
#include <synchapi.h>



Myserial::Myserial( QString comName,QObject *parent):
    QSerialPort(parent)
{
    

    this->serialTimer = new QTimer(this);   //串口管理与定时发送定时器
    this->resendTimer =new QTimer(this);  //超时重发定时器

    connect(serialTimer, SIGNAL(timeout()), this, SLOT(timeoutMethod()));  //温度采样板测试
    connect(resendTimer,SIGNAL(timeout()),this,SLOT( slotSendMMIDataTimeOut()));  //超时重发

    serialName = comName;
    // this->serialTimer->start(100);
 //   tempPoll->start(100);

     //初始化串口
     Init_serial();
     connect(this,&Myserial:: readyRead, this, &Myserial::recvData);//
     resendtimes  = 0;
     cmdKind = 0;
     m_timeout = 0;
     recv_cnt = 0;		  //当前通讯口第几个字符   三个通讯口串口1 2 3
     head_cnt = 0;
     length = 0;
     wait = 0;

}



Myserial::~Myserial()
{
    
}




void Myserial::recvDataMethod(const QString &ip, const uchar *data)
{


      if(*(data+4) == 0) //数据下发正确
      {
            cmdKind++;
         if(cmdKind<6)
         {

             pcdParaSend();
             resendtimes = 0;
         }
         else
             cmdKind =6;
      }
      else
      {
           resendtimes++;
           if(resendtimes<3)
            pcdParaSend();
           else
           {
                   cmdKind++;
               if(cmdKind<6)
                {

                   pcdParaSend();
               }
               else
                   cmdKind = 6;

           }

      }

}
















//数据超时重发函数
void Myserial::slotSendMMIDataTimeOut()
{

    #if 0
    QString text,str;

    this->resendTimer->stop();


    this->resendCnt[this->traynumSinding]++;
    str =  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ");
    if(this->resendCnt[this->traynumSinding]>3)
    {
        this->resendCnt[this->traynumSinding] = 0;
        //报该节点掉线并发送下一个节点采样
        text = QString("%1.%2.%3.%4").arg(str).arg("设备").arg(QString::number(this->traynumSinding)).arg("断通信");
        emit  updateBedUi(this->cmdSending[this->traynumSinding],this->traynumSinding,text);

        this->traynumSinding++;

        if(  this->traynumSinding<this->tempSlaves)
        {
            this->partlySendData(this->portName(), this->traynumSinding,'S',0,0);
        }
        else
        {
            this->traynumSinding = 0;
            this->partlySendData(this->portName(),  this->traynumSinding,'S',0,0);
        }
    }
    else
    {
        this->partlySendData(this->portName(),this->traynumSinding,this->cmdSending[this->traynumSinding],0,0);
    }


    pcbResendCnt++;
    if(pcbResendCnt > 3)
    {
        pcbResendCnt = 0;
        text = QString("%1%2%3").arg("断通信").arg(",").arg("FAIL");
        emit  updateBedUi(pcbResendCmd,0,text);
        //记录异常
    }
    else
    {
         this->partlySendData(this->portName(),0,pcbResendCmd,0,0);
    }
#endif
}






//取出界面传来的数据
void Myserial::slotGetData(QVariant  variant,uchar  deviveType)
{
     cmdKind = 0;
     m_pcdPara  =variant.value<pcdPara>();
     pcdParaSend();

}



void Myserial::pcdParaSend(void)
{
    uchar sendBuf[200]={0};


    switch (cmdKind) {
        case 0:   //pwm
        sendBuf[0]=0x40;
        sendBuf[1] = 4;
        sendBuf[2]=0x01;
        sendBuf[3]=   m_pcdPara.id;
        sendBuf[4]= 0x11;  //pwm
        sendBuf[5]=   m_pcdPara.pwm;
        sendBuf[6] = 0;
        for(uchar i=0;i<6;i++)
            sendBuf[6]+=sendBuf[i];
         this->write(reinterpret_cast<const char*>(sendBuf),7);
            break;

    case 1:     //level
        sendBuf[0]=0x40;
        sendBuf[1] = 4;
        sendBuf[2]=0x01;
        sendBuf[3]=   m_pcdPara.id;
        sendBuf[4]=0x10;  //触发高低电平
        sendBuf[5]=   m_pcdPara.level;
        sendBuf[6] = 0;
        for(uchar i=0;i<6;i++)
            sendBuf[6]+=sendBuf[i];
         this->write(reinterpret_cast<const char*>(sendBuf),7);
        break;
    case 2:     //亮度模式
        sendBuf[0]=0x40;
        sendBuf[1] = 4;
        sendBuf[2]=0x01;
        sendBuf[3]=   m_pcdPara.id;
        sendBuf[4]=0x3A;   //亮度模式
        sendBuf[5]=   m_pcdPara.brightMode;
        sendBuf[6] = 0;
        for(uchar i=0;i<6;i++)
            sendBuf[6]+=sendBuf[i];
         this->write(reinterpret_cast<const char*>(sendBuf),7);
        break;

    case 3:     //触发模式
        sendBuf[0]=0x40;
        sendBuf[1] = 4;
        sendBuf[2]=0x01;
        sendBuf[3]=   m_pcdPara.id;
        if(   m_pcdPara.triggerMode == 4 )  //复位
        {
            sendBuf[4]=0x33;
            sendBuf[5] = 0;
        }
        else
        {
            sendBuf[4]=0x32;
            sendBuf[5] = m_pcdPara.triggerMode;
        }
        sendBuf[6] = 0;
        for(uchar i=0;i<6;i++)
            sendBuf[6]+=sendBuf[i];
         this->write(reinterpret_cast<const char*>(sendBuf),7);
        break;
    case 4:     //4个通道亮度值
        sendBuf[0]=0x40;
        sendBuf[1] = 21-3;
        sendBuf[2]=0x01;
        sendBuf[3]=   m_pcdPara.id;
        for(uchar i =0;i<4;i++)//亮度
        {
            sendBuf[4+3*i+i]=0x1A;  //命令字
            sendBuf[5+3*i+i]=i;  //通道 号
            sendBuf[6+3*i+i]=   m_pcdPara.brightVal[i];
            sendBuf[7+3*i+i]=   m_pcdPara.brightVal[i]>>8;
        }
        sendBuf[20] = 0;
        for(uchar i=0;i<20;i++)
            sendBuf[20]+=sendBuf[i];
         this->write(reinterpret_cast<const char*>(sendBuf),21);
        break;

    case 5:     //4个通道开关控制
        sendBuf[0]=0x40;
        sendBuf[1] = 14-3;
        sendBuf[2]=0x01;
        sendBuf[3]=   m_pcdPara.id;
        for(uchar i =0;i<3;i++)//开关
        {
            sendBuf[4+2*i+i]=0x2A;  //命令字
            sendBuf[5+2*i+i]=i;  //通道 号
            sendBuf[6+2*i+i]= m_pcdPara. pcdCtrl[i];
        }
        sendBuf[13] = 0;
        for(uchar i=0;i<13;i++)
            sendBuf[13]+=sendBuf[i];
         this->write(reinterpret_cast<const char*>(sendBuf),14);
        break;

    case 6:     //设备复位
        sendBuf[0]=0x40;
        sendBuf[1] = 4;
        sendBuf[2]=0x01;
        sendBuf[3]=   m_pcdPara.id;
        sendBuf[4]= 0x23;
        sendBuf[5] = 0;
        sendBuf[6] = 0;
        for(uchar i=0;i<6;i++)
            sendBuf[6]+=sendBuf[i];
         this->write(reinterpret_cast<const char*>(sendBuf),7);
        break;

    default:
        break;
    }



#if 0


    for(uchar i =0;i<3;i++)//开关
    {
        sendBuf[28+2*i+i]=0x2A;  //命令字
        sendBuf[29+2*i+i]=i;  //通道 号
        sendBuf[30+2*i+i]=   m_pcdPara. pcdCtrl[i];
    }
    sendBuf[37] = 0;
    for(uchar i=0;i<37;i++)
        sendBuf[37]+=sendBuf[i];

     this->write(reinterpret_cast<const char*>(sendBuf),38);
    #endif


}






void  Myserial::Init_serial()
{
    //  qDebug()<<"INthread:"<<QThread::currentThreadId();

    //设置串口名
    this->setPortName(serialName);
    //设置波特率
    this->setBaudRate(19200);
    //设置数据位数
    this->setDataBits(QSerialPort::Data8);
    //设置奇偶校验
    this->setParity(QSerialPort::NoParity);
    //设置停止位
    this->setStopBits(QSerialPort::OneStop);
    //设置流控制
    this->setFlowControl(QSerialPort::NoFlowControl);
    this->open(QIODevice::ReadWrite);

    QCoreApplication::processEvents();//避免界面冻结
    
}




//串口接收数据
void Myserial::recvData(void)
{
    // qDebug()<<"INthread:"<<QThread::currentThreadId();
    QByteArray  str1;
    uchar str2[300]={0};

    Sleep(5);
    
    str1 = this->readAll();
    memcpy(str2,str1.data(),str1.length()); //
//    for(uchar i=0;i<str1.length();i++)
//    {
//        if(str1[0]==0x40)
//        {
//            if(str1[4]==0) //
//             {
//                if(cmdKind<4)
//                {
//                    cmdKind++;
//                    pcdParaSend();
//                }

//             }
//            else
//                 pcdParaSend();

//        }
//    }
     #if 1
        for(uchar i=0;i<str1.length();i++)
        {
            if(this->wait == WAIT_HEAD)    //接收
            {

                if(str2[i] == 0x40)
                {
                    if(this->head_cnt == this->recv_cnt)
                    {
                        this->serialRecv[this->recv_cnt]=str2[i];

                            this->wait = WAIT_LEN;
                    }
                    else
                        goto  err_process;
                }
                else
                    goto   err_process;
            }
            else  if(this->wait== WAIT_LEN)
            {
//                if(this->recv_cnt == 1)
//                {
//                    this->serialRecv[this->recv_cnt]=str2[i];
//                    this->length = str2[i];
//                }
                if(this->recv_cnt == 1)
                {
                    this->serialRecv[this->recv_cnt]=str2[i];

                    this->length=str2[i];

                    this->wait = WAIT_MSG;    //

                }
            }
            else if(this->wait == WAIT_MSG)
            {
                this->serialRecv[this->recv_cnt]=str2[i];
                if(this->recv_cnt>= (this->length +3-1))
                {
                    this->wait= WAIT_HEAD;
                    this->recv_cnt =0;
                    this->head_cnt =0;
                    this->length = 0;
                    this->recvDataMethod(this->portName(),this->serialRecv);
                    return ;
                }
            }
            else
                goto   err_process;
            this->recv_cnt++;
            continue;
        }

  return;
err_process:
    wait = WAIT_HEAD;
    recv_cnt = 0;
    head_cnt= 0;
    length = 0;
    return;

#endif
}








void Myserial::sendData(const QString  &ip, const  int traynum,const uchar cmd,ushort num,ushort value)
{
    
    //  qDebug()<<"SDthread:"<<QThread::currentThreadId();
    //  QString  text = "ghtytuitowqe";
    //  QByteArray  data = text.toUtf8();
    //  this->write( data);
    
    QMutexLocker  Locker(&this->mutex);
    QString  peerAddr=  this->portName();
    
    if (ip == peerAddr) {                 //判定发送是否是目的客户端的socket
        cmdKind = 6;
        pcdParaSend();
       // this->partlySendData(ip, traynum, cmd,num,value);
    }

}

























//分包发送数据
void Myserial::partlySendData(const QString  &ip, const  int traynum,const uchar cmd,ushort num,ushort value)
{

    ;

}









//串
void Myserial::timeoutMethod(void)
{

  ;

}




//关闭串口
void Myserial::stopSerialSlot(const QString &ip)
{
    
    if(this->portName() == ip)
    {
        emit serialDiscnct(ip);  //销毁线程及串口对象
        this->close();

    }
    
}


