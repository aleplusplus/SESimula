#-------------------------------------------------
#
# Project created by QtCreator 2013-04-19T23:06:56
#
#-------------------------------------------------

QT       += core gui webkit network help webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SESimula
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    documentacion.cpp \
    helpviewer.cpp \
    helpenginewrapper.cpp \
    collectionconfiguration.cpp \
    findwidget.cpp \
    searchwidget.cpp \
    helpviewer_qwv.cpp \
    indexwindow.cpp \
    contentwindow.cpp \
    diagramtextitem.cpp \
    diagramscene.cpp \
    diagramitem.cpp \
    arrow.cpp \
    simuladorWidget.cpp \
    testwidget.cpp \
    aleatoriedadwidget.cpp

HEADERS  += mainwindow.h \
    generador.h \
    documentacion.h \
    helpviewer.h \
    helpenginewrapper.h \
    tracer.h \
    collectionconfiguration.h \
    findwidget.h \
    searchwidget.h \
    helpviewer_qwv.h \
    indexwindow.h \
    contentwindow.h \
    diagramtextitem.h \
    diagramscene.h \
    diagramitem.h \
    arrow.h \
    simuladorWidget.h \
    testwidget.h \
    aleatoriedadwidget.h \
    test.h

FORMS    += mainwindow.ui \
    testwidget.ui \
    aleatoriedadwidget.ui

RESOURCES += \
    sesimula.qrc
