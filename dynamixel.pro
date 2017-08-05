#-------------------------------------------------
#
# Project created by QtCreator 2017-06-19T02:05:11
#
#-------------------------------------------------

QT       += core serialport testlib network

QT       -= gui

TARGET = dynamixel
CONFIG   += console
CONFIG   -= app_bundle

target.path += /home/pi/Desktop
INSTALLS += target

TEMPLATE = app


SOURCES += main.cpp \
    dynamixel.cpp \
    Media/media.cpp \


HEADERS += \
    dynamixel.h \
    Media/media.h \

