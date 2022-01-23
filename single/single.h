#ifndef Single_H
#define Single_H
#include<QMutexLocker>
//#include <QSharedPointer>
#include "database/databasesql.h"
#include <QDateTime>
#include<QObject>
#include <QTreeWidgetItem>
#define       ONELINELEN        40
#define   MAX_SEND_SIZE   1024
#define  WRITEN_SIZE  200
#define  NEWPT100SAMPLE     0              //0地址 :温度采样板(新板)
#define  NEEDLEBED          1              // 针床
#define  OCVACTTION         2              //OCV动作
#define  OCVSWITCH          3               //OCV切换




class Single: public QObject
{
    Q_OBJECT
public:
    static   Single * m_Instance()
    {
        if(m_pInstance == NULL)
        {
            QMutexLocker  mlocker(&m_Mutex);  //
            if(m_pInstance == NULL)
            {
                m_pInstance = new  Single();
            }
        }
        return  m_pInstance;
    }
    QStringList   portNamelist; //当前所有接入串口名
    QStringList  serialPara; //当前升级所用串口的所有参数
    uchar  netMethod;  //软件在使用时作客户端还是作服务器 0 为服务器  1：客户端
    uchar socketFlag;//网口分配集合

    /*
     * 0： 安捷伦
     * 1:地址： 宝能温度工装
     * 2:地址： 宝能压力工装
     */
    QString  comBuf[10];  //使用串口分配集合
    /* 0地址 :温度采样板(新板)
     * 1地址: 针床
     * 2地址：OCV动作
     * 3地址：OCV切换
     * 4地址：内阻表
     * 5地址：拔嘴工装
     * 6地址：温控
     * 7地址：压力
     * 8地址 ： 三菱modbus
                      */
    uchar  sendReceiveBs;  //数据收发显示控制
    /* 0地址 :欧姆龙Modbus
     * 1地址: 针床
     * 2地址：OCV动作
     * 3地址：OCV切换
     * 4地址：内阻表
     * 5地址：拔嘴工装
     * 6地址：温控
     * 7地址：压力
     * 8地址 ： 三菱modbus
                      */
    bool net_packet_checksum(const uchar  *pBuff, ushort len);//和校验
    ushort CRC16_Modbus(const uchar *_pBuf, ushort _usLen);//CRC校验
    uchar checksum(const uchar *pBuff, ushort len);


    //数据库操作部分函数

    static   quiteSql();
    bool insertData(const QString sqlStr, const float *val ,const int row, const int columns);   //插入数据
    bool updateData(const QString sqlStr);    //更新数据
    bool selectData(const QString sqlStr, QList<QMap<QString,QVariant>> &valuesList);  //查询数据
    bool deleteData(const QString sqlStr);  //删除 数据
    bool openDatabase(const QString dbFileName);  //打开数据库
    bool initDatabase(const QString sqlStr);   //初始化数据库

    //get  ip队列
    QStringList getIpList(void);
    //get  item节点
    QTreeWidgetItem *getm_itemNode(uchar num); //获取
    //ip 地址入队列
    void insertIpList(QString ipNode);
    //入item队列
    void insertItemList(  QTreeWidgetItem *node);
    //ip 地址出列
    void removeIpList(ushort num);
    //出item节点
    void removeItemList(   uchar num);
    //get ip num
    ushort getIpListCount();
    ushort getItemListCount();

private:
    Single();   //禁止构造
    //    Single(const Single&){}    //禁止复制构造
    //   Single& operator ==(const   Single&){}    //禁止运算符重载

    static Single* m_pInstance;  //类的指针
    static QMutex  m_Mutex;
    static QSqlDatabase database;  //全局数据库
    static QString dbPath;    //
    static QString dbFile;
    static QString dbName;
    static QString dbUser;
   // static QString dbPasswd;
  //  QList<QTreeWidgetItem *>m_itemList;  //中位机树节点集合
    QStringList ipList ;  //ip地址队列

public:
    class CGarbo    //析构m_pInstance 指针的类
    {
    public:
        ~CGarbo() {
            if(m_pInstance!=NULL)
            {
                delete  m_pInstance;
                m_pInstance = NULL;
            }
        }
    };
    static CGarbo  m_Garbo;
};


#endif
