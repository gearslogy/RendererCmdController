QT += core network
QT -= gui

TARGET = RendererHost
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    RendererHost.cpp \
    Renderer_Process.cpp \
    Renderer_ConfigParse.cpp

HEADERS += \
    RendererHost.h \
    RendererTask.h \
    Renderer_Process.h \
    Renderer_ConfigParse.h

