TEMPLATE	= lib
LANGUAGE	= C++

CONFIG	+= plugin

INCLUDEPATH	+= .

HEADERS	+= time_date.h

SOURCES	+= time_date.cpp

TARGET = dnctimedate


LIBS += -L/usr/lib

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



######################################################################
# Automatically generated by qmake (1.07a) Tue May 19 15:25:52 2009
######################################################################


# Input
