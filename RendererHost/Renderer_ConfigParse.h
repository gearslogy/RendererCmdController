#ifndef RENDERER_CONFIGPARSE_H
#define RENDERER_CONFIGPARSE_H

#include <QString>
#include <QList>
#include <QMap>
#include <QFile>
namespace TopVertex
{


    class Renderer_ConfigParse
    {
    public:
        Renderer_ConfigParse();
        void readFile(QString file);
        QString getStringValue(QString key);
        int getIntValue(QString key);
        bool getBoolValue(QString key);
    private:
        QString getType(QString line);
        QString _fileName;
        QList <QString> _keywords;
        QMap <QString,QString> _strMap;
        QMap <QString,int> _intMap;
        QMap <QString,bool> _boolMap;
        QFile _file;
    };
}


#endif // RENDERER_CONFIGPARSE_H
