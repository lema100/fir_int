QT       += core gui charts widgets

CONFIG += c++1z

SOURCES += \
	../fir_int/fir_int.c \
	main.cpp \
	mainwindow.cpp

HEADERS += \
	../fir_int/fir_int.h \
	main.h \
	mainwindow.h

FORMS += \
	mainwindow.ui
