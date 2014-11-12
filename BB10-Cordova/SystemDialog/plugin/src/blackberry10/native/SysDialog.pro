APP_NAME = SysDialog
TEMPLATE = lib

CONFIG += qt warn_on

QT += core declarative
QT -= gui

include(config.pri)

LIBS += -lcpp -lslog2 -lbb -lbbdata -lbbdevice -lbbcascades -lscreen

QMAKE_CFLAGS   += -Wno-psabi
QMAKE_CXXFLAGS += -Wno-psabi

device {
    QNX_VARIANT = so.le-v7
}

simulator {
    QNX_VARIANT = so
}

CONFIG(release, debug|release) {
    TEMPLATE = lib
    DESTDIR = $$QNX_VARIANT
    #QMAKE_CFLAGS += -fstack-protector-strong -fvisibility=hidden -mthumb -Os
    #QMAKE_CXXFLAGS += -fstack-protector-strong -fvisibility=hidden -mthumb -Os
    #QMAKE_LFLAGS += -Wl,-z,relro
    #DEFINES += _FORTIFY_SOURCE=2
    QMAKE_CFLAGS += -Og
}

CONFIG(debug, debug|release) {
    DESTDIR = $$QNX_VARIANT-g
    QMAKE_CFLAGS += -Og
}

profile {
    QMAKE_CXXFLAGS += -g -finstrument-functions
    LIBS += -lprofilingS
}

OBJECTS_DIR = $${DESTDIR}/.obj
MOC_DIR = $${DESTDIR}/.moc
RCC_DIR = $${DESTDIR}/.rcc
UI_DIR = $${DESTDIR}/.ui
