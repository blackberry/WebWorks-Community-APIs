APP_NAME = MediaKeys
TEMPLATE = lib

CONFIG += qt warn_on

QT += core declarative
QT -= gui

include(config.pri)

LIBS += -lcpp  -lbb -lbbdata -lbbdevice -lbbcascades -lscreen -lbbplatform -lbbsystem -lQtCore -lbbmultimedia

QMAKE_CFLAGS   += -Wno-psabi
QMAKE_CXXFLAGS += -Wno-psabi

CONFIG(release, debug|release) {
    TEMPLATE = lib
    QMAKE_CFLAGS += -fstack-protector-strong -mthumb -Os
    QMAKE_CXXFLAGS += -fstack-protector-strong -mthumb -Os
    QMAKE_LFLAGS += -Wl,-z,relro
    DEFINES += _FORTIFY_SOURCE=2
    QMAKE_CFLAGS += -Og
}

CONFIG(debug, debug|release) {
    QMAKE_CFLAGS += -Og
}

OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .rcc
UI_DIR = .ui
