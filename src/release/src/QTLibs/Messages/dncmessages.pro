TEMPLATE	= lib
LANGUAGE	= C++

CONFIG	+= plugin

LIBS	+= -L/usr/lib -L../../../bin/lib -ldncsetupparam -ldncblocktimer -ldncdkbinput

INCLUDEPATH	+= /usr/include/pgsql ../../../bin/include

HEADERS	+= messages.h

SOURCES	+= messages.cpp

FORMS	= form/messageform.ui

IMAGES	= img/error.png \
	img/info.png

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

