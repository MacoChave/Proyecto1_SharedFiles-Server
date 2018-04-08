#-------------------------------------------------
#
# Project created by QtCreator 2018-03-15T20:05:20
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Proyecto1_SharedFiles_Server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    SparseMatrix/matrix.cpp \
    SparseMatrix/Interno/columnlist.cpp \
    SparseMatrix/Interno/matrixnode.cpp \
    SparseMatrix/Interno/rowlist.cpp \
    SparseMatrix/Tad/tadcolumn.cpp \
    SparseMatrix/Tad/tadmatrixnode.cpp \
    SparseMatrix/Tad/tadrow.cpp

HEADERS  += mainwindow.h \
    Lista/list.h \
    Lista/node.h \
    SparseMatrix/matrix.h \
    SparseMatrix/Interno/columnlist.h \
    SparseMatrix/Interno/matrixnode.h \
    SparseMatrix/Interno/rowlist.h \
    SparseMatrix/MatrixHeader/headercolumn.h \
    SparseMatrix/MatrixHeader/headerrow.h \
    SparseMatrix/Tad/tadcolumn.h \
    SparseMatrix/Tad/tadmatrixnode.h \
    SparseMatrix/Tad/tadrow.h

FORMS    += mainwindow.ui

DISTFILES += \
    .gitignore
