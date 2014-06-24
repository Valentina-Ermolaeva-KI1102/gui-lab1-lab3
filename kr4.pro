TEMPLATE = app
TARGET = kr4
DESTDIR = ../Release
QT += core widgets gui
CONFIG += release
DEFINES += QT_DLL QT_WIDGETS_LIB
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Release
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(kr4.pri)
