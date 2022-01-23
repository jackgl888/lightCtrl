#ifndef CONNECTDLG_H
#define CONNECTDLG_H
#include<QDialog>
#include <QGroupBox>
#include  <QLabel>
#include  <QComboBox>
#include  <QLineEdit>
#include  <QPushButton>
#include  <QSpinBox>
class connectdlg : public QDialog
{
    
    Q_OBJECT
public:
    connectdlg(QWidget *parent = NULL);
    QString driverName() const;
    QString databaseName() const;
    QString userName() const;
    QString password() const;
    QString hostName() const;
    int port() const;
    void  addSqliteConnection(void);
    void  creatDB(void);
//    QSqlError addConnection(const QString &driver, const QString &dbName, const QString &host,const QString &user,
//                            const QString &passwd, int port = -1);
private slots:
    
     void slotConectTosql(void);   //fp
     void driveChanged(const  QString &);
private:
    QGroupBox   *cnectmsgBox;
    QLabel *  driverLab;
    QComboBox  *driverBox;
    QLabel * sqlName;
    QLineEdit  *  sqlNameEdit;
    //QLabel  * userName;
    QLineEdit  *userNameEdit;
    QLabel  *userCode;
    QLineEdit  *userCodeEdit;
    QLabel  *serverName;
    QLineEdit *serverNameEdit;
    QLabel * sourceName;
    QSpinBox *sourceNameBox;
    QPushButton  *connectBtn;
    QPushButton  *exitBtn;
    QLabel  * msgLable;
    
};

#endif // CONNECTDLG_H
