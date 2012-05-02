TEMPLATE	= lib
LANGUAGE	= C++

CONFIG	+= plugin

LIBS	+= -L/usr/lib -L/usr/lib/pgsql/ -lpq -L../../../bin/lib -ldncpgsql -ldncdkbinput -ldnchwsigcontroller

INCLUDEPATH	+= /usr/include/pgsql/ ../../../bin/include

FORMS	= authorizationform.ui

IMAGES	= image/Untitled-1.png

TARGET = dncauthform

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}




