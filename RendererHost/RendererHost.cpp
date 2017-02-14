#include "RendererHost.h"
#include <QHostAddress>
#include <QDebug>
#include <QHostInfo>
#include <QTimer>
using namespace TopVertex;
RendererHost::RendererHost(QObject *parent) : QObject(parent)
{
    _socket = new QTcpSocket(this);
    connect(_socket,&QTcpSocket::readyRead,this,&RendererHost::slot_readyRead);
    connect(_socket,&QTcpSocket::disconnected,this,&RendererHost::slot_disconnected);
    connect(_socket,&QTcpSocket::connected,this,&RendererHost::slot_connected); // use it to register our renderer
    _configParse.readFile("./SYS.conf");

    QString host = _configParse.getStringValue("SERVER_HOST");
    int getPort = _configParse.getIntValue("PORT");
    int getReconnectTimer = _configParse.getIntValue("RECONNECT_HOLD_TIME");


    _socket->connectToHost(QHostAddress(host),getPort);
    if(_socket->waitForConnected(getReconnectTimer))
    {
        qDebug() << "connect host success";
        _isConnect = true;
    }
    else
    {
        QTimer::singleShot(getReconnectTimer, this, SLOT(slot_reconnect()));
        qDebug() << "connect to host failed, reconnect after 3 sec ";
    }


}


void RendererHost::slot_readyRead()
{
    QByteArray datas = _socket->readAll();


    QString header = datas;
    if(header.startsWith("sc"))
    {
        // reciever format is :
        // B(renderer) receiver A Format:-> sc::A TcpSocketID::cmd::cmdArguments

        QStringList spData = header.split("::");

        Renderer_Process * process = new Renderer_Process;
        connect(process,&Renderer_Process::signal_endResultAsString,this,&RendererHost::slot_processResult);
        connect(process,&Renderer_Process::signal_pidEnded,this,&RendererHost::slot_processEnded);
        RendererTask *task = new RendererTask;
        QString cmd = spData[2];
        if(spData.size() == 3) // no arguments-> sc::A TcpSocketID::cmd
        {
            task->_taskName = cmd;
            task->_process = process;
            task->_requestTcpSocketID = spData[1];
            process->start(cmd);
            _tasks.push_back(task);
            qDebug() << "Eval command is " << cmd;

        }
        if(spData.size() == 4) // have arguments -> sc::A TcpSocketID::cmd::cmdArguments
        {
            task->_taskName = cmd;
            task->_process = process;
            task->_requestTcpSocketID = spData[1];
            task->_taskArguments.push_back(spData[3]);
            process->start(cmd);
            _tasks.push_back(task);
            qDebug() << "Eval command with arguments is " << cmd << " " << spData[3];
        }
    }
    if(header.startsWith("kc"))
    {
        // kc::lstask::userID
        // kc::killtask::userID::pid
        QStringList spData = header.split("::");
        if(spData.size()<2)return;
        QString scmd = spData[1];
        QString userTcpID = spData[2];
        if(scmd == "lstask")
        {
            QByteArray array;
            array.push_back("rc::");
            array.push_back(userTcpID.toLocal8Bit());
            array.push_back("::");
            array.push_back(taskNameAsString().toLocal8Bit());
            array.push_back("\n");
            _socket->write(array);
        }
        if(scmd == "killtask")
        {
            // kc::killtask::pid
            int pid = spData[2].toInt();
            qDebug() << "Kill the task pid: " << pid;
            foreach(RendererTask *m,_tasks)
            {
                if(m->_process->getProcessPID() == pid)
                {
                    m->_process->getProcess()->kill();
                    delete m;
                    _tasks.removeOne(m);
                }
            }
        }
    }



}
void RendererHost::slot_disconnected() // if this socket is disconnected ,then clear all task
{
    foreach(RendererTask *m,_tasks)
    {
        if(m->_process->getProcess()->waitForFinished(2000))
        {
            m->_process->getProcess()->kill();
            delete m->_process;
            delete m;
        }
    }
    _tasks.clear();

    _isConnect = false;
    qDebug() << "try to connect to server";
    int getReconnectTimer = _configParse.getIntValue("RECONNECT_HOLD_TIME");
    QTimer::singleShot(getReconnectTimer, this, SLOT(slot_reconnect()));


    // reconnect the server...
}
void RendererHost::slot_reconnect()
{
    QString host = _configParse.getStringValue("SERVER_HOST");
    int getPort = _configParse.getIntValue("PORT");
    int getReconnectTimer = _configParse.getIntValue("RECONNECT_HOLD_TIME");
    _socket->connectToHost(QHostAddress(host),getPort);
    if(_socket->waitForConnected(getReconnectTimer))
    {
        qDebug() << "connect host success";
        _isConnect = true;
    }
    else
    {
       qDebug() << "connect to host failed, reconnect after 3 sec ";
        QTimer::singleShot(getReconnectTimer,this, SLOT(slot_reconnect()));
    }
}


void RendererHost::slot_connected()
{
    QByteArray array;
    array.push_back("renderer ");
    QString localHostName = QHostInfo::localHostName();
    array.push_back(localHostName.toLocal8Bit());
    _socket->write(array);
}



void RendererHost::slot_processResult(QString &data, int pid)
{
    //return the process result to the query user
    //rc::userTcpSocketID::result

    foreach(RendererTask *t,_tasks)
    {
        if(t->_process->getProcessPID() == pid)
        {

            QString header = QString("rc::") + t->_requestTcpSocketID+QString("::") + t->_taskName
                    + QString("::")+ data;
            //qDebug() << " will write out " << header;
            QByteArray array = header.toLocal8Bit();
            _socket->write(array);
            return ;
        }
    }

}

void RendererHost::slot_processEnded(int pid)
{
    foreach(RendererTask *t,_tasks)
    {
        if(t->_process->getProcessPID() == pid)
        {
            delete t->_process;
            _tasks.removeOne(t);
        }
    }
    //qDebug() << " RendererHost::slot_processResult have task num is " << _tasks.size();
}
