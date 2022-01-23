#include "mysocket.h"
#include <synchapi.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QDebug>


MySocket::MySocket(int sockDesc, uchar flag,QObject *parent) :
    QTcpSocket(parent),
    m_sockDesc(sockDesc)
{

    this->resendTimer = new QTimer(this);
    connect(this, SIGNAL(readyRead()), this, SLOT(recvData()));
    connect(this->resendTimer,SIGNAL(timeout()),this,SLOT(timeoutMethod()));
    if(flag == 0x01)  //客户端
    {
        this->resendTimer  = new QTimer(this);
        connect(this,SIGNAL(connected()),this,SLOT(slotConnected()));
        connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));

    }

    itemIndex = 0;
    frameIndex = 0;
    resendCnt = 0;
}

MySocket::~MySocket()
{

}


void MySocket::sendData(const QString  &ip, const  int traynum,const   uchar  cmd,const uchar *data)
{


    // QMutexLocker  Locker(&mutex);
    QString  peerAddr=  this->peerAddress().toString().remove(0,7);

    if (ip == peerAddr) {                 //判定发送是否是目的客户端的socket

        //emit   timerStart(200);

        this->partlySendData(ip, traynum, cmd,data);

    }
    //   qDebug()<<"SDthread:"<<QThread::currentThreadId();

}



void MySocket::recvData(void)
{

    // QMutexLocker  Locker(&mutex2);
    // qDebug()<<"REthread:"<<QThread::currentThreadId();
    QString ip = this->peerAddress().toString().remove(0, 7);

//    emit startSwitch();

#if 1
    //   QByteArray data = readAll();

    QByteArray  str1;
    QString   str3,str4,str5;
    float  value;
    uchar str2[800];
    typedef enum { WAIT_HEAD, WAIT_LEN,WAIT_MSG } wait_t;
    static  uint recv_cnt;		  //当前通讯口第几个字符   三个通讯口串口1 2 3
    static  uchar head_cnt;
    static  uint length;
    static  wait_t wait;


    uint tmp;
    str1 =   this->readAll();

#if 0

    if(Single::m_Instance()->socketFlag == 0) //安捷伦
    {
        str3 =    str1 ;
        if(str3.contains("READ?"))
        {
            str4=  str3.remove( "READ?");
            str5=  str4.left(8);
            value =  str5.toFloat();
            value/=100;
            //  Single::m_Instance()->AjlValue = QString("%1").arg(value);
            //
            return;
        }
        else
        {
            str4=  str3.left(8);
            value =  str4.toFloat();
            value/=100;
            // Single::m_Instance()->AjlValue= QString("%1").arg(value);
            //  emit startSwitch();
            return;
        }

    }


#endif

    memset(str2,0,800);
    memcpy(str2,str1.data(),str1.length());
    for(ushort i=0;i<str1.length();i++)
    {

        if(wait == WAIT_HEAD)  //接收
        {
            if(str2[i] == 0xEE)
            {
                if(head_cnt == recv_cnt)
                {
                    this->recvbuf[recv_cnt]=str2[i];
                    if(++head_cnt>=4)
                        wait = WAIT_LEN;
                }
                else
                    goto  err_process;
            }
            else
                goto   err_process;
        }
        else  if(wait == WAIT_LEN)
        {
            if(recv_cnt == 4)
            {
                this->recvbuf[recv_cnt]=str2[i];
                length = str2[i];
            }
            if(recv_cnt == 5)
            {
                this->recvbuf[recv_cnt]=str2[i];
                tmp= str2[i];
                tmp<<=8;
                length|=tmp;
                if((length +6)<=RX_BUFF_LEN)
                {
                    wait = WAIT_MSG;
                }
                else {
                    goto err_process;
                }
            }

        }
        else if(wait == WAIT_MSG)
        {
            this->recvbuf[recv_cnt]=str2[i];
            if(recv_cnt>=(length+6-1))
            {
                wait = WAIT_HEAD;
                recv_cnt =0;
                head_cnt =0;
                length = 0;
                //接收一帧数处理
                this->recvDataMethod(ip,this->recvbuf);
                return ;
            }
        }
        else
            goto   err_process;

        recv_cnt++;
        continue;

err_process:
        wait = WAIT_HEAD;
        recv_cnt = 0;
        head_cnt = 0;
        length = 0;
        return;
    }
#endif
}

