TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

LIBS	+= -L/usr/lib/pgsql -lpq -L../../../bin/lib -ldncdllog -ldncencoding -ldncreadconf -ldncpgsql -ldncaddonfunc -ldncreceipt -ldncsetupparam -ldncutil -ldncdecoder -ldncwaresystem -ldncdbmanager -ldncutil -ldncpersistent

INCLUDEPATH	+= /usr/include/pgsql ../../../bin/include

HEADERS	+= load_file.h \
	read_parameters.h \
	error_strings_log.h \
	upload_const.h \
	shtrih_load.h \
	dnc_load.h \
	DncCommand.h

SOURCES	+= main.cpp \
	load_file.cpp \
	read_parameters.cpp \
	error_strings_log.cpp \
	shtrih_load.cpp \
	dnc_load.cpp \
	DncCommand.cpp

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



