#-------------------------------------------------
#
# Project created by QtCreator 2015-12-27T13:42:23
#
#-------------------------------------------------

QT       += core gui network
CONFIG   += c++11

win32 {
    QT += winextras
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtercon
TEMPLATE = app

RC_ICONS = resources/codlogo.ico
VERSION = 0.1.0.9
QMAKE_TARGET_PRODUCT = QteRcon
QMAKE_TARGET_DESCRIPTION = QteRcon
GIT_VERSION = $$system(git --git-dir $$PWD/.git --work-tree $$PWD describe --always --tags)

DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_NAME=\\\"$$QMAKE_TARGET_PRODUCT\\\"
DEFINES += GIT_VERSION=\\\"$$GIT_VERSION\\\"

SOURCES += main.cpp\
        serverwindow.cpp \
    network\rcon.cpp \
    network\query.cpp \
    parser/outputparser.cpp \
    parser/output.cpp \
    commandwindow.cpp \
    network/server.cpp \
    parser/statusparser.cpp \
    parser/player.cpp \
    parser/status.cpp \
    playertablemodel.cpp \
    htmldelegate.cpp

HEADERS  += serverwindow.h \
    network\rcon.h \
    network\query.h \
    parser/outputparser.h \
    parser/output.h \
    commandwindow.h \
    network/server.h \
    parser/statusparser.h \
    parser/player.h \
    parser/status.h \
    playertablemodel.h \
    htmldelegate.h

FORMS    += \
    commandwindow.ui \
    serverwindow.ui

RESOURCES += \
    application.qrc
