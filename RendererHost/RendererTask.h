#ifndef RENDERERTASK
#define RENDERERTASK
#include "Renderer_Process.h"
#include <QStringList>
namespace TopVertex
{
    struct RendererTask
    {
        QString _requestTcpSocketID;
        QString _taskName;
        QStringList _taskArguments;
        Renderer_Process *_process;
    };

}

#endif // RENDERERTASK

