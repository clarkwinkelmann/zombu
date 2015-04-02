#-------------------------------------------------
#
# Project created by QtCreator 2014-01-31T13:38:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Zombu
TEMPLATE = app


SOURCES += main.cpp\
        mainfrm.cpp \
    gamescene.cpp \
    sprite.cpp \
    gamecore.cpp \
    resources.cpp \
    hero.cpp \
    missile.cpp \
    zombie.cpp \
    gamestatus.cpp \
    obstacle.cpp \
    libzombu.cpp \
    player.cpp \
    mysteryword.cpp \
    amelioration.cpp \
    wordlibrary.cpp \
    apptranslator.cpp

HEADERS  += mainfrm.h \
    gamescene.h \
    sprite.h \
    gamecore.h \
    resources.h \
    hero.h \
    missile.h \
    zombie.h \
    gamestatus.h \
    obstacle.h \
    libzombu.h \
    player.h \
    mysteryword.h \
    amelioration.h \
    wordlibrary.h \
    apptranslator.h

FORMS    += mainfrm.ui

TRANSLATIONS = locales/en.ts
