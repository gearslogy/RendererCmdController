#include "Cmd_TcpServer.h"
#include <QThread>
#include <QDataStream>

#define HOSTPORT 5000


namespace TopVertex
{
    // construct
    Cmd_TcpServer::Cmd_TcpServer(QObject *parent):QTcpServer(parent)
    {
        // Register keyword
        this->buildingKeyword();


        int ret = this->listen(QHostAddress::Any,HOSTPORT);
        if(!ret)
        {
            qDebug() << "Server binding " << HOSTPORT <<" error";
        }
        else
        {
            qDebug() << "================****Server Started Normaly****===============";
        }

    }
    //
    Cmd_TcpServer::~Cmd_TcpServer()
    {

    }
    //
    void Cmd_TcpServer::buildingKeyword() //register our keyworld,server do not accept other commands
    {
        _parse._keyword.push_back("lsr");
        _parse._keyword.push_back("help");
        _parse._keyword.push_back("lstask");
        _parse._keyword.push_back("killtask");
        _parse._keyword.push_back("count");


    }
    bool Cmd_TcpServer::findAuthBySocketID(int socketID)
    {
        foreach(Client_MemberInfo* m ,_clients)
        {
            if(m->_socketID == socketID)
            {
                return m->_auth;
            }
        }
        return false;
    }
    int Cmd_TcpServer::getClientNum()
    {
        int num = 0;
        foreach (Client_MemberInfo *m, _clients) {
            if(m->_auth == false)
            {
                num += 1;
            }

        }
        return num;
    }

    bool Cmd_TcpServer::socketIsExist(int socketID)
    {
        foreach(Client_MemberInfo* m ,_clients)
        {
            if(m->_socketID == socketID)
            {
                return true;
            }
        }
        return false;
    }

    Client_MemberInfo *Cmd_TcpServer::findMemberBySocketID(int socketID)
    {
        foreach (Client_MemberInfo* m, _clients) {
            if(m->_socketID == socketID)
            {
                return m;
            }
        }
        return 0;
    }

    QByteArray Cmd_TcpServer::keywordAsBytes()
    {
        QByteArray bytes;
        foreach(QString m,_parse._keyword)
        {
            QString msg = m + " ";
            bytes.push_back(msg.toLocal8Bit());
        }
        return bytes;
    }
    QByteArray Cmd_TcpServer::clientsAsBytes()
    {

        QByteArray array;
        array.push_back("{\n");
        array.push_back("    hostname: tcp_port: ip: authority:\n");
        // format : HOSTNAME PORT IP RENDERER/CONTROLLER
        foreach(Client_MemberInfo *m,_clients)
        {
            if(m->_auth==true)
            {
                QString msg =  QString("    ") + m->_hostName + QString(", ") +
                        QString::number(m->_socketID) + QString(", ") +
                        m->_ip + QString(", ") + QString("CONTROLLER\n");
                array.push_back(msg.toLocal8Bit());
            }
            else
            {
                QString msg =  QString("    ") + m->_hostName + QString(", ") +
                        QString::number(m->_socketID) + QString(", ") +
                        m->_ip + QString(", ") + QString("RENDERER\n");
                array.push_back(msg.toLocal8Bit());
            }

        }
        array.push_back("}\n");

        return array;
    }

    void Cmd_TcpServer::ERROR_MESSAGE(QString MSG, int socketID)
    {
        QString emsg = QString("ERROR: ") + MSG;
        emit signal_socketSendData(emsg.toLocal8Bit(),socketID);
    }


    // dispatacher connections
    void Cmd_TcpServer::incomingConnection(qintptr socketDescriptor)
    {
        qDebug() << "Cmd_TcpServer::incomingConnection " << socketDescriptor ;
        Cmd_TcpSocket *socket = new Cmd_TcpSocket(socketDescriptor);
        connect(socket,&Cmd_TcpSocket::signal_disconnected,this,&Cmd_TcpServer::slot_socketDisconnected);
        connect(socket,&Cmd_TcpSocket::signal_readyRead,this,&Cmd_TcpServer::slot_socketRead);
        connect(this,&Cmd_TcpServer::signal_socketSendData,socket,&Cmd_TcpSocket::slot_sendData);


        QThread *thread = new QThread(socket);
        socket->moveToThread(thread);
        thread->start();


        Client_MemberInfo *tempMember = new Client_MemberInfo;
        tempMember->_auth = false; // default is false
        tempMember->_ip = socket->getIP();
        tempMember->_socket = socket;
        tempMember->_socketID = socketDescriptor;
        tempMember->_thread = thread;
        tempMember->debugMember();
        _clients.push_back(tempMember);
    }


