#-------------------------------------------------
#
# Project created by QtCreator 2015-11-01T21:43:40
#
#-------------------------------------------------

QT       += core gui testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#QMAKE_CXXFLAGS_DEBUG += -D TEST

TARGET = StateMachineCreator
TEMPLATE = app

CONFIG+=testcase

SOURCES += main.cpp\
        mainwindow.cpp \
    projectdialog.cpp \
    codegenerate.cpp \
    testcodegenerate.cpp \
    treeitem.cpp \
    statemodel.cpp \
    teststatemodel.cpp \
    treemodel.cpp \
    variabledialog.cpp

HEADERS  += mainwindow.h \
    projectdialog.h \
    codegenerate.h \
    testcodegenerate.h \
    treeitem.h \
    statemodel.h \
    teststatemodel.h \
    treemodel.h \
    variabledialog.h

FORMS    += mainwindow.ui \
    projectdialog.ui \
    variabledialog.ui