//分包发送数据
void MySocket:: partlySendData(const QString  &ip, const  int traynum,const   uchar  cmd,const uchar *data)
{
    ushort crc,len,num=0;
    static uchar secindex =16;  //扇区索引起始地址为16*addrsize + 0x0800 0000;
    QString text;
    QString trnum = QString::number(traynum);
    for(uchar j =0;j<4;j++)
        dataSend[j] = 0xFF;

    if(Single::m_Instance()->netMethod == 0x01)     //软件作为客户端时
    {
        if(Single::m_Instance()->socketFlag == 0x01) //安捷伦
        {
            ;
        }
        //        else if()
        //        {
        //             ;
        //        }
    }
    else //软件作为服务器时
    {
        if(Single::m_Instance()->socketFlag == 0) //温度工装
        {
            ;
        }
        //         else  if( )
        //         {
        //               ;
        //         }
    }
    this->cmdSending = cmd; //保存界面传过来的发送命令
    this->traynumSinding=traynum;//保存当前正在发送的托盘 号
    this->resendData = data;
    //数据发送并使能200ms超时发送定时器
    this->resendTimer->start(1000); //

#if 0
    switch (cmd) {

    case 'A':                        //统一联机命令
        for(uchar i =0;i<Single::m_Instance()->Traymsgset.count();i++)
        {
            if(Single::m_Instance()->Traymsgset.at(i)->getTraynum() == traynum)
            {
                Single::m_Instance()->Traymsgset.at(i)->writeSdflag(true);
                len = 7;
                dataSend[4]=len ;   //长度
                dataSend[5]=len>>8;   //长度
                dataSend[6]= 0;    //托盘号
                dataSend[7]=  cmd;   //升级命令字
                dataSend[8]=  0x0A ;    //工装类型
                dataSend[9]= 0x00 ;   //流水号
                dataSend[10]= 0x00 ;   //流水号
                dataSend[11]=0x00;// 加密
                dataSend[12]=0;     //校验码
                for(uchar i=6;i<len+6-1;i++)
                    dataSend[12]+=dataSend[i];
                this->write(reinterpret_cast<const char*>(dataSend),13);
            }
        }

    case 'P':                        //压力清零
        for(uchar i =0;i<Single::m_Instance()->Traymsgset.count();i++)
        {
            if(Single::m_Instance()->Traymsgset.at(i)->getTraynum() == traynum)
            {
                Single::m_Instance()->Traymsgset.at(i)->writeSdflag(true);

                len = 7;
                dataSend[4]=len ;   //长度
                dataSend[5]=len>>8;   //长度
                dataSend[6]= 0;    //托盘号
                dataSend[7]=  cmd;   //升级命令字
                dataSend[8]=  0x0A ;    //工装类型
                dataSend[9]= 0x00 ;   //流水号
                dataSend[10]= 0x00 ;   //流水号
                dataSend[11]=0x00;// 加密
                dataSend[12]=0;     //校验码
                for(uchar i=6;i<len+6-1;i++)
                    dataSend[12]+=dataSend[i];
                this->write(reinterpret_cast<const char*>(dataSend),13);
            }
        }
        break;
    case 'S':                        //压力采样
        for(uchar i =0;i<Single::m_Instance()->Traymsgset.count();i++)
        {
            if(Single::m_Instance()->Traymsgset.at(i)->getTraynum() == traynum)
            {
                Single::m_Instance()->Traymsgset.at(i)->writeSdflag(true);

                len = 5;
                dataSend[4]=len ;   //长度
                dataSend[5]=len>>8;   //长度
                dataSend[6]= 0;    //托盘号
                dataSend[7]=  cmd;   //升级命令字

                dataSend[8]= 0x00 ;   //流水号
                dataSend[9]=0x00;// 加密
                dataSend[10]=0;     //校验码
                for(uchar i=6;i<len+6-1;i++)
                    dataSend[10]+=dataSend[i];
                this->write(reinterpret_cast<const char*>(dataSend),11);
            }
        }
        break;
    default :
        break;
    }


#if 0
    if(cmd == 'w')
    {
        for(uchar i = 0;i<Single::m_Instance()->Traymsgset.count();i++)
        {
            if(Single::m_Instance()->Traymsgset.at(i)->getSdnum() != -2)
            {
                Single::m_Instance()->Traymsgset.at(i)->writeSdflag(true);
                len = 16-6;
                dataSend[4]=len ;   //长度
                dataSend[5]=len>>8;   //长度
                dataSend[6]= traynum ;    //托盘号
                dataSend[7]= cmd;   //升级命令字
                dataSend[8]= Single::m_Instance()->select;      //设备类型  0：表示针床  1：表示温度控制板
                dataSend[9]= Single::m_Instance()->Single::m_Instance()->upgradeNumMax ;      //  0：默认为4个 否则  1~255;
                for(char c = 0;c<4;c++)
                    dataSend[10+c]=0;
                crc = Single::m_Instance()->CRC16_Modbus(&dataSend[6], 14-6);
                dataSend[14]=  crc& 0XFF;
                dataSend[15] = (crc>> 8)& 0XFF;
                this->write(reinterpret_cast<const char*>(dataSend),16);
                return;
            }

        }

    }
    for(uchar i = 0;i<Single::m_Instance()->Traymsgset.count();i++)
    {
        if((Single::m_Instance()->Traymsgset.at(i)->getIp()==ip)&&
                (Single::m_Instance()->Traymsgset.at(i)->getTraynum()==traynum)&&
                (Single::m_Instance()->Traymsgset.at(i)->getStaChoose()==true)&&
                (Single::m_Instance()->Traymsgset.at(i)->getStaHiden() ==false))
        {
            switch (cmd) {
            case 'x':
                Single::m_Instance()->Traymsgset.at(i)->writeSdflag(true);//置位发送标志
                num =  Single::m_Instance()->Traymsgset.at(i)->getSdnum();
                dataSend[6]= traynum ;    //托盘号
                dataSend[7]= cmd;   //升级命令字
                dataSend[8]=0;     //预留位1
                if( Single::m_Instance()->select == 1)  //温度采板flash容量为1024，此为1
                {
                    dataSend[9]= secindex+num;

                    dataSend[10]=0;
                }

                else
                {
                    if(num%2==0)
                        dataSend[9]= secindex+num/2;      //  扇区索引 secindex   secAddr = (uint32_t)FLASH_PAGE_SIZE * secIndex;
                    dataSend[10]=num%2;    //扇区偏移  secOffset   温度采样板恒为 1 针床主控制板 0 ,//扇区地址
                }

                if(( Single::m_Instance()->lastbytes>0)&&(num ==  Single::m_Instance()->totalSdtimes -1))//最后一帧
                {
                    len = 5+  Single::m_Instance()->lastbytes  +2;  //从箱号到最后帧最后一个字节
                    dataSend[4]=len ;   //长度
                    dataSend[5]=len>>8;   //长度
                    memcpy(dataSend+11,(Single::m_Instance()->total_dataSend+(num* MAX_SEND_SIZE )),Single::m_Instance()->lastbytes);
                    crc = Single::m_Instance()->CRC16_Modbus(&dataSend[6], 5+ Single::m_Instance()->lastbytes );
                    dataSend[11+Single::m_Instance()->lastbytes]=  crc& 0XFF;
                    dataSend[11+Single::m_Instance()->lastbytes+1] =  (crc>> 8)& 0XFF;
                    this->write(reinterpret_cast<const char*>(dataSend),len+6);
                    text =  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ")+ip + " 托盘"+ trnum+"正在升级...";
                    Single::m_Instance()->messageRefresh(ip,traynum,  text ,Single::m_Instance()->totalSdtimes,false,false,false);
                    return ;
                }
                else
                {
                    len = 5+ MAX_SEND_SIZE  +2;  //从箱号到最后帧最后一个字节
                    dataSend[4]=len ;   //长度
                    dataSend[5]=len>>8;   //长度
                    memcpy(dataSend+11,(Single::m_Instance()->total_dataSend+(num* MAX_SEND_SIZE )),MAX_SEND_SIZE );
                    crc =Single::m_Instance()->CRC16_Modbus(&dataSend[6], 5+ MAX_SEND_SIZE );
                    dataSend[11+MAX_SEND_SIZE ]= crc& 0XFF;
                    dataSend[11+MAX_SEND_SIZE +1] = (crc>> 8)& 0XFF;
                    this->write(reinterpret_cast<const char*>(dataSend), len+6);
                    text =  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ")+ip + " 托盘"+ trnum+"正在升级...";
                    Single::m_Instance()->messageRefresh(ip, traynum, text ,num +1,false,false,false);
                    return;
                }
                break;
            case 'z':
                for(uchar i = 0;i<Single::m_Instance()->Traymsgset.count();i++)
                {
                    if((Single::m_Instance()->Traymsgset.at(i)->getIp()==ip)&&
                            (Single::m_Instance()->Traymsgset.at(i)->getTraynum()==traynum))
                    {
                        Single::m_Instance()->Traymsgset.at(i)->writeSdflag(true);
                        len = 4;
                        dataSend[4]=len ;   //长度
                        dataSend[5]=len>>8;   //长度
                        dataSend[6]= traynum ;    //托盘号
                        dataSend[7]= cmd;   //升级命令字
                        crc =Single::m_Instance()->CRC16_Modbus(&dataSend[6], 2);
                        dataSend[8]=crc& 0XFF;
                        dataSend[9]=(crc>> 8)& 0XFF;
                        this->write(reinterpret_cast<const char*>(dataSend), 10);
                        return;
                    }
                }
                break;
            default:
                break;
            }
        }
    }
#endif
#endif
}



