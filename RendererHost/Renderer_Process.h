//
// Created by gearslogy on 10/24/16.
//

#ifndef TOPVERTEXYICHAT_YCHAT_PROCESS_H
#define TOPVERTEXYICHAT_YCHAT_PROCESS_H

#include <QObject>
#include <QProcess>
namespace TopVertex
{

    class Renderer_Process :public QObject
    {
    Q_OBJECT
    public:
        Renderer_Process(QObject*parent = 0);
        void clearArguments(){_arguments.clear();}
        void setArguments(QStringList &argument);
        void appendArgument(QString argument);
        void start(QString program);
        int isRunning(){return _isRunning;}
        int getProcessPID(){return _process->pid();}
        QProcess *getProcess(){return _process;}
    signals:
        void signal_pidEnded(int pid);
        void signal_endResultAsString(QString &data,int pid);
    private slots:
        void slot_processEndResult(int exitCode, QProcess::ExitStatus exitStatus);
        void slot_readyRead();
    private:
        QProcess *_process;
        QStringList _arguments;
        QString _buffer;
        int _isRunning;
    };

}



#endif //TOPVERTEXYICHAT_YCHAT_PROCESS_H
