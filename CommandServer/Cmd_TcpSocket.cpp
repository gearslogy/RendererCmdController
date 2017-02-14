#include "Cmd_TcpSocket.h"
#include <QDebug>
#include <QThread>

namespace TopVertex
{
    Cmd_TcpSocket::Cmd_TcpSocket(qintptr socketID,QObject *parent):
        QTcpSocket(parent),_socketID(socketID)
    {
        this->setSocketDescriptor(socketID);
        connect(this,&Cmd_TcpSocket::readyRead,this,&Cmd_TcpSocket::slot_readyRead);
        connect(this,&Cmd_TcpSocket::disconnected,this,&Cmd_TcpSocket::slot_disconnected);

    }


    void Cmd_TcpSocket::slot_readyRead()
    {
        QByteArray dataArray;
        dataArray.clear();
        dataArray = this->readAll();
        //qDebug() << "Cmd_TcpSocket::slot_readyRead() THREAD ID: " <<QThread::currentThreadId() << "DATAS IS: " << dataArray;
        emit signal_readyRead(_socketID,this->peerAddress().toString(),this->peerPort(),dataArray);

    }

    void Cmd_TcpSocket::slot_disconnected()
    {
        //qDebug() << "Cmd_TcpSocket::slot_disconnected() THREAD ID: " << QThread::currentThreadId();
        emit signal_disconnected(_socketID,this->peerAddress().toString(),this->peerPort());
    }

    void Cmd_TcpSocket::slot_sendData(const QByteArray &data, int socketID)
    {
        //qDebug() << "Cmd_TcpSocket::slot_sendData THREAD ID: "<<QThread::currentThreadId() << " will write Data: " <<data;
        if(socketID == _socketID)
        {
            this->write(data);
        }
    }

}
