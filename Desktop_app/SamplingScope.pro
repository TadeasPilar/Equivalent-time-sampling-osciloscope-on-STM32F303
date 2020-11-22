QT       += core gui charts serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = icon.ico
ICON = icon.icns

SOURCES += \
    about.cpp \
    cursors.cpp \
    generator.cpp \
    help.cpp \
    infobar.cpp \
    main.cpp \
    mathmatics.cpp \
    nucleoscope.cpp \
    scopedisplay.cpp \
    scopewindow.cpp \
    serialselector.cpp \
    settings.cpp \
    waveform.cpp
    generator.cpp

HEADERS += \
    about.h \
    cursors.h \
    generator.h \
    help.h \
    infobar.h \
    mathmatics.h \
    nucleoscope.h \
    scopedisplay.h \
    scopewindow.h \
    serialselector.h \
    settings.h \
    waveform.h
    generator.h

FORMS += \
    scopewindow.ui

TRANSLATIONS += \
    SamplingScope_cs_CZ.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    logo_cvut_en.jpg

RESOURCES += \
    resources.qrc
