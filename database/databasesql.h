#ifndef DATABASESQL_H
#define DATABASESQL_H

#include<QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QList>
#include <QMap>
#include <QDir>
#include <QSqlRecord>
#include <QVariant>



// loginInfo
#define SQL_CREATE_LOGININFO QString("create table if not exists loginInfo(user text primary key unique,password text,loginTime date)")
#define SQL_INSERT_LOGININFO QString("insert into loginInfo values(\"%1\",\"%2\",%3)")
#define SQL_UPDATE_LOGININFO QString("update loginInfo set password=\"%1\",loginTime=%2 where user=\"%3\"")
#define SQL_SELECT_LOGININFO_BY_USER QString("select * from loginInfo where user=\"%1\"")
#define SQL_SELECT_LOGININFO_ALL QString("select * from loginInfo")
#define SQL_DELETE_LOGININFO_BY_USER QString("delete from loginInfo where user=\"%1\"")

// userInfo
#define SQL_CREATE_USERINFO QString("create table if not exists userInfo(workerno text primary key unique,workername text,password text,serverlevel text)")
#define SQL_INSERT_USERINFO QString("insert into userInfo values(\"%1\",\"%2\",\"%3\",\"%4\")")
#define SQL_UPDATE_USERINFO QString("update userInfo set workername=\"%1\",password=\"%2\",serverlevel=\"%3\" where workerno=\"%4\"")
#define SQL_SELECT_USERINFO_BY_USER QString("select * from userInfo where workerno=\"%1\"")
#define SQL_SELECT_USERINFO_ALL QString("select * from userInfo")
#define SQL_DELETE_USERINFO_BY_USER QString("delete from userInfo where workerno=\"%1\"")
#define SQL_DELETE_USERINFO_ALL QString("delete from userInfo")




//tempreture
#define SQL_CREATE_TEMPRETURE     QString(  "create table bedtemp3 (id QString primary key, t1 int,\
t2 int,t3 int,t4 int, t5 int,t6 int,t7 int,\
t8 int,t9 int,t10 int, t11 int,t12 int,t13 int,\
t14 int,t15 int,t16 int, t17 int,t18 int,t19 int ,\
t20 int,t21 int,t22 int, t23 int,t24 int,t25 int,\
t26 int,t27 int,t28 int, t29 int,t30 int,t31 int,\
t32 int,t33 int,t34 int, t35 int,t36 int,t37 int,\
t38 int,t39 int,t40 int, t41 int,t42 int,t43 int,\
t44 int,t45 int,t46 int,t47 int,t48 int,t49 int,t50 int,t51 int,t52 int)")


//tempreture
#define SQL_CREATE_TEMPRETURE   QString(  "create table bedtemp3 (id QString primary key, t1 int,\
t2 int,t3 int,t4 int, t5 int,t6 int,t7 int,\
t8 int,t9 int,t10 int, t11 int,t12 int,t13 int,\
t14 int,t15 int,t16 int, t17 int,t18 int,t19 int ,\
t20 int,t21 int,t22 int, t23 int,t24 int,t25 int,\
t26 int,t27 int,t28 int, t29 int,t30 int,t31 int,\
t32 int,t33 int,t34 int, t35 int,t36 int,t37 int,\
t38 int,t39 int,t40 int, t41 int,t42 int,t43 int,\
t44 int,t45 int,t46 int,t47 int,t48 int,t49 int,t50 int,t51 int,t52 int)")



#define SQL_CREATE_TEMP   QString(  " (id QString primary key, t1 int,\
t2 int,t3 int,t4 int, t5 int,t6 int,t7 int,\
t8 int,t9 int,t10 int, t11 int,t12 int,t13 int,\
t14 int,t15 int,t16 int, t17 int,t18 int,t19 int ,\
t20 int,t21 int,t22 int, t23 int,t24 int,t25 int,\
t26 int,t27 int,t28 int, t29 int,t30 int,t31 int,\
t32 int,t33 int,t34 int, t35 int,t36 int,t37 int,\
t38 int,t39 int,t40 int, t41 int,t42 int,t43 int,\
t44 int,t45 int,t46 int,t47 int,t48 int,t49 int,t50 int,t51 int,t52 int)")


#define   SQL_INSERT_TEMP  QString(  "  values(?,?,?,?,?,?,?,?,?,\
                       ?,?,?,?,?,?,?,?,\
                       ?,?,?,?,?,?,?,?,\
                       ?,?,?,?,?,?,?,?,\
                        ?,?,?,?,?,?,?,?,\
                       ?,?,?,?,?,?,?,?,?,?,?,?)")



//48通道 PT1000
#define SQL_CREATE_48CHTEMP QString(  "create table bedtemp3 (id QString primary key, t1 int,\
t2 int,t3 int,t4 int, t5 int,t6 int,t7 int,\
t8 int,t9 int,t10 int, t11 int,t12 int,t13 int,\
t14 int,t15 int,t16 int, t17 int,t18 int,t19 int ,\
t20 int,t21 int,t22 int, t23 int,t24 int,t25 int,\
t26 int,t27 int,t28 int, t29 int,t30 int,t31 int,\
t32 int,t33 int,t34 int, t35 int,t36 int,t37 int,\
t38 int,t39 int,t40 int, t41 int,t42 int,t43 int,\
t44 int,t45 int,t46 int,t47 int,t48 int,t49 int)")



#define   SQL_INSERT_TEMPRETURE   QString(  "insert into bedtemp3 values(?,?,?,?,?,?,?,?,?,\
                       ?,?,?,?,?,?,?,?,\
                       ?,?,?,?,?,?,?,?,\
                       ?,?,?,?,?,?,?,?,\
                        ?,?,?,?,?,?,?,?,\
                       ?,?,?,?,?,?,?,?,?,?,?,?)")


#define   SQL_INSERT_48CHTEMP   QString(  "insert into bedtemp3 values(?,?,?,?,?,?,?,?,?,\
                       ?,?,?,?,?,?,?,?,\
                       ?,?,?,?,?,?,?,?,\
                       ?,?,?,?,?,?,?,?,\
                        ?,?,?,?,?,?,?,?,\
                       ?,?,?,?,?,?,?,?)")



inline  QString  creatTempTable(ushort addr )
{
        QString dbCmd;
          dbCmd = QString("%1%2%3").arg("create table temp").arg(QString::number(addr)).arg(SQL_CREATE_TEMP  );

        return  dbCmd;

}


inline  QString insertTempTable(ushort addr)
{
    QString dbCmd;
      dbCmd = QString("%1%2%3").arg("insert into temp").arg(QString::number(addr)).arg(SQL_INSERT_TEMP);

    return  dbCmd;
}


#endif // DATABASESQL_H
