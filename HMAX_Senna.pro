#-------------------------------------------------
#
# Project created by QtCreator 2014-04-19T17:28:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

QMAKE_CXXFLAGS -= -O
QMAKE_CXXFLAGS -= -O1
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS *= -O3
# Otimiza para a plataforma intel i*
QMAKE_CXXFLAGS += -march=corei7 -mtune=corei7

TARGET = HMAX_Senna
TEMPLATE = app

SOURCES += main.cpp\
    HMax_Class/s1th.cpp \
    HMax_Class/c1th.cpp \
    HMax_Class/c2th.cpp \
    HMax_Class/c1pathdiccreator.cpp \
    processaimagem.cpp \
    adicionaclasse.cpp \
    processaclasses.cpp \
    ../mainwindow.cpp \
    mainwindow.cpp

HEADERS  += mainwindow.h \
    HMax_Class/s1th.h \
    HMax_Class/baseStructs.h \
    HMax_Class/c1th.h \
    HMax_Class/c2th.h \
    HMax_Class/c1pathdiccreator.h \
    processaimagem.h \
    adicionaclasse.h \
    estruturas.h \
    processaclasses.h

FORMS    += mainwindow.ui \
    adicionaclasse.ui

INCLUDEPATH += /usr/local/include/
INCLUDEPATH += /usr/local/include/opencv2/
INCLUDEPATH += /usr/local/include/opencv/
DEPENDPATH += /usr/local/lib

LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_features2d -lopencv_calib3d

OTHER_FILES += \
    LICENSE.txt \
    GLP.txt

RESOURCES += \
    imgs.qrc
