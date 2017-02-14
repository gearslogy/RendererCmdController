//
// Created by gearslogy on 10/24/16.
//

#include "Renderer_Process.h"
using namespace TopVertex;
#include <QDebug>
#include <string>
#include <iostream>
using namespace std;
Renderer_Process::Renderer_Process(QObject *parent):QObject(parent)
{
    _process = new QProcess(this);
    connect(_process,SIGNAL(finished(int , QProcess::ExitStatus )),this,SLOT(slot_processEndResult(int,QProcess::ExitStatus)));
    connect(_process,SIGNAL(readyRead()),this,SLOT(slot_readyRead()));
    _isRunning = 0;
}

void Renderer_Process::setArguments(QStringList &argument)
{
    _arguments = argument;
}
void Renderer_Process::appendArgument(QString argument)
{
    _arguments.append(argument);
}
void Renderer_Process::start(QString program)
{
    _isRunning = 1;
    if(_arguments.size() == 0)
    {
        _process->start(program);
    }
    else
    {
        _process->start(program,_arguments);
    }


}
void Renderer_Process::slot_processEndResult(int exitCode, QProcess::ExitStatus exitStatus)
{
    _isRunning = 0;

    emit signal_pidEnded(_process->pid());


}
void Renderer_Process::slot_readyRead()
{
    QString data = _process->readAll();
    //qDebug() <<data.toLocal8Bit();
    //qDebug() << qPrintable(data);
    emit signal_endResultAsString(data,_process->pid());

}
