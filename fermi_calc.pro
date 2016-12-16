#-------------------------------------------------
#
# Project created by QtCreator 2016-11-08T23:31:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = fermi_calc
TEMPLATE = app


SOURCES += main.cpp\
    qcustomplot.cpp \
    customdoublespinbox.cpp \
    fermicalcwindow.cpp \
    fermicalc.cpp

HEADERS  += \
    qcustomplot.h \
    customdoublespinbox.h \
    fermicalcwindow.h \
    fermicalc.h

FORMS    += \
    fermicalcwindow.ui

LIBS    += \
    -lboost_program_options
