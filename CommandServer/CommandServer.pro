QT += core network
QT -= gui

TARGET = CommandServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    Cmd_TcpServer.cpp \
    Cmd_TcpSocket.cpp

HEADERS += \
    Cmd_TcpServer.h \
    Cmd_TcpSocket.h \
    Cmd_MemberInfo.h

