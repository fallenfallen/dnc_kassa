TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

LIBS	+= -L /usr/lib/pgsql -lpq -L../../bin/lib -ldncpgsql -ldncaddonfunc -ldncdllog -ldncencoding -ldncsetupparam -ldncreadconf -ldncreceipt -ldncwaresystem -ldncdbmanager -ldncutil -ldncpersistent

INCLUDEPATH	+= /usr/include/pgsql ../../bin/include

HEADERS	+= update_db.h

SOURCES	+= main.cpp \
	update_db.cpp

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

