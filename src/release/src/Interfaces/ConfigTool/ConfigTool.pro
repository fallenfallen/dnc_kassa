TEMPLATE = app

QT += network sql

INCLUDEPATH += /opt/postgres/9.0/include
LIBS += -L/usr/include/postgresql/ -lpq -lcrypto -lssl

MOC_DIR = moc
OBJECTS_DIR = obj
TARGET = dnc_configtool

HEADERS += \
    translate.h \
    dbconfigdialog.h \
    dialog.h \
    manager.h \
    databaseengine.h \
    settings.h \
    exception.h \
    blinklabel.h \
    systemconfigdialog.h \
    proxyserverconfigdialog.h \
    networkrequest.h \
    serverclient.h

SOURCES += \
    translate.cpp \
    main.cpp \
    dbconfigdialog.cpp \
    dialog.cpp \
    manager.cpp \
    databaseengine.cpp \
    settings.cpp \
    exception.cpp \
    blinklabel.cpp \
    systemconfigdialog.cpp \
    proxyserverconfigdialog.cpp \
    networkrequest.cpp \
    serverclient.cpp

RESOURCES += \
    resource.qrc
