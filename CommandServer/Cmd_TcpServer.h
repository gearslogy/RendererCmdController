#ifndef CMD_TCPSERVER_H
#define CMD_TCPSERVER_H
#include "Cmd_TcpSocket.h"
#include <QTcpServer>
#include <QList>
#include <QDebug>
#include <QByteArray>

#include "Cmd_MemberInfo.h"
namespace TopVertex
{

    struct Cmd_Parse
    {
        QStringList _keyword;
        inline bool hasKey(QString key)
        {
            foreach(QString k,_keyword)
            {
                if(k == key)
                    return true;
            }
            return false;
        }
    };


    class Cmd_TcpServer : public QTcpServer
    {
        Q_OBJECT
    public:
        explicit Cmd_TcpServer(QObject *parent = 0);
        ~Cmd_TcpServer();

    signals:
        // @QByteArray is data , int is socketID
        void signal_socketSendData(const QByteArray &,int );
    public slots:

        //read the incoming socket msg and parse
        void slot_socketRead(int skID,QString ip,quint16 port,const QByteArray &data);
        // one socket is disconnected....
        void slot_socketDisconnected(int socketID,QString ip,quint16 port);
    protected:
        void incomingConnection(qintptr socketDescriptor);
    private:
        bool findAuthBySocketID(int socketID);
        bool socketIsExist(int socketID);
        Client_MemberInfo *findMemberBySocketID(int socketID);
        QByteArray clientsAsBytes();
        QList <Client_MemberInfo*> _clients;
        void registerUser(QString hostName,int socketId);
        void registerRenderer(QString hostName,int socketId);
        void ERROR_MESSAGE(QString MSG,int socketID);
    private:
        void buildingKeyword();

        // scan the server cmd,kc meaning of k
        void parse_kc_cmd(QString &keyword_cmd,int socketID); // kc::help kc::lsrenderers

        // parse user cmd to the renderer
        void parse_sc_cmd(QString &send_cmd,int userSocketID);

        //parse renderer cmd to the user
        void parse_rc_cmd(QByteArray &rc_result,int rendererSocketID);

        QByteArray keywordAsBytes();
        Cmd_Parse _parse;
    };
}



#endif // CMD_TCPSERVER_H