    // slot define

    //a socket data incoming
    void Cmd_TcpServer::slot_socketRead(int skID, QString ip, quint16 port, const QByteArray &data)
    {

        QByteArray array = data;
        QString dataHeader;
        QTextStream stream(&array,QIODevice::ReadOnly);
        dataHeader = stream.readAll().simplified();

        //qDebug() <<"Orig data is " << array ;
        //qDebug() << "Printable is " << qPrintable(array);
        //qDebug() << "Simplified is " <<dataHeader;
        //qDebug() << "Cmd_TcpServer::slot_socketRead " <<array << "| header is "<< dataHeader;


         // cuid
        if(dataHeader.startsWith("user")) // register controller
        {
            QStringList _userData= dataHeader.split(" ");
            if(_userData.size() >= 2)
            {
                registerUser(_userData[1],skID);
            }
            else
            {
                const QByteArray reply = "ERROR:: Register user error,format is :user HOSTNAME\n";
                emit signal_socketSendData(reply,skID);
            }
        }
        else if(dataHeader.startsWith("renderer")) // register renderer
        {
            QStringList _userData= dataHeader.split(" ");
            if(_userData.size() >= 2)
            {
                registerRenderer(_userData[1],skID);
            }
        }

        else if(dataHeader == "h" || dataHeader=="help")
        {
            QByteArray array;
            array.push_back("{\n");
            array.push_back("    Our system build two part cmd:\n");
            array.push_back("    (1): use the cmd-> kc::help,kc is get info from server\n");
            array.push_back("    (2): use the cmd-> sc::help,sc is sending cmd to renderer\n");
            array.push_back("    (3): code by GearsLogy, liuyangping207@qq.com\n");
            array.push_back("}\n");
            emit signal_socketSendData(array,skID);
        }


        // title send_cmd
        // format is: sc::rendererSokcetID::ControllerSocketID::controlMSG::controlArugments

        else if(dataHeader.startsWith("sc")) // controller send cmd
        {
            if(!findAuthBySocketID(skID))
            {
                QByteArray msg;
                msg.push_back("ERROR:You have no authority to send cmd\n");
                emit signal_socketSendData(msg,skID);
                return ;
            }

            parse_sc_cmd(dataHeader,skID);
        }
        // title return_cmd,that is renderer process result
        // format is: rc[|]controllerSocketID[|]returnMSG[|]
        else if(dataHeader.startsWith("rc")) // renderer send return cmd result
        {
            //qDebug() << "==============================================Go To RC";
            parse_rc_cmd(array,skID);
        }

        // title keyword_cmd
        // format is:kc[|]cmd
        // exp: kc::help kc::ls kc::lstask
        else if(dataHeader.startsWith("kc")) // this is server cmd that has no others
        {
            if(!findAuthBySocketID(skID))
            {
                QByteArray msg;
                msg.push_back("ERROR:You have no authority to send cmd\n");
                emit signal_socketSendData(msg,skID);
                return ;
            }
            parse_kc_cmd(dataHeader,skID);
        }
        else // all of that do not command ....
        {
            QByteArray msg;
            msg.push_back("ERROR:Can't parse cmd: ");
            msg.push_back(data);
            emit signal_socketSendData(msg,skID);
            return ;
        }

    }


    // a socket disconneted
    void Cmd_TcpServer::slot_socketDisconnected(int socketID, QString ip, quint16 port)
    {
        foreach(Client_MemberInfo *m,_clients)
        {
            if(m->_socketID == socketID)
            {
                m->_socket->close();
                m->_thread->quit();
                if(m->_thread->wait(3000))
                {
                    m->_thread->terminate();
                    m->_thread->wait();
                }
                m->_thread->deleteLater();
                delete m->_socket;
                _clients.removeOne(m);

            }
        }

        qDebug() << "Cmd_TcpServer::slot_socketDisconnected ID:" << socketID <<
                    ",Clients num:" << _clients.size();

    }