//接收数据

void MySocket::recvDataMethod(const QString &ip, const uchar * data)
{

    this->resendTimer->stop();
    this->resendCnt =0;

    ushort len;
    QString   text;       //  const  QString str = QString::fromUtf8(data.data(),data.length());
    
    len = *(data + 4) +(*(data + 5)<<8) ;  //数据长度
    //   if(Single::m_Instance()->CRC16_Modbus(data+6, len))
    //      return;

    uchar cmd =     *(data+7);   //str[6];
    uchar traynum = *(data+6);   //str[7]
    uchar runFlag = *(data+9);   //str[8]
    QString  msg,time;
    time =  QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ");
    switch (cmd) {
    case CONNECTTARGET:

        msg = QString("%1%2%3%4").arg(time).arg("温度盒").arg(QString::number(traynum)).arg("已联机");
        updateBedUi(CONNECTTARGET,traynum,msg);
        if(itemIndex<checkedItems.count())
            itemIndex++;
        else  //联机完成,准备下发升级数据
        {
            itemIndex = 0;
            //this->slotDataSent(RUNTARGET,0);
        }
        break;
    case UPDATETARGET :
        cmdUpgradeAnalysis(data);   //解析升级回复
        break;
    case DATAVERIFY :   //验证

        break;
    default:
        break;
    }


}

