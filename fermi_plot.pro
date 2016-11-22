#-------------------------------------------------
#
# Project created by QtCreator 2016-11-08T23:31:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = fermi_plot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    fermiplot.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    fermiplot.h

FORMS    += \
    mainwindow.ui

LIBS    += \
    -lboost_program_options
