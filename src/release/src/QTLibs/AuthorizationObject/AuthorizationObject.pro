TEMPLATE	= lib
LANGUAGE	= C++

CONFIG	+= plugin

LIBS	+=  -L/usr/lib -L../../../bin/lib -lhwsrv -ldncblocktimer -ldncsetupparam -ldncauthform

INCLUDEPATH	+= /usr/include/pgsql ../../../bin/include

HEADERS	+= authorization.h

SOURCES	+= authorization.cpp

TARGET += dncauthobj

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



