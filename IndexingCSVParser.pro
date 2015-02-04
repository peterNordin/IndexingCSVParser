TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR = $${PWD}/

SOURCES += main.cpp \
    IndexingCSVParser.cpp

HEADERS += \
    Converters.hpp \
    IndexingCSVParser.h \
    IndexingCSVParserImpl.hpp

