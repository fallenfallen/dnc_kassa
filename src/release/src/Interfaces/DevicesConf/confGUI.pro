TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

LIBS	+= -L../../../bin/lib -ldncaddonfunc -ldncreadconf -ldncpgsql -lhwsrv -ldncdllog -ldncdedicationcombobox -ldncdialognewscheme -ldncnewschemeperec -ldncmessages -ldncsetupparam -ldncencoding -ldncreceipt -ldncblocktimer -ldncdkbinput -ldncwaresystem -ldncutil -ldncpersistent -ldncdbmanager

INCLUDEPATH	+= /usr/include/pgsql ../../../bin/include

HEADERS	+= conf_const.h

SOURCES	+= main.cpp \
	const.cpp

FORMS	= configurations.ui \
	scannertest.ui \
	mcrtest.ui \
	newperecodirovka.ui \
	autoform.ui \
	insertkod.ui \
	ewtest.ui

IMAGES	= Arrows.png \
	Edit.png

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
