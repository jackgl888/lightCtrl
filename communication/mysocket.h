#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QTcpSocket>
#include<QMutexLocker>
#include<QTreeWidgetItem>
#include <QHostAddress>
#include <QObject>
#include <QTimer>
#include"single/single.h"
#include "struct.h"

#define    CONNECTTARGET       'w'
#define    RUNTARGET           'z'
#define    UPDATETARGET        'x'
#define    DATAVERIFY          'y'
#define    RX_BUFF_LEN        2000
#define   MAX_SEND_SIZE     1024
class QTimer;
#define  SOCKET_NUM  10
class MySocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MySocket(int sockDesc, uchar flag, QObject *parent = Q_NULLPTR);
    ~MySocket();



signals:

    void  timerStart(int secs);
    void  startSwitch(void);
    void  serialDiscnct(void);    //
    void  updateBedUi(const uchar cmd,const uchar traynum,const QString &msg);  //更新bed  ui


private slots:
    void recvData(void);
    void sendData(const QString  &ip, const  int traynum,const   uchar  cmd,const uchar *data);
    void startNextSend(const QString &ip,const uchar cmd);
    void partlySendData(const QString  &ip, const  int traynum,const   uchar  cmd,const uchar *data);
    void recvDataMethod(const QString &ip, const uchar *data);
    void slotConnected();
    void slotDisconnected();
    void timeoutMethod(void);
    void slotReadData(void);  //读取电压值
    void tempSendData(const QString  &ip, const  int traynum,const   uchar  cmd,const uchar *data); //温度工装数据发送
    void ohmmeterSendData(const QString  &ip, const  int traynum,const   uchar  cmd,const uchar *data);//读取值值
    void pressFrockSendData(const QString  &ip, const  int traynum,const   uchar  cmd,const uchar *data);//压力工装数据发送

    void  slotGetData(QVariant variant,uchar device );  //获得ui的数据
    void  slotDataSent(uchar cmd,uchar type);
    void  slotSendBootData();      //发送boot数据
    void  slotSentBootJump(void);  //boot联机
    void  sendDataVerifyCmd(void);   //发送数据校验命令
    void  slotSendRunCmd(void);    //发送运行命令
    void  cmdUpgradeAnalysis(const uchar *data);  //解析升级回复
    void  verifyReplyAnalysis(const uchar *data);   //校验命令回复
    void  runAppCmdAnalysis(const uchar *data); //运行命令回复
    void  slotItemChange(QString rootIp,QString device);
    void  sendBootJumpConnect();  //发送boot跳转与boot联机


private:

    int m_sockDesc;
    QMutex   mutex,mutex2;   //鍒涘缓涓€涓簰鏂ラ噺
    uchar  dataSend[1200];//褰撳墠鍙戦€佸寘
    uchar  recvbuf[2000];
    QTimer  * m_timer;
    uchar    sendCnt; //命令互斥时计时使用
    uchar    resendCnt; //串口重发送计数 大于3次，发送失败
    uchar      resendCmd;  //重新发送命令字
    uchar    cmdSending; //
    uchar    traynumSinding; //
    const  uchar    *resendData;
    QTimer  *resendTimer; //监视数据发送
    updatePara  m_para;    //配置参数
    uchar updata[500000];   //保存的数数据
    uchar readBackData[500000];   //读取回来的数据
    ushort itemIndex;
    QList<ushort>checkedItems;
    uint  frameIndex;    //   frameindex
    ocvCfgPara  m_ocvPara;    //ocv配置参数
    
};

#endif // MYSOCKET_H
