TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += debug_and_release
CONFIG += static

INCLUDEPATH	+=./

SOURCES += \
	rbtree_example.c \
	rbtree.c \
	utility_routines.c

HEADERS += \
    rbtree.h

#LIBS		+= -static

CONFIG (debug, debug|release) {
	#CONFIG	+= warn_off
	DEFINES	+= _DEBUG=1 __TRACE__=1

} else {
	#CONFIG	+= warn_off
	DEFINES	+= _DEBUG=1 __TRACE__=1
}
