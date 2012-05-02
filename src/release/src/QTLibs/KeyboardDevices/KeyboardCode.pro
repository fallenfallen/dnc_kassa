TEMPLATE	= lib
LANGUAGE	= C++

CONFIG	+= plugin

INCLUDEPATH	+= ../../../bin/include

LIBS += -L/usr/lib

HEADERS	+= dkbinput.h \
	bcskbinput.h \
	mcrkbinput.h

SOURCES	+= dkbinput.cpp \
	bcskbinput.cpp \
	mcrkbinput.cpp

TARGET = dncdkbinput

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

