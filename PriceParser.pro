#-------------------------------------------------
#
# Project created by QtCreator 2015-04-16T16:05:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PriceParcer
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp \
           xlreader.cpp \
    table.cpp \
    model.cpp \
    inputmodel.cpp \
    myproxymodel.cpp \
    outputmodel.cpp \
    xlwriter.cpp \
    firmparamdialog.cpp \
    pathparamdialog.cpp \
    drugdilersdialog.cpp


HEADERS  += mainwindow.h \
            xlreader.h \
    table.h \
    model.h \
    inputmodel.h \
    myproxymodel.h \
    outputmodel.h \
    xlwriter.h \
    firmparamdialog.h \
    pathparamdialog.h \
    drugdilersdialog.h

FORMS    += mainwindow.ui



unix{
LIBS += -L/usr/local/lib -lfreexl
}

win32{
    SOURCES +=  freexl_lib/freexl.c\
                freexl_lib/localcharset.c

    HEADERS  += freexl_lib/freexl.h \
                freexl_lib/freexl_internals.h \
                freexl_lib/config.h
}

include(qtxlsxwriter_lib/src/xlsx/qtxlsx.pri)


RESOURCES += \
    rc.qrc

win32:RC_FILE = app_icon.rc

QMAKE_LFLAGS += -Wl,-rpath,"'\$$ORIGIN'"
