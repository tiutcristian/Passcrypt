QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    clipboard.cpp \
    constants.cpp \
    createnew.cpp \
    database.cpp \
    editpass.cpp \
    encrypt.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    signupdialog.cpp

HEADERS += \
    clipboard.h \
    constants.h \
    createnew.h \
    database.h \
    editpass.h \
    encrypt.h \
    logindialog.h \
    mainwindow.h \
    signupdialog.h

FORMS += \
    createnew.ui \
    editpass.ui \
    logindialog.ui \
    mainwindow.ui \
    signupdialog.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    infos

LIBS += -L "P:\libsodium-win64\bin" -lsodium-23

INCLUDEPATH += "P:\libsodium-win64\include"

QMAKE_POST_LINK += $$QMAKE_COPY "P:\libsodium-win64\bin\libsodium-23.dll" .
