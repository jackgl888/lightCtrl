#include "connectdlg.h"

#include<QHBoxLayout>
#include <QVBoxLayout>

connectdlg::connectdlg(QWidget *parent):QDialog(parent)
{
    
    this->cnectmsgBox = new QGroupBox(this);
    this->cnectmsgBox->setWindowIconText("数据库连接设置");
    this->driverLab = new QLabel(tr("驱动:"),this);
    this->driverBox  = new  QComboBox(this);
    QStringList   list = QSqlDatabase::drivers();
#if 0
    this->driverBox.addItems(list);
    this->sqlName  = new QLabel(tr("数据库名:"),this);
    this->sqlNameEdit  = new QLineEdit(this);
    this->userName = new QLabel(tr("用户名:"),this);
    this->userNameEdit   = new QLineEdit(this);
    this->userCode  = new QLabel(tr("密码:"),this);
    this->userCodeEdit  = new QLineEdit(this);
    this->serverName  = new QLabel(this);
    this->serverNameEdit = new QLineEdit(this);
    this->sourceName = new QLabel(tr("端口:"),this);
    this->sourceNameBox  = new QSpinBox(this);
    this->msgLable = new  QLabel(tr(""),this);
    
    this->sourceNameBox->setValue(0);
    QHBoxLayout  *driverlayout = new QHBoxLayout();
    driverlayout->addWidget(this->driverLab);
    driverlayout->addWidget(this->driverBox);
    QHBoxLayout  *sqlNamelayout = new QHBoxLayout();
    sqlNamelayout->addWidget(this->sqlName);
    sqlNamelayout->addWidget(this->sqlNameEdit);
    QHBoxLayout  *userNamelayout = new QHBoxLayout();
    userNamelayout->addWidget(this->userName);
    userNamelayout->addWidget(this->userNameEdit);
    QHBoxLayout    *codelayout   =new QHBoxLayout();
    codelayout->addWidget(this->userCode);
    codelayout->addWidget(this->userCodeEdit);
    QHBoxLayout   *soucerlayout = new QHBoxLayout();
    soucerlayout->addWidget(this->sourceName);
    soucerlayout->addWidget(this->sourceNameBox);
    
    QVBoxLayout  * m_layout  = new QVBoxLayout();
    m_layout->addLayout(driverlayout);
    m_layout->addLayout(sqlNamelayout);
    m_layout->addLayout(userNamelayout);
    m_layout->addLayout(codelayout);
    m_layout->addLayout(soucerlayout);
    this->cnectmsgBox->setLayout(m_layout);
    
    QHBoxLayout  *btnLayout  = new QHBoxLayout();
    btnLayout->addWidget(this->connectBtn);
    btnLayout->addWidget(this->exitBtn);
    
    QVBoxLayout *mainlayout  =new QVBoxLayout(this);
    mainlayout->addWidget(this->cnectmsgBox);
    mainlayout->addLayout(btnLayout);
    
    connect(this->driverBox,SIGNAL(currentIndexChanged(const  QString &)),this,SLOT(driveChanged(const QString &));
            connect(this->connectBtn,SIGNAL(clicked()),this,SLOT(slotConectTosql());
            
            
   #endif
}


QString connectdlg::driverName() const
{
    return this->driverBox->currentText();
}

QString connectdlg::databaseName() const
{
    return  this->sqlNameEdit->text();
}

QString connectdlg::userName() const
{
    return  userNameEdit->text();
}

QString connectdlg::password() const
{
    return  userCodeEdit->text();
}

QString connectdlg::hostName() const
{
    return  serverNameEdit->text();
}

int connectdlg::port() const
{
    return  sourceNameBox->value();
}


void connectdlg::addSqliteConnection()
{
    QSqlDatabase  db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("databasefile");
    
    if (!db.open())
    {
        //this->msgLable->setText(db.lastError().text());
        return;
    }
    this->msgLable->setText(tr("创建sqlite数据库成功!"));
}

void connectdlg::creatDB()
{
    #if 0
    QSqlQuery query;                                    //(a)
    query.exec("create table factory (id int primary key, "
               "temp int,"
               " temp int,"
               "temp int,"
               "temp int,"
               " temp int,"
               "temp int,"
               "temp int )");	
    
    //(b)
    query.exec(QObject::tr("insert into factory values(1, 111, 111,111, 111,111, 111"));
    query.exec(QObject::tr("insert into factory values(2, 211, 211,211, 211,211, 211"));
    query.exec(QObject::tr("insert into factory values(3, 311, 311,311, 311,311, 311"));
  

    query.exec("create table cars (carid int primary key, name varchar(50), factoryid int, year int, foreign key(factoryid) references factory(id))");	//(c)
    query.exec(QObject::tr("insert into cars values(1,'奥迪A6',1,2005)"));
    query.exec(QObject::tr("insert into cars values(2, '捷达', 1, 1993)"));
    query.exec(QObject::tr("insert into cars values(3, '宝来', 1, 2000)"));
    query.exec(QObject::tr("insert into cars values(4, '毕加索',2, 1999)"));
    query.exec(QObject::tr("insert into cars values(5, '富康', 2, 2004)"));
    query.exec(QObject::tr("insert into cars values(6, '标致307',2, 2001)"));
    query.exec(QObject::tr("insert into cars values(7, '桑塔纳',3, 1995)"));
    query.exec(QObject::tr("insert into cars values(8, '帕萨特',3, 2000)"));
#endif 
}

//QSqlError connectdlg::addConnection(const QString &driver, const QString &dbName, const QString &host, const QString &user, const QString &passwd, int port)
//{
//    QSqlError err;
//    QSqlDatabase db = QSqlDatabase::addDatabase(driver);
//    db.setDatabaseName(dbName);
//    db.setHostName(host);
//    db.setPort(port);
//    if (!db.open(user, passwd))                         //(a)
//    {
//        err = db.lastError();
//    }
//    return err;
//}


void connectdlg::slotConectTosql()
{
    if(this->driverBox->currentText().isEmpty())
    {
        this->msgLable ->setText("please choose driver!");
        this->driverBox->setFocus();
    }
    else if(this->driverBox->currentText()=="QSQLITE")
    {
        addSqliteConnection();
        //创建数据库表，已存在则无须执行
        this->creatDB();
        accept();
    }
    else
    {
//        QSqlError err = addConnection(driverName(), databaseName(), hostName(),userName(), password(), port());				//(d)
//        if (err.type() != QSqlError::NoError)           //(e)
//            this->msgLable->setText(err.text());
//        else											//(f)
//            this->msgLable->setText(tr("连接数据库成功!"));
        //创建数据库表，如已存在则无须执行
        accept();
    }
}


void connectdlg::driveChanged(const QString & text)
{
    if(text == "QSQLITE" )
    {
        this->cnectmsgBox->setEnabled(false);
        this->sqlNameEdit->setEnabled(false);
        this->userNameEdit->setEnabled(false);
        this->userCodeEdit->setEnabled(false);
        this->sourceNameBox->setEnabled(false);
    }
    else
    {
        this->cnectmsgBox->setEnabled(true);
        this->sqlNameEdit->setEnabled(true);
        this->userNameEdit->setEnabled(true);
        this->userCodeEdit->setEnabled(true);
        this->sourceNameBox->setEnabled(true);
    }
    
}
