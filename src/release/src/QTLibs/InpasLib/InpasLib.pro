TEMPLATE	= lib
LANGUAGE	= C++

CONFIG	+= plugin

LIBS	+= -L/usr/lib -L ../../../bin/lib -lhwsrv

INCLUDEPATH	+= ../../../bin/include

FORMS	= additionalrequestForm.ui \
	npaspayform.ui \
	npasreconciliationform.ui

TARGET = dncinpas

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