void MySocket::slotConnected()
{
    qDebug()<<"连接成功";
}

void MySocket::slotDisconnected()
{
    qDebug()<<"连接失败";
}


//数据发送超时处理
void MySocket::timeoutMethod(void)
{
    //  QString ip = this->peerAddress().toString().remove(0, 7);
     if(resendCnt >3)   //超时下一个节点
     {
                itemIndex++;
          this->slotDataSent(resendCmd,0);
     }
     else  //重新发送
     {
         resendCnt++;
         this->slotDataSent(resendCmd,0);
     }
}




//读取电压值
void MySocket::slotReadData()
{
    uchar buf[7] ={0x52, 0x45, 0x41, 0x44, 0x3F, 0x0D, 0x0A };
    this->write(reinterpret_cast<const char*>(buf),7);
}



void MySocket::tempSendData(const QString &ip, const int traynum, const uchar cmd, const uchar *data)
{

}

void MySocket::ohmmeterSendData(const QString &ip, const int traynum, const uchar cmd, const uchar *data)
{

}

void MySocket::pressFrockSendData(const QString &ip, const int traynum, const uchar cmd, const uchar *data)
{

}



void MySocket::slotGetData(QVariant variant,uchar device)
{

    updatePara para = variant.value<updatePara>();
    m_para.lastbytes = para.lastbytes;
    m_para.totalBytes = para.totalBytes;
    m_para.totalFrames = para.totalFrames;
    memcpy(updata,para.updateData,
           sizeof(para.updateData));

}



