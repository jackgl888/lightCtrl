
#include"Single.h"
#include <QDebug>
QSqlDatabase Single::database;
Single*  Single::m_pInstance  = NULL;
Single::CGarbo  m_Garbo;
QMutex  Single::m_Mutex;
QString Single::dbPath = "./database";
QString Single::dbFile = "databse.db";
QString  Single :: dbName = "default";
QString  Single :: dbUser = "root";
QString  Single :: dbPasswd = "1234";

// CRC 低位字节值表
static uchar s_CRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

// CRC 高位字节值表
static uchar s_CRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
} ;



Single::Single()
{

    //数据库文件目录，不存在则创建
    QDir dir;
    if(!dir.exists(dbPath))//判断路径是否存在
    {
        dir.mkpath(dbPath); //创建路径
        qInfo("mkpath:%s",qPrintable(dbPath));
    }

    if(openDatabase(dbPath + "/" + dbFile))  //make db
    //   this-> initDatabase();
        ;

}





/*
 * 判断是否符合和校验
*/
bool Single::net_packet_checksum(const uchar *pBuff, ushort len)
{
   uchar sum;

    sum = checksum(pBuff, (len - 1));

    if (sum == *(pBuff + len - 1 ))
        return true;
    else
        return false;
}





//crc校验
ushort Single::CRC16_Modbus(const uchar *_pBuf, ushort _usLen)
{

        uint 	i = 0;
        uchar 	CRCH = 0xFF;
        uchar	CRCL = 0xFF;
        ushort 	index = 0x0000;

        while (_usLen-- > 0)
        {
            index = (ushort)(CRCL ^ _pBuf[i++]);
            CRCL = (uchar)(CRCH ^ s_CRCHi[index]);
            CRCH = s_CRCLo[index];
        }

        return (CRCH << 8 | CRCL);

}

uchar Single::checksum(const uchar *pBuff, ushort len)
{
    uchar sum;
    ushort i;
    sum = 0;
    for (i = 0; i < len; i++)
    sum += *pBuff++;

    return sum;
}


//退出数据库连接
Single::quiteSql()
{
    if(database.isOpen()){
        database.close();
        qInfo("Close database success.");
    }
    qInfo("[OK] AppDatabase quit.");
}

//插入数据
bool Single::insertData(const QString sqlStr, const float  *val ,const int row,const int columns)
{


    QSqlQuery sql_query;
//  static  uchar k;
//     for(uchar j = 0;j<24;j++)
//     {
//          if((*(val+j)) == 0)
//          {
//              k++;
//              if(k ==20)
//              {
//                   k = 0;
//                    return 0;
//              }

//          }
//     }
//     k = 0;
    database.transaction();
    sql_query.prepare(sqlStr);

    for(int i = 0; i<=columns;i++)
    {
        if(i == 0)
           sql_query.bindValue(i,QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz: "));
        else
        {

                  sql_query.bindValue(i,*(val+i-1));
        }
    }
    bool success= sql_query.exec();

    if(!success)
    {
        QSqlError lastError=sql_query.lastError();
        qDebug()<<lastError.driverText()<<QString(QObject::tr("插入失败"));
    }
     database.commit();

  #if 0
    qInfo("[SQL] %s",qPrintable(sqlStr));
    if(!sql_query.exec(sqlStr))
    {
        qWarning("[SQL] %s",qPrintable(sql_query.lastError().text()));
        return false;
    }
    database.commit();
#endif
    return true;
}

//更新数据
bool Single::updateData(const QString sqlStr)
{
    QSqlQuery sql_query;
     qInfo("[SQL] %s",qPrintable(sqlStr));
     if(!sql_query.exec(sqlStr))
     {
         qWarning("[SQL] %s",qPrintable(sql_query.lastError().text()));
         return false;
     }
     return true;
}

//查询数据
bool Single::selectData(const QString sqlStr, QList<QMap<QString, QVariant> > &valuesList)
{
    QSqlQuery sql_query;
      qInfo("[SQL] %s",qPrintable(sqlStr));
      if(!sql_query.exec(sqlStr))
      {
          qWarning("[SQL] %s",qPrintable(sql_query.lastError().text()));
          return false;
      }

      for( int r=0; sql_query.next(); r++ )
      {
          QSqlRecord rec = sql_query.record();
          QMap<QString,QVariant> valueMap;
          valueMap.clear();

          for( int c=0; c<rec.count(); c++ )
          {
              valueMap[rec.fieldName(c)] = rec.value(c);
          }

          valuesList.append(valueMap);
      }

      return true;
}

//删除数据
bool Single::deleteData(const QString sqlStr)
{
    QSqlQuery sql_query;
    qInfo("[SQL] %s",qPrintable(sqlStr));
    if(!sql_query.exec(sqlStr))
    {
        qWarning("[SQL] %s",qPrintable(sql_query.lastError().text()));
        return false;
    }
    return true;
}


//打开数据
bool Single::openDatabase(const QString dbFileName)
{
    //建立并打开数据库
      if(QSqlDatabase::contains("qt_sql_default_connection"))
          database = QSqlDatabase::database("qt_sql_default_connection");
      else
          database = QSqlDatabase::addDatabase("QSQLITE");

      database.setDatabaseName(dbFileName);
      database.setUserName(dbUser);
      database.setPassword(dbPasswd);
      if (!database.open())
      {
          qWarning(qPrintable(QString("Connect database %1 error, %2").arg(dbFileName).arg(database.lastError().text())));
          return false;
      }
      else
      {
          qWarning(qPrintable(QString("Connect database %1 succeed.").arg(dbFileName)));
          return true;
      }

}


//初始化数据库
bool Single::initDatabase(const QString sqlStr)
{
    if(database.open())
      {

          QSqlQuery sql_query;
          qInfo("[SQL] %s",qPrintable(sqlStr));
          if(!sql_query.exec(sqlStr))
          {
              qWarning("[SQL] %s",qPrintable(sql_query.lastError().text()));
              return false;
          }
#if 0
          //创建表userInfo         //创建表loginInfo
          qInfo("[SQL] %s",qPrintable(SQL_CREATE_USERINFO));
          if(!sql_query.exec(SQL_CREATE_USERINFO))
          {
              qWarning("[SQL] %s",qPrintable(sql_query.lastError().text()));
              return false;
          }
   #endif
          return true;
      }
      else
      {
          qWarning("Init database failed,database is not open.");
          return false;
    }
}

void Single::insertIpList(QString ipNode)
{
       this->ipList.append(ipNode);
}

QStringList Single::getIpList()
{
    return this->ipList;
}


//get 中位机  treewidget item  节点
QTreeWidgetItem *Single::getm_itemNode(uchar num)
{
    return this->m_itemList.at(num);
}


//插入节点
void Single::insertItemList(QTreeWidgetItem *node)
{
    this->m_itemList.append(node);
}


//中位机节点出列
void Single::removeIpList(ushort num)
{
        this->ipList.removeAt(num);
}


//出item队列
void Single::removeItemList(uchar num)
{
    this->m_itemList.removeAt(num);
}

ushort Single::getIpListCount()
{
       return  ipList.count();
}

ushort Single::getItemListCount()
{
       return  m_itemList.count();
}
