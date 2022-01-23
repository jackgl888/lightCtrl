#ifndef MYSERIAL_H
#define MYSERIAL_H
#include<QtSerialPort/qserialport.h>
#include<QtSerialPort/qserialportinfo.h>
#include<QTimer>
#include<QMutexLocker>
#include<QTreeWidgetItem>
#include "single/single.h"
//#include "tempreture.h"
#include "struct.h"
//#include "xlsxdocument.h"
//#include  "xlsxformat.h"
//#include  "xlsxcellrange.h"
//#include "xlsxchart.h"

#define   RX_BUFF_LEN    1000

#define   SERIAL_NUM    10

#define  WAIT_HEAD    0
#define  WAIT_LEN  0x01
#define  WAIT_MSG  0x02

#define   MAX_STOREROOM_MODULE   52
#define   MAX_TEMP_MODULE         4

class Myserial : public QSerialPort
{
    Q_OBJECT
public:
    explicit Myserial( QString comName, QObject *parent = Q_NULLPTR);
    ~Myserial();
    
    void Init_serial(void);




signals:
    
    void  serialDiscnct(const QString &ip);

    void  updateBedUi(const uchar cmd,const uchar traynum,const QString  &str);  //更新bed  ui
    void  sig(QVariant);  //线程传数据给UI


private  slots:

    void  recvData(void);
    void  sendData(const QString  &ip, const  int traynum,const uchar cmd,ushort num,ushort value);

    void  partlySendData(const QString  &ip, const  int traynum,const uchar cmd,ushort num,ushort value);

    void  timeoutMethod(void);
    void  stopSerialSlot(const QString &ip);
    void  recvDataMethod(const QString &ip, const uchar *data);  //协议解析

    void  slotSendMMIDataTimeOut(void);//超时重发函数数

    void  slotGetData(QVariant variant,uchar deviceType);

     void pcdParaSend(void);  //参数下发


private:

    QStringList  list;
    QString   serialName;
    uint    boundRate;
    QMutex   mutex,mutex2;   //创建一个互斥量
    uchar  sending_buf[1200];//当前发送包
    uchar serialRecv[500];  //接收数据缓存
    QTimer  *serialTimer; // 温度采样板程序测试定时器
    QTimer  *resendTimer; //监视数据发送

    const  uchar    *resendData;
    uint recv_cnt;		  //当前通讯口第几个字符   三个通讯口串口1 2 3
    uchar head_cnt;
    uint length;
    uchar wait;
    uchar  modbusType;   //modbus协议类型    1 欧姆龙   0  三菱
    uchar  tempSlaves;  //温度采样板从站个数
    bool  startTempAdjust;  //校准开关
    uchar    m_timeout;   //通信计时
    pcdPara  m_pcdPara;

    uchar cmdKind; //命令类型
    uchar resendtimes;  //重发次数
};


#endif // Myserial_H
