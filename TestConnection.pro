#-------------------------------------------------
#
# Project created by QtCreator 2013-03-11T11:08:11
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui

TARGET = TestConnection
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    filesaver.cpp \
    downloadmanager.cpp \
    downloadmember.cpp \
    speedcounter.cpp \
    sender.cpp

HEADERS += \
    filesaver.h \
    downloadmanager.h \
    downloadmember.h \
    speedcounter.h \
    sender.h