//数据发送
void MySocket::slotDataSent(uchar cmd, uchar type)
{


    switch (cmd) {

    case CONNECTTARGET :   //boot跳转与boot联机

           sendBootJumpConnect();
        break;
    case   UPDATETARGET   :         //下发升级数据
           slotSendBootData();
        break ;

    case   DATAVERIFY  :     //校验数据
         sendDataVerifyCmd();
        break;

    case  RUNTARGET:      //运行数据

        break;
    default:
        break;
    }
    //启动重发送定时器
    this->resendCmd  = cmd;
    this->resendTimer->start(1000);
}




//发送boot数据
void MySocket::slotSendBootData()
{
    ushort crc,len=0;
    for(uchar j =0;j<4;j++)
        dataSend[j] = 0xFF;
    dataSend[4]=len-6 ;   //长度
    dataSend[5]=(len-6)>>8;   //长度
    dataSend[6]= checkedItems.value(itemIndex);
    dataSend[7]= RUNTARGET;
    dataSend[8]=0;     //
    dataSend[9]=frameIndex;     //

   dataSend[10]=1;  //扇区偏移温度为1，其它为2


    if(frameIndex == (m_para.totalFrames -1) )//最后一帧
    {
        if( m_para.lastbytes >0)  //剩余不满1k字节数的发送
        {
            len = 5+ m_para.lastbytes  +2;  //从箱号到最后帧最后一个字节
            dataSend[4]=len ;   //长度
            dataSend[5]=len>>8;   //长度
            memcpy( dataSend+11,(this->updata+(frameIndex* MAX_SEND_SIZE )),m_para.lastbytes);
            crc = Single::m_Instance()->CRC16_Modbus(&dataSend[6], 4+ m_para.lastbytes );
            dataSend[11+m_para.lastbytes]=  crc& 0XFF;
            dataSend[11+m_para.lastbytes+1] =  (crc>> 8)& 0XFF;
            this->write(reinterpret_cast<char *>(dataSend),len);
            // emit dataToSend(ip,  dataSend,len+6);
        }
        else  //最后一帧刚好1024字节
        {
            len = 5+ MAX_SEND_SIZE  +2;  //从箱号到最后帧最后一个字节
            dataSend[4]=len ;   //长度
            dataSend[5]=len>>8;   //长度
            memcpy( dataSend+11,(this->updata+(frameIndex* MAX_SEND_SIZE )),MAX_SEND_SIZE );
            crc =Single::m_Instance()-> CRC16_Modbus(&dataSend[6], 4+ MAX_SEND_SIZE );
            dataSend[11+MAX_SEND_SIZE ]= crc& 0XFF;
            dataSend[11+MAX_SEND_SIZE +1] = (crc>> 8)& 0XFF;
            this->write(reinterpret_cast<char *>(dataSend),len);
            // emit dataToSend(ip,  dataSend,len+6);

        }
    }
    else if(frameIndex<(m_para.totalFrames -1))
    {
        len = 5+ MAX_SEND_SIZE  +2;  //从箱号到最后帧最后一个字节
        dataSend[4]=len ;   //长度
        dataSend[5]=len>>8;   //长度
        memcpy( dataSend+11,(this->updata+(frameIndex* MAX_SEND_SIZE )),MAX_SEND_SIZE );
        crc =Single::m_Instance()-> CRC16_Modbus(&dataSend[6], 4+ MAX_SEND_SIZE );
        dataSend[11+MAX_SEND_SIZE ]= crc& 0XFF;
        dataSend[11+MAX_SEND_SIZE +1] = (crc>> 8)& 0XFF;
        this->write(reinterpret_cast<char *>(dataSend),len);
        // emit dataToSend(ip,  dataSend,len+6);
    }
    else
    {
        frameIndex = 0;
        return;
    }

    //this->resendTimer->start(RESENDDELAY);  //数据重新发送定时器
}