    void Cmd_TcpServer::registerUser(QString hostName,int socketId)
    {
        foreach (Client_MemberInfo*m, _clients) {
            if(m->_socketID == socketId)
            {
                m->_hostName = hostName;
                m->_auth = true;
                qDebug() << "*Register user " << hostName <<" | IP: " <<m->_ip;

                const QByteArray reply = "Server:: You will as a user\n";
                emit signal_socketSendData(reply,socketId);
            }
        }
    }

    void Cmd_TcpServer::registerRenderer(QString hostName,int socketId)
    {
        foreach (Client_MemberInfo*m, _clients) {
            if(m->_socketID == socketId)
            {
                m->_hostName = hostName;
                qDebug() << "*Register renderer " << hostName << " | IP: " <<m->_ip;
            }
        }
    }

    void Cmd_TcpServer::parse_kc_cmd(QString &keyword_cmd,int socketID)
    {
        // kc::help kc::lsrenderers
        QStringList keys = keyword_cmd.split("::");
        //qDebug() << keys;
        if( keys.size() >= 2 && _parse.hasKey(keys[1]) )
        {

            if (keys[1] ==  QString("help"))
            {
                QByteArray array;
                array.push_back("KEYWOLDS:\n{\n");
                array.push_back("    *kc is the keyword cmd that get info from server*\n");
                array.push_back("    example: kc::lstask::tcpsocketID\n");
                array.push_back("    example: kc::killtask::tcpsocketID::pid\n");
                foreach(QString m,_parse._keyword)
                {
                    array.push_back("    kc::"+m.toLocal8Bit()+"\n");
                }
                array.push_back("}\n");
                emit signal_socketSendData(array,socketID);

            }
            else if (keys[1] == QString("lsr"))
            {

                QByteArray array = clientsAsBytes();
                emit signal_socketSendData(array,socketID);

            }
            //kc::lstask::tcpscoketid
            else if(keys[1] == QString("lstask") && keys.size()==3)
            {
                int taskSocketID = keys[2].toInt();
                QString requestID = QString::number(socketID);
                if(!socketIsExist(taskSocketID))return;
                QByteArray array;
                array.push_back("kc::lstask::");
                array.push_back(requestID.toLocal8Bit());
                emit signal_socketSendData(array,taskSocketID);

            }
            //kc::killtask::tcpsocketid::pid
            else if(keys[1] == QString("killtask") && keys.size()==4)
            {
                int taskSocketID = keys[2].toInt();
                QByteArray array;
                array.push_back("kc::killtask::");
                array.push_back(keys[3].toLocal8Bit());
                emit signal_socketSendData(array,taskSocketID);

            }
            else if(key[1] == QString("count"))
            {
                QByteArray array;
                int num = getClientNum();
                QString snum = QString::number(num);
                array.push_back(snum.toLocal8Bit());
                emit signal_socketSendData(array,socketID);
            }

            else
            {
                ERROR_MESSAGE("Error format maybe is: kc::lstask::rendererID,kc::killtask::rendererID::pid,kc::help,kc::lsr.....\n",socketID);
            }


        }
        else
        {
            ERROR_MESSAGE(keyword_cmd + QString(" not recognized cmd\n"),socketID);
        }
    }

