TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

HEADERS	+= accessrights.h \
	class_pgsql.h \
	get_param_pgsql.h \
	reg_pgsql.h \
	setup_dis_pgsql.h \
	setup_pgsql.h \
	trans_data.h \
	trans_pgsql.h \
	unload_pgsql.h \
	upload_pgsql.h \
	setup_doc_pgsql.h \
	setup_sam_pgsql.h

SOURCES	+= accessrights.cpp \
	class_pgsql.cpp \
	get_param_pgsql.cpp \
	reg_pgsql.cpp \
	setup_dis_pgsql.cpp \
	setup_pgsql.cpp \
	trans_data.cpp \
	trans_pgsql.cpp \
	unload_pgsql.cpp \
	upload_pgsql.cpp \
	setup_doc_pgsql.cpp \
	setup_sam_pgsql.cpp

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}



