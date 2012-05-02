TEMPLATE	= lib
LANGUAGE	= C++

CONFIG	+= plugin

HEADERS	+= dedicationcombobox.h

SOURCES	+= dedicationcombobox.cpp

LIBS += -L/usr/lib/

TARGET = dncdedicationcombobox

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



