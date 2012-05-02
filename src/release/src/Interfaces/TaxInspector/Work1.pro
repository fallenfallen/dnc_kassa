TEMPLATE = app

MOC_DIR = moc
OBJECTS_DIR = obj
TARGET = dnc_taxinspector

INCLUDEPATH += ../../../bin/include
LIBS += -L../../../bin/lib -lhwsrv

SUBDIRS = SearchDeviceApp

HEADERS += \
    mainwindow.h \
    frmodel.h \
    frparam.h \
    calendar.h \
    eklsmodel.h \
    model.h \
    hwserver.h \
    progressbar.h \
    translator.h \
    SearchDeviceApp/searchprogressbar.h \
    SearchDeviceApp/searchdevicewindow.h \
    SearchDeviceApp/searchdevice.h \
    SearchDeviceApp/connectoptions.h \
    error.h

SOURCES += \
    mainwindow.cpp \
    main.cpp \
    frmodel.cpp \
    calendar.cpp \
    eklsmodel.cpp \
    model.cpp \
    hwserver.cpp \
    progressbar.cpp \
    translator.cpp \
    SearchDeviceApp/searchprogressbar.cpp \
    SearchDeviceApp/searchdevicewindow.cpp \
    SearchDeviceApp/searchdevice.cpp \
    error.cpp

RESOURCES += \
    resource/resource.qrc
