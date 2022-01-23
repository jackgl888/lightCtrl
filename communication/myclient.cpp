#include "myclient.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QObject>
#include <QtCore/QThread>


Myclient::Myclient(int soc, QObject *parent):  QTcpSocket(parent),
    m_socdesc(soc)

{


 //   this->connectToHost("192");
    connect(this,SIGNAL(connected()),this,SLOT(slotConnected()));
    connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
    connect(this,SIGNAL(readyRead()),this,SLOT( dataRecieved()));
    
}

Myclient::~Myclient()
{

}


void Myclient::slotConnected()
{

}

void Myclient::slotDisconnected()
{
    
}

void Myclient::dataRecieved()
{
    
}
