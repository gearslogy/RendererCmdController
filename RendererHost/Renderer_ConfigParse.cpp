#include "Renderer_ConfigParse.h"
#include <QTextStream>

#include <QDebug>
#include <limits>
using namespace TopVertex;
Renderer_ConfigParse::Renderer_ConfigParse()
{
    _keywords.push_back("STRING");
    _keywords.push_back("INT");
    _keywords.push_back("BOOL");
    _keywords.push_back("FLOAT");
}

void Renderer_ConfigParse::readFile(QString file)
{
    _strMap.clear();
    _intMap.clear();
    _boolMap.clear();
    _fileName = file;
    _file.setFileName(_fileName);
    if(!_file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Read Config file error";
        return ;
    }
    // read by line
    while(!_file.atEnd())
    {
        QString line = _file.readLine().simplified();
        QStringList line_sp = line.split("::");
        QString rtype = getType(line);
        if(line_sp.size()>=2)
        {
            QString defines = line_sp[1];

            QStringList defines_sp = defines.split("=");
            QString var_name = defines_sp[0];
            QString var_value = defines_sp[1];

            //qDebug() << var_value;
            if(rtype.isEmpty())
            {
                continue;
            }
            if(rtype == "STRING")
            {
                _strMap.insert(var_name,var_value);
            }
            if(rtype == "INT")
            {
                int rhvar = var_value.toInt();
                _intMap.insert(var_name,rhvar);
            }
            if(rtype == "BOOL")
            {
                bool rhvar = var_value.toInt();
                _boolMap.insert(var_name,rhvar);
            }
        }
    }
    _file.close();

}

QString Renderer_ConfigParse::getType(QString line)
{
    foreach (QString k, _keywords)
    {
        if(line.startsWith(k))
        {
            return k;
        }
    }
    return QString();
}
QString Renderer_ConfigParse::getStringValue(QString key)
{
    if(_strMap.contains(key))
    {
        return _strMap.value(key);
    }
    return QString();
}
int Renderer_ConfigParse::getIntValue(QString key)
{
    if(_intMap.contains(key))
    {
        return _intMap.value(key);
    }
    return INT_MAX;
}
bool Renderer_ConfigParse::getBoolValue(QString key)
{
    if(_boolMap.contains(key))
    {
        return _boolMap.value(key);
    }
    return false;
}
