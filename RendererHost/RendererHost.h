#ifndef RENDERERHOST_H
#define RENDERERHOST_H

#include <QObject>
#include <QTcpSocket>
#include <QList>
#include "RendererTask.h"
#include "Renderer_ConfigParse.h"
namespace TopVertex
{
    class RendererHost : public QObject
    {
        Q_OBJECT
    public:
        explicit RendererHost(QObject *parent = 0);
        inline QString taskNameAsString()
        {
            QString temp;
            if(_tasks.size()==0)
            {
                temp.push_back("NULL PROCESS");
                return temp;
            }

            foreach(RendererTask *r,_tasks)
            {
                temp.push_back(r->_taskName);
                temp.push_back(":");
                temp.push_back(QString::number(r->_process->getProcessPID()));
                temp.push_back("|");

            }
            return temp;
        }

    signals:

    public slots:
        void slot_readyRead();
        void slot_disconnected();
        void slot_processResult(QString &data,int pid);
        void slot_processEnded(int pid);
        void slot_connected();
        void slot_reconnect();
    private:
        QTcpSocket *_socket;
        QList <RendererTask *> _tasks;
        Renderer_ConfigParse _configParse;
        bool _isConnect;

    };
}


#endif // RENDERERHOST_H
