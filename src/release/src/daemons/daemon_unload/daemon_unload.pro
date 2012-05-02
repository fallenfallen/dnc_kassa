TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

LIBS	+= -L/usr/lib/pgsql/ -lpq -L../../../bin/lib -ldncreadconf -ldncpgsql -ldncdllog -ldncaddonfunc -ldncsetupparam -ldncencoding -ldncreceipt -ldncwaresystem -ldncdbmanager -ldncpersistent -ldncutil

INCLUDEPATH	+= . ../../../bin/include /usr/include/pgsql/

HEADERS	+= Command.h \
	Writer.h \
	const_command.h \
	const_writer.h \
	const_main.h

SOURCES	+= Command.cpp \
	main.cpp \
	Writer.cpp

	
unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
