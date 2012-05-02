TEMPLATE	= lib
LANGUAGE	= C++

CONFIG	+= plugin

HEADERS	+= blocktimer.h

SOURCES	+= blocktimer.cpp

TARGET = dncblocktimer

LIBS += -L/usr/lib/

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



