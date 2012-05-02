TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

LIBS	+= -L/usr/lib/pgsql -lpq -L../../../bin/lib -lhwsrv -ldncpgsql -ldncreceipt -ldncdllog -ldncdkbinput -ldncmessages -ldncsetupparam -ldncblocktimer -ldncaddonfunc -ldncreadconf -ldncencoding -ldnchwsigcontroller -ldncwaresystem -ldncutil -ldncdbmanager -ldncpersistent

INCLUDEPATH	+= /usr/include/pgsql/ ../../../bin/include

SOURCES	+= main.cpp

FORMS	= accessrightsform.ui \
	profileform.ui \
	userform.ui

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}


