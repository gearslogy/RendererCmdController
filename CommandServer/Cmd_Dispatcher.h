#ifndef CMD_DISPATCHER_H
#define CMD_DISPATCHER_H

#include <QObject>
#include "Cmd_MemberInfo.h"
#include <QList>
namespace TopVertex
{
    class Cmd_Dispatcher : public QObject
    {
        Q_OBJECT
    public:
        explicit Cmd_Dispatcher(QObject *parent = 0);
        void installClients(QList <Client_MemberInfo*> &listMembers);



    public slots:



    private:
        QList <Client_MemberInfo *> _listMembers;
    };

}

#endif // CMD_DISPATCHER_H
