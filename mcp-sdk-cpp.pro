DESTDIR = $$PWD/build
TARGET = tiny-mcp-server
TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += src/

SOURCES += \
    example/main.cpp \
	src/mcpserver.cpp

HEADERS += \
    src/jstream.h \
	src/mcpserver.h \
	src/strformat.h \
	src/toi.h
