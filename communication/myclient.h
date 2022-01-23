#ifndef MYCLIENT_H
#define MYCLIENT_H
#include  <QTcpSocket>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QObject>
#include "myclientthread.h"
class Myclient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit Myclient(int soc, QObject *parent) ;
    ~Myclient();

signals:

public slots:
        void  slotConnected();
        void  slotDisconnected();
        void  dataRecieved();

private:

     int m_socdesc;

};

#endif // MYCLIENT_H
