TEMPLATE = app

QT += network sql

CONFIG += console

INCLUDEPATH += /opt/postgres/9.0/include
LIBS += -L/usr/include/postgresql/ -lpq -lcrypto -lssl

MOC_DIR = moc
OBJECTS_DIR = obj
TARGET = dnc_edsd

SOURCES += \
    main.cpp \
    manager.cpp \
    server.cpp \
    translate.cpp \
    datamanager.cpp \
    networkrequest.cpp \
    databaseengine.cpp \
    exception.cpp \
    cryptoengine.cpp \
    crypto/rijndael.cpp \
    settings.cpp

HEADERS += \
    request.h \
    manager.h \
    server.h \
    exception.h \
    translate.h \
    datamanager.h \
    networkrequest.h \
    databaseengine.h \
    cryptoengine.h \
    crypto/rijndael.h \
    settings.h