//
void MySocket::slotSentBootJump()
{
    uint crc,len;
    len = 12;

    len = 15-6;
    dataSend[4]=len ;   //长度
    dataSend[5]=len>>8;   //长度
    dataSend[6]=checkedItems.value(itemIndex);    //托盘号
    dataSend[7]= 'w';   //升级命令字
    dataSend[8]= 0;      //设备类型  0：表示针床  1：表示温度控制板2：工装主控板
    dataSend[9]=4 ;      //  0：默认为4个 否则  1~255;
    for(char c = 0;c<4;c++)
        dataSend[10+c]=0;
    dataSend[14] = 0;
    for(uchar i=6;i<len+6-1;i++)
        dataSend[14]+=dataSend[i];

    this->write(reinterpret_cast<const char*>(dataSend),15);
    Sleep(30);

    len = 16-6;
    dataSend[4]=len ;   //长度
    dataSend[5]=len>>8;   //长度
    dataSend[6]= checkedItems.value(itemIndex);
    dataSend[7]= 'w';   //升级命令字
    dataSend[8]= 0;      //设备类型  0：表示针床  1：表示温度控制板2：工装主控板
    dataSend[9]= 4 ;      //  0：默认为4个 否则  1~255;
    for(char c = 0;c<4;c++)
        dataSend[10+c]=0;
    crc = Single::m_Instance()->CRC16_Modbus(&dataSend[6], 14-6);
    dataSend[14]=  crc& 0XFF;
    dataSend[15] = (crc>> 8)& 0XFF;
    this->write(reinterpret_cast<const char*>(dataSend),16);


}


//下发数据校验命令
void MySocket::sendDataVerifyCmd()
{
    uint crc,len;

    len = 16-6;
    dataSend[4]=len ;   //长度
    dataSend[5]=len>>8;   //长度
    dataSend[6]= checkedItems.value(itemIndex);
    dataSend[7]= 'y';   //校验命令字
    dataSend[8]= 0;      //default
    dataSend[9]= frameIndex ;      // 扇区索引
    for(char c = 0;c<4;c++)
        dataSend[10+c]=0;
    crc = Single::m_Instance()->CRC16_Modbus(&dataSend[6], 14-6);
    dataSend[14]=  crc& 0XFF;
    dataSend[15] = (crc>> 8)& 0XFF;
    this->write(reinterpret_cast<const char*>(dataSend),16);

}



