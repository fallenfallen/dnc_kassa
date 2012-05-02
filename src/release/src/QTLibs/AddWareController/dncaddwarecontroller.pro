TEMPLATE	= lib
LANGUAGE	= C++

CONFIG	+= plugin

LIBS	+= -L/usr/lib -L../../../bin/lib -ldncpgsql -ldncreceipt -ldncstatestore -ldnccdspview -ldnchwsigcontroller -ldncdecoder -ldncwaresystem -ldncutil -ldncdbmanager

INCLUDEPATH	+= /usr/include/pgsql ../../../bin/include

HEADERS	+= addwarecontroller.h

SOURCES	+= addwarecontroller.cpp

FORMS	= wareaddition_form.ui

TARGET += dncaddwarecontroller

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

