TEMPLATE	= lib
LANGUAGE	= C++

CONFIG	+= plugin

LIBS += -L/usr/lib

TARGET = dncstriketabitm

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

HEADERS	+= striketabitm.h

SOURCES	+= striketabitm.cpp