//发送运行命令
void MySocket::slotSendRunCmd()
{
    uint crc,len;

    len = 16-6;
    dataSend[4]=len ;   //长度
    dataSend[5]=len>>8;   //长度
    dataSend[6]= checkedItems.value(itemIndex);
    dataSend[7]= 'z';   //校验命令
    dataSend[8]= frameIndex ;      // 扇区索引
    for(char c = 0;c<4;c++)
        dataSend[9+c]=0;
    crc = Single::m_Instance()->CRC16_Modbus(&dataSend[6], 13-6);
    dataSend[13]=  crc& 0XFF;
    dataSend[14] = (crc>> 8)& 0XFF;
    this->write(reinterpret_cast<const char*>(dataSend),15);
}


//升级回复解析
void MySocket::cmdUpgradeAnalysis(const uchar *data)
{
    uchar result,addr;
    QString  time,msg;
    time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss: ");
    result = *(data + 7);
    addr = *(data + 4);

    if(result == 'O')  //升级成功
    {
        this->frameIndex++;
        if(this->frameIndex>=m_para.totalFrames) //数据发送完成，开始校验
        {
            msg=QString("%1%2%3%4").arg(time).arg("温度板").arg(QString::number(addr)).arg("数据下发完成！");
            emit updateBedUi('x',addr,msg);
            frameIndex = 0 ;
        //
            if(itemIndex<checkedItems.count())
                itemIndex++;
            else  //联机完成,准备下发升级数据
            {
                itemIndex = 0;
                //各节点数据下发完成 准备开始校验
                  this->slotDataSent(DATAVERIFY,0);
            }
             slotSendBootData();//发送boot数据
        }
        else
        {
            msg =QString("%1%2%3%4").arg(time).arg("温度板").arg(QString::number(addr)).arg("正在下发app数据！") ;
            slotSendBootData();   //继续下发下一包
            emit updateBedUi('x',addr,msg);    //
            //emit sigRunMsgToUi(targetIp,updateCmd:: APPUPDATE,list,  this->frameIndex);
        }

    }
    else  //下发失败
    {
        //   msg="升级失败！";
        //list<<msg;
        //   emit sigRunMsgToUi(targetIp,updateCmd:: APPUPDATE,list, this->frameIndex);
        itemIndex++;
        this->frameIndex = 0;//
         slotSendBootData();//发送boot数据
    }
}



//校验命令回复
void MySocket::verifyReplyAnalysis(const uchar *data)
{

    uchar index = *(data + 9);
     if(frameIndex!= index)  //回复的帧索引与下发的帧索引对比
     {
           return ;
     }
     else
     {
         if(index ==m_para.totalFrames )  //数据接收完成
          {
             memcpy(readBackData+MAX_SEND_SIZE*index,data+10,MAX_SEND_SIZE);
             for(quint64 i = 0; i <m_para.totalBytes;i++)
             {
                 if(readBackData[i]!=updata[i])//校验不通过
                  {
                       return;
                 }
             }
              //校验成功

             if(itemIndex<checkedItems.count())
                 itemIndex++;
             else  //校验完成
             {
                 itemIndex = 0;
                  //准备启动
             }
          }
         else   //继续读取
          {
              frameIndex++;
              memcpy(readBackData+MAX_SEND_SIZE*index,data+10,MAX_SEND_SIZE);
              sendDataVerifyCmd();//下发读回命令
         }
     }

}




//运行命令回复处理
void MySocket::runAppCmdAnalysis(const uchar *data)
{
        uchar result;
        result = *(data + 7);

        if(itemIndex<checkedItems.count())
            itemIndex++;
        else  //运行完成
        {
            itemIndex = 0;
             //
        }
        this->slotDataSent('z',0);
        if(result== 'O')   //运行成功
         ;
        else//运行失败
        ;
}





//查找选中的节点

