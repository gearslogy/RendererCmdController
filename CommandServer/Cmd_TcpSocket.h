#ifndef CMD_TCPSOCKET_H
#define CMD_TCPSOCKET_H
#include <QTcpSocket>
#include <QHostAddress>
namespace TopVertex
{
    class Cmd_TcpSocket : public QTcpSocket
    {
        Q_OBJECT
    public:
        Cmd_TcpSocket(qintptr socketID,QObject *parent = 0);
        QString getIP() { return this->peerAddress().toString();}
    private:
        qintptr _socketID;

    signals:
        // @int is socketID
        // @QString is peer ip
        // @quint16 is peer port
        // @QbyteArray is incoming peer data
        void signal_readyRead(int,QString,quint16,const QByteArray&);

        // @int is socketID, @QString is ip,@quint16 is port
        void signal_disconnected(int,QString,quint16);


    public slots:
        void slot_readyRead();
        void slot_disconnected();
        void slot_sendData(const QByteArray &data,int socketID);
    };

}

#endif // CMD_TCPSOCKET_H
