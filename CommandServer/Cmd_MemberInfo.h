#ifndef CMD_MEMBERINFO
#define CMD_MEMBERINFO
#include "Cmd_TcpSocket.h"

namespace TopVertex
{
    struct Client_MemberInfo
    {
        // @_ip is connection ip
        QString _ip;

        // @_hostName is connection name
        QString _hostName;

        //@_socketID is connction socket id

        int _socketID;

        // _auth that is can control the renderer
        bool _auth;

        // _socket that is connection sokcet
        Cmd_TcpSocket *_socket;

        inline void debugMember()
        {
            qDebug() << "{\n  Client_Members::debugMember\n  HostName: " << _hostName
                     << " \n  IP: " << _ip << "\n  SocketID" << _socketID
                     << "\n}";
        }
        QThread *_thread;
    };

}

#endif // CMD_MEMBERINFO

