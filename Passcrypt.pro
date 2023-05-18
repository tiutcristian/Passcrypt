QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    database.cpp \
    editpass.cpp \
    encrypt.cpp \
    generate.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    save_pass_dialog.cpp \
    signupdialog.cpp

HEADERS += \
    database.h \
    editpass.h \
    encrypt.h \
    generate.h \
    logindialog.h \
    mainwindow.h \
    save_pass_dialog.h \
    signupdialog.h

FORMS += \
    editpass.ui \
    generate.ui \
    logindialog.ui \
    mainwindow.ui \
    save_pass_dialog.ui \
    signupdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    infos

unix:!macx|win32: LIBS += -L$$PWD/'../Cryptlib compiled/' -lcryptopp

INCLUDEPATH += $$PWD/'../Cryptlib compiled'
DEPENDPATH += $$PWD/'../Cryptlib compiled'

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/'../Cryptlib compiled/cryptopp.lib'
else:unix:!macx|win32-g++: PRE_TARGETDEPS += $$PWD/'../Cryptlib compiled/libcryptopp.a'