void MySocket::slotItemChange(QString rootIp, QString device)
{
    uchar childCount ;
    ushort addr;

    if(rootIp ==this->peerAddress().toString().remove(0,7) )
    {
        ushort count=   Single::m_Instance()->getItemListCount();

        checkedItems.clear(); //清除
        for(ushort i=0;i<count;i++)
        {
            if(rootIp== Single::m_Instance()->getm_itemNode(i)->text(0))
            {
                childCount = Single::m_Instance()->getm_itemNode(i)->childCount();
                for(uchar j =0;j<childCount;j++)
                {
                    if(Single::m_Instance()->getm_itemNode(i)->child(j)->checkState(0)==Qt::Checked)
                    {
                        
                        addr= Single::m_Instance()->getm_itemNode(i)->child(j)->text(0).toUShort();
                        checkedItems.append(addr);   //升级节点队列
                        
                    }
                }
            }
        }
    }
    else
        return;

}



//boot跳转与boot联机
void MySocket::sendBootJumpConnect()
{

    ushort   len ,crc;
    for(uchar j =0;j<4;j++)
       dataSend[j] = 0xFF;
    len = 15-6;
    dataSend[4]=len ;   //长度
    dataSend[5]=len>>8;   //长度
    dataSend[6]=checkedItems.value(itemIndex) ;    //托盘号
    dataSend[7]='w';   //升级命令字
    dataSend[8]= 0;      //设备类型  0：表示针床  1：表示温度控制板2：工装主控板
    dataSend[9]=4 ;      //  0：默认为4个 否则  1~255;
    for(char c = 0;c<4;c++)
        dataSend[10+c]=0;
    dataSend[14] = 0;
    for(uchar i=6;i<len+6-1;i++)
        dataSend[14]+=dataSend[i];
    this->write(reinterpret_cast<const char*>(dataSend),15);
    Sleep(30);

    //to  tempbord boot
    len = 16-6;
    dataSend[4]=len ;   //长度
    dataSend[5]=len>>8;   //长度
    dataSend[6]= checkedItems.value(itemIndex);
    dataSend[7]='w';   //升级命令字
    dataSend[8]= 0;      //设备类型  0：表示针床  1：表示温度控制板2：工装主控板
    dataSend[9]= 4 ;      //  0：默认为4个 否则  1~255;
    for(char c = 0;c<4;c++)
        dataSend[10+c]=0;
    crc = Single::m_Instance()->CRC16_Modbus(&dataSend[6], 14-6);
    dataSend[14]=  crc& 0XFF;
    dataSend[15] = (crc>> 8)& 0XFF;
    this->write(reinterpret_cast<const char*>(dataSend),16);

}







//发送下一个节点数据
void MySocket::startNextSend(const QString &ip, const uchar cmd)
{
#if 0
    for(uchar i = 0;i<Single::m_Instance()->Traymsgset.count();i++)
    {
        if(Single::m_Instance()->Traymsgset.at(i)->getIp()==ip)
        {

            if((Single::m_Instance()->Traymsgset.at(i)->getStaChoose()==true)&&
                    Single::m_Instance()->Traymsgset.at(i)->getStaHiden() ==false)
            {

                if(cmd =='x')
                {
                    if(Single::m_Instance()->Traymsgset.at(i)->getSdnum()!=-1)
                    {
                        Single::m_Instance()->Traymsgset.at(i)->writeSdnum(0);
                        Single::m_Instance()->messageRefresh(ip,Single::m_Instance()->Traymsgset.at(i)->getTraynum(),NULL,0,false,false,false);
                        this->sendData(ip,Single::m_Instance()->Traymsgset.at(i)->getTraynum(),cmd); //分包发送
                        return;
                    }

                }
                else if(cmd =='z')
                {
                    if(Single::m_Instance()->Traymsgset.at(i)->getSdnum()!=-2)
                    {
                        this->sendData(ip, Single::m_Instance()->Traymsgset.at(i)->getTraynum(),cmd);
                        return;
                    }

                }
                else
                    ;
            }
            if((cmd == 'w')&&(Single::m_Instance()->Traymsgset.at(i)->getSdnum()!=-2))
            {
                this->sendData(ip, Single::m_Instance()->Traymsgset.at(i)->getTraynum(),cmd);
                return;
            }
        }
    }
#endif

}


