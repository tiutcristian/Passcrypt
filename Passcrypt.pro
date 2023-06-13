QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
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

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    infos

LIBS += -lsodium
