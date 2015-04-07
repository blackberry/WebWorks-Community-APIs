/****************************************************************************
** Meta object code from reading C++ file 'NowPlaying_ndk.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/NowPlaying_ndk.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NowPlaying_ndk.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_webworks__NowPlayingNDK[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   24,   24,   24, 0x05,
      38,   24,   24,   24, 0x05,
      51,   24,   24,   24, 0x05,

 // slots: signature, parameters, type, tag, flags
      65,   24,   24,   24, 0x0a,
      72,   24,   24,   24, 0x0a,
      79,   24,   24,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_webworks__NowPlayingNDK[] = {
    "webworks::NowPlayingNDK\0\0playSignal()\0"
    "stopSignal()\0pauseSignal()\0play()\0"
    "stop()\0pause()\0"
};

void webworks::NowPlayingNDK::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        NowPlayingNDK *_t = static_cast<NowPlayingNDK *>(_o);
        switch (_id) {
        case 0: _t->playSignal(); break;
        case 1: _t->stopSignal(); break;
        case 2: _t->pauseSignal(); break;
        case 3: _t->play(); break;
        case 4: _t->stop(); break;
        case 5: _t->pause(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData webworks::NowPlayingNDK::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject webworks::NowPlayingNDK::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_webworks__NowPlayingNDK,
      qt_meta_data_webworks__NowPlayingNDK, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &webworks::NowPlayingNDK::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *webworks::NowPlayingNDK::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *webworks::NowPlayingNDK::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_webworks__NowPlayingNDK))
        return static_cast<void*>(const_cast< NowPlayingNDK*>(this));
    return QObject::qt_metacast(_clname);
}

int webworks::NowPlayingNDK::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void webworks::NowPlayingNDK::playSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void webworks::NowPlayingNDK::stopSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void webworks::NowPlayingNDK::pauseSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
