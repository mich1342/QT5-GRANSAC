#--------------------------------------------------------
#
# Project created by Michael Jonathan 2021-12-20T11:26:40
#
#--------------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DSP_openCV_Demo1
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    MatToQImage.cpp

HEADERS  += mainwindow.h \
    AbstractModel.hpp \
    GRANSAC.hpp \
    LineModel.hpp \
    MatToQImage.h
    unix {

        QMAKE_CXXFLAGS += -std=c++1y
        QMAKE_CXXFLAGS += -Wall
        # OpenCv Configuration opencv-4.2.0
        INCLUDEPATH += "/usr/include/opencv4/opencv2"
        INCLUDEPATH += "/usr/include/opencv4"
        LIBS += -L~/usr/lib/x86_64-linux-gnu \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_imgcodecs \
        -lopencv_objdetect \
        -lopencv_features2d \
        -lopencv_calib3d \
        -lopencv_video\
        -lopencv_videoio

        # OpenMP Configuration
        INCLUDEPATH += "/usr/lib/gcc/x86_64-linux-gnu/7.5.0/include"
        LIBS += -L~/usr/lib/gcc/x86_64-linux-gnu/7.5.0\
        -lgomp
    }

FORMS    += mainwindow.ui

