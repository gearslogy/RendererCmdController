#include <QCoreApplication>
#include "Cmd_TcpServer.h"
#include <iostream>
#include <QTextStream>
using namespace std;
using namespace TopVertex;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Cmd_TcpServer *server = new Cmd_TcpServer;
    return a.exec();
}

