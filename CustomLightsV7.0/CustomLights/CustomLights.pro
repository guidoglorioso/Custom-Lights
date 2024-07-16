QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += serialport
QT += multimedia
QT += core gui winextras opengl
QT += core gui network
CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -lgdi32 -lOpengl32 -lglu32

SOURCES += \
    audioinfo.cpp \
    ayudared.cpp \
    confighw.cpp \
    configuracionesp32.cpp \
    configuracionserie.cpp \
    datosusuario.cpp \
    esp32udp.cpp \
    imprimir.cpp \
    main.cpp \
    mainwindow.cpp \
    modosconfig.cpp \
    noconfigwindow.cpp \
    pantalla.cpp \
    test.cpp

HEADERS += \
    audioinfo.h \
    ayudared.h \
    confighw.h \
    configuracionesp32.h \
    configuracionserie.h \
    datosusuario.h \
    esp32udp.h \
    imprimir.h \
    mainwindow.h \
    modosconfig.h \
    noconfigwindow.h \
    pantalla.h \
    test.h

FORMS += \
    ayudared.ui \
    confighw.ui \
    mainwindow.ui \
    noconfigwindow.ui \
    test.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_ICONS = iconoApp2.ico
