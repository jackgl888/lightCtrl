#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H
//#include "server.h"
#include <QThread>

#include <QTimer>
#include <QTreeWidgetItem>
#include<QMutexLocker>
#include "myserial.h"
#include<QDateTime>

class Myserial;


class serverThread : public QThread
{
    Q_OBJECT
public:
    serverThread(int sockDesc,QString comeName,QObject *parent = Q_NULLPTR) ;

    ~serverThread();
  void run(void);


signals:

    void  disconnectTCP(const QString &ip);
    void  sendData(const QString  &ip, const  int traynum,const uchar cmd,ushort channel,ushort value);
    void  stopSerial(const QString &ip);
    void  updateUiSerial(const QString &ip);
    void  bedUpdate( const uchar cmd,const uchar traynum,const QString &str); //
    void  readData();
    void  startSwitch();
    void  sigto(QVariant para,uchar deviceType);  //ui发到线程
    void  sigtu(QVariant para);   //线程数据返回ui
    void  sigTempBack(QVariant para);
    void  sendUpdateCmd(uchar cmd,uchar type);   //转发升级数据
     void sigItemChange(QString ip,QString device);  //主线程item状态改变
private slots:   


    void stopSerialSlot(const QString &ip );
    void disconnectToHost(void);
    void sendDataSlot(const QString  &ip, const  int traynum,const uchar cmd,ushort num,ushort value);
    void serialDisconnect(const QString & ip);
    void bedCmdDataSlot( const uchar cmd,const uchar traynum,const QString &str);  //数据传回界面
    void dataSendUi(QVariant para);
    void slotReadData();
    void startSwitchSlot();
    void slotGetData(QVariant para,uchar deviceType);
    void slotTempBack(QVariant para);  //温度返回
    void slotSendUpdateCmd(uchar cmd ,uchar type); //转发升级命令
     void slotItemChange(QString ip,QString device);  //响应主线程item状态改变

private:


    int m_sockDesc;
    QList <int >ports;
    QList <QString> ipList;
    Myserial  * m_serial;
    QString serialName;

};

#endif // SERVERTHREAD_H
