TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

LIBS	+= -L/usr/lib/pgsql -L../../../bin/lib -ldncreadconf -ldncpgsql -lhwsrv -ldncaddonfunc -ldnckkmregistration -ldncreceipt -ldncstatestore -ldnctimedate -ldncdllog -ldncencoding -ldncsetupparam -ldncblocktimer -ldncdkbinput -ldncdialognewscheme -ldncinserttransaction -ldnchwsigcontroller -ldncmessages -ldncauthform -ldncauthobj -ldncservice -ldncsetup -ldncinpas -ldncutil -ldncwaresystem -ldncdbmanager -ldncpersistent

INCLUDEPATH	+= /usr/include/pgsql ../../../bin/include

SOURCES	+= main.cpp

FORMS	= exitform.ui \
	aboutform.ui \
	mainmenuform.ui

IMAGES	= images/DNC.png \
	images/1.png \
	images/2.png \
	images/3.png \
	images/4.png \
	images/5.png \
	images/6.png \
	images/7.png \
	images/8.png \
	images/logo.png

######################################################################
# Automatically generated by qmake (1.07a) Thu Jun 4 12:03:57 2009
######################################################################


unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