    void Cmd_TcpServer::parse_sc_cmd(QString &send_cmd,int userSocketID)
    {

        // A is controller,B is renderer
        // A send cmd to B Format:->sc::B TcpSocketID::cmd::cmdArguments
        // B receiver A Format:-> sc::A TcpSocketID::cmd::cmdArguments



        QStringList datas = send_cmd.split("::");
        //qDebug() << "datas length is " << datas.size();
        if(datas.size() ==2 && _parse.hasKey(datas[1])) // sc::help
        {
            if (datas[1] ==  QString("help"))
            {
                QByteArray array;
                array.push_back("KEYWOLDS:\n{\n");
                array.push_back("    *sc is sending cmd to the renderer*\n");
                array.push_back("    format is: sc::rendererTcpSokcetID::controlMSG::controlArguments\n");
                array.push_back("}\n");
                emit signal_socketSendData(array,userSocketID);
            }
            else
            {
                ERROR_MESSAGE("error parse cmd\n",userSocketID);
            }
        }
        else if(datas.size() ==4 ) // that have the arguments
        {
             //transfer socket format must : sc::rendererSokcetID::ControllerSocketID::controlMSG::controlArugments
            // * pattern is to all
            if(datas[1]==QString("*"))
            {
                foreach (Client_MemberInfo *m, _clients)
                {
                    if(m->_auth==false) // if it's a renderer
                    {
                        QString userSKID = QString::number(userSocketID);
                        QString fixCmd =QString("sc::")+userSKID+QString("::")+
                                datas[2]+QString("::")+datas[3];
                        emit signal_socketSendData(fixCmd.toLocal8Bit(),m->_socketID);
                    }
                }
            }
            else
            {
                QString userSKID = QString::number(userSocketID);
                QString fixCmd =QString("sc::")+userSKID+QString("::")+
                        datas[2]+QString("::")+datas[3];


                int receiverTcpID = datas[1].toInt();
                if(!socketIsExist(receiverTcpID))
                {
                    ERROR_MESSAGE(datas[1] + QString("tcp socket id do not exist\n"),userSocketID);
                    return;
                }
                emit signal_socketSendData(fixCmd.toLocal8Bit(),receiverTcpID);

            }


        }
        else if(datas.size() == 3 )
        {
            //transfer socket format must : sc::rendererSokcetID::ControllerSocketID::controlMSG
            if(datas[1] == QString("*"))
            {
                foreach (Client_MemberInfo *m, _clients)
                {
                    if(m->_auth==false) // if it's a renderer
                    {
                        QString userSKID = QString::number(userSocketID);
                        QString fixCmd =QString("sc::")+userSKID+QString("::")+
                                datas[2];
                        emit signal_socketSendData(fixCmd.toLocal8Bit(),m->_socketID);
                    }
                }
            }
            else
            {
                QString userSKID = QString::number(userSocketID);
                QString fixCmd =QString("sc::")+userSKID+QString("::")+
                        datas[2];
                int receiverTcpID = datas[1].toInt();
                if(!socketIsExist(receiverTcpID))
                {
                    ERROR_MESSAGE(datas[1] + QString("tcp socket id do not exist\n"),userSocketID);
                    return;
                }
                emit signal_socketSendData(fixCmd.toLocal8Bit(),receiverTcpID);
            }


        }
        else // all error
        {
            QByteArray array;
            array.push_back("ERROR:error parse cmd:");
            array.push_back(send_cmd.toLocal8Bit()+"\n");
            emit signal_socketSendData(array,userSocketID);
            ERROR_MESSAGE(QString("error parse cmd: ") +send_cmd ,userSocketID);
        }

    }

    void Cmd_TcpServer::parse_rc_cmd(QByteArray &rc_result, int rendererSocketID)
    {

        QString rh_rcresult = rc_result;
        if(rh_rcresult.isEmpty())return;
        QStringList sprc = rh_rcresult.split("::");
        if(sprc.size() == 0) // if no data
        {
            return ;
        }
        if(sprc.size() == 4) // rc::userSKID::taskName::data
        {
            int userID = sprc[1].toInt();
            Client_MemberInfo *rendererM = findMemberBySocketID(rendererSocketID);
            if(rendererM!=0)
            {
                QByteArray temp_array = rendererM->_hostName.toLocal8Bit();
                temp_array.push_back("->PROCESS CMD:   [");
                temp_array.push_back(sprc[2].toLocal8Bit());
                temp_array.push_back("]\n");
                temp_array.push_back(sprc[3].toLocal8Bit());
                temp_array.push_back("\n");

                emit signal_socketSendData(temp_array,userID);
            }
        }

        if(sprc.size() == 3) //rc::userSKID::data
        {

            int userID = sprc[1].toInt();
            Client_MemberInfo *rendererM = findMemberBySocketID(rendererSocketID);
            if(rendererM !=0)
            {
                QByteArray temp_array = rendererM->_hostName.toLocal8Bit();
                temp_array.push_back("->PROCESSING PROGRAM:    ");
                temp_array.push_back(sprc[2].toLocal8Bit());
                temp_array.push_back("\n");
                emit signal_socketSendData(temp_array,userID);
            }
        }

    }
}
