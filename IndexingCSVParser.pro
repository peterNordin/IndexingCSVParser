TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR = $${PWD}/

SOURCES += main.cpp \
    IndexingCSVParser.cpp

HEADERS += \
    IndexingCSVParser.h \
    IndexingCSVParserImpl.hpp

