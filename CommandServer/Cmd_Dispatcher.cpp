#include "Cmd_Dispatcher.h"

namespace TopVertex
{



    Cmd_Dispatcher::Cmd_Dispatcher(QObject *parent) :
        QObject(parent)
    {

    }



    void Cmd_Dispatcher::installClients(QList<Client_MemberInfo *> &listMembers)
    {
        _listMembers = listMembers;
    }



}//end of namespace
