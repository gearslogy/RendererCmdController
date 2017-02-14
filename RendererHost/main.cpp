#include <QCoreApplication>
#include "RendererHost.h"
#include "Renderer_ConfigParse.h"
#include <QDebug>
using namespace TopVertex;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    RendererHost host;
    return a.exec();
}

