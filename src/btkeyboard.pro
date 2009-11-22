#QMake project

QT -= gui

HEADERS += btkeyboard.h
SOURCES += btkeyboard.cpp main.cpp

QT += dbus

CONFIG += link_pkgconfig
PKGCONFIG += glib-2.0


