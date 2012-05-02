TEMPLATE	= lib
LANGUAGE	= C++

CONFIG	+= plugin

LIBS	+= -L/usr/lib

INCLUDEPATH	+= /usr/include/pgsql ../../../bin/include

HEADERS	+= cdsp.h

SOURCES	+= cdsp.cpp

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

TARGET += dnccdspview
