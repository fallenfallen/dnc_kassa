TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

HEADERS	+= receipt.h \
	copyinfo.h \
	discount.h \
	recordreceipt.h \
	taxrate.h \
	positiontaxes.h

SOURCES	+= receipt.cpp \
	copyinfo.cpp \
	discount.cpp \
	recordreceipt.cpp \
	taxrate.cpp \
	positiontaxes.cpp

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



