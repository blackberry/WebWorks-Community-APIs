APP_NAME = QtBridge
TEMPLATE = lib

CONFIG += qt warn_on

QT += core declarative
QT -= gui

# Enable full Qt-Bridge debugging
# DEFINES += DEBUG_QtBridge

include(config.pri)

LIBS += -lcpp -lslog2 -lbb -lbbdata -lbbdevice -lbbcascades -lscreen

QMAKE_CXXFLAGS += -Wno-psabi

# Temporary until PR-212885 is resolved.
QMAKE_LFLAGS += -Wl,-z,relro -Wl,-z,now -Wl,--as-needed -Wl,--no-undefined

device {
    QNX_VARIANT = so.le-v7
}

simulator {
    QNX_VARIANT = so
}

CONFIG(release, debug|release) {
    TEMPLATE = lib
    DESTDIR = $$QNX_VARIANT
    QMAKE_CFLAGS += -fstack-protector-strong -fvisibility=hidden -mthumb -Os
    QMAKE_CXXFLAGS += -fstack-protector-strong -fvisibility=hidden -mthumb -Os
    QMAKE_LFLAGS += -Wl,-z,relro
    DEFINES += _FORTIFY_SOURCE=2
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
