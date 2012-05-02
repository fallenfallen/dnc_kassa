TEMPLATE	= lib
LANGUAGE	= C++

CONFIG	+= plugin

INCLUDEPATH	+= ../../../bin/include

LIBS += -L/usr/lib/

FORMS	= dialognewscheme.ui

TARGET = dncdialognewscheme

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
