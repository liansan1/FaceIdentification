#-------------------------------------------------
#
# Project created by QtCreator 2021-01-07T11:37:31
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Face_demo0107
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    daofactory.cpp \
    userdao.cpp \
    userentity.cpp \
    historyform.cpp \
    clockdao.cpp \
    clockentity.cpp

HEADERS  += widget.h \
    daofactory.h \
    userdao.h \
    userentity.h \
    historyform.h \
    clockdao.h \
    clockentity.h

FORMS    += widget.ui \
    historyform.ui

INCLUDEPATH += E:/opencv_20075/opencv3.4-install/install/include
INCLUDEPATH += E:/opencv_20075/opencv3.4-install/install/include/opencv
INCLUDEPATH += E:/opencv_20075/opencv3.4-install/install/include/opencv2
LIBS += E:/opencv_20075/opencv3.4-install/install/x86/mingw/lib/libopencv_*.a

