TEMPLATE	= lib
LANGUAGE	= C++

CONFIG	+= plugin

INCLUDEPATH	+= /usr/include/pgsql ../../../bin/include

LIBS += -L/usr/lib

HEADERS	+= hwsighandlerscontroller.h

SOURCES	+= hwsighandlerscontroller.cpp

TARGET += dnchwsigcontroller

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



