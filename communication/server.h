#ifndef SERVER_H
#define SERVER_H
#include "tempreture.h"
#include <QTcpServer>
#include <QDebug>
#include "serverthread.h"
#include"temperature/adjusttemp.h"
#include "update/updatemainwindow.h"
#include <QDateTime>
#define  PORT_NUM   500
class updateMainwindow;
class Dialog;
class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = Q_NULLPTR);
    ~Server();

signals:


private:
    void incomingConnection(int sockDesc);


private slots:
    void clientDisconnected(const QString &ip);


private:
    updateMainwindow *m_Window;

    int m_sockDesc;


};

#endif // SERVER_H
