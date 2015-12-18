/****************************************************************************
** Meta object code from reading C++ file 'NowPlaying_ndk.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/NowPlaying_ndk.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NowPlaying_ndk.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_webworks__NowPlayingNDK[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   24,   24,   24, 0x05,
      38,   24,   24,   24, 0x05,
      52,   24,   24,   24, 0x05,
      65,   24,   24,   24, 0x05,
      78,   24,   24,   24, 0x05,
     108,   95,   24,   24, 0x05,

 // slots: signature, parameters, type, tag, flags
     140,  128,   24,   24, 0x0a,
     157,   24,   24,   24, 0x0a,
     168,   24,   24,   24, 0x0a,
     180,   24,   24,   24, 0x0a,
     191,   24,   24,   24, 0x0a,
     202,   24,   24,   24, 0x0a,
     217,   95,   24,   24, 0x0a,
     242,   24,  235,   24, 0x0a,
     281,  270,   24,   24, 0x0a,
     316,  270,   24,   24, 0x0a,
     352,  270,   24,   24, 0x0a,
     387,  270,   24,   24, 0x0a,
     422,  270,   24,   24, 0x0a,
     461,  270,   24,   24, 0x0a,
     502,  497,  235,   24, 0x0a,
     525,   24,  235,   24, 0x0a,
     543,   24,  235,   24, 0x0a,
     562,   24,  235,   24, 0x0a,
     579,   24,  235,   24, 0x0a,
     596,   24,  235,   24, 0x0a,
     617,  497,  235,   24, 0x0a,
     641,   24,  235,   24, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_webworks__NowPlayingNDK[] = {
    "webworks::NowPlayingNDK\0\0playSignal()\0"
    "pauseSignal()\0stopSignal()\0nextSignal()\0"
    "previousSignal()\0errorMessage\0"
    "errorSignal(string)\0windowGroup\0"
    "joinSlot(string)\0playSlot()\0pauseSlot()\0"
    "stopSlot()\0nextSlot()\0previousSlot()\0"
    "errorSlot(string)\0string\0"
    "NowPlayingRequestPlayback()\0callbackId\0"
    "NowPlayingBindPlayCallback(string)\0"
    "NowPlayingBindPauseCallback(string)\0"
    "NowPlayingBindStopCallback(string)\0"
    "NowPlayingBindNextCallback(string)\0"
    "NowPlayingBindPreviousCallback(string)\0"
    "NowPlayingBindErrorCallback(string)\0"
    "data\0NowPlayingPlay(string)\0"
    "NowPlayingPause()\0NowPlayingResume()\0"
    "NowPlayingStop()\0NowPlayingNext()\0"
    "NowPlayingPrevious()\0NowPlayingError(string)\0"
    "NowPlayingGetState()\0"
};

void webworks::NowPlayingNDK::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        NowPlayingNDK *_t = static_cast<NowPlayingNDK *>(_o);
        switch (_id) {
        case 0: _t->playSignal(); break;
        case 1: _t->pauseSignal(); break;
        case 2: _t->stopSignal(); break;
        case 3: _t->nextSignal(); break;
        case 4: _t->previousSignal(); break;
        case 5: _t->errorSignal((*reinterpret_cast< const string(*)>(_a[1]))); break;
        case 6: _t->joinSlot((*reinterpret_cast< const string(*)>(_a[1]))); break;
        case 7: _t->playSlot(); break;
        case 8: _t->pauseSlot(); break;
        case 9: _t->stopSlot(); break;
        case 10: _t->nextSlot(); break;
        case 11: _t->previousSlot(); break;
        case 12: _t->errorSlot((*reinterpret_cast< const string(*)>(_a[1]))); break;
        case 13: { string _r = _t->NowPlayingRequestPlayback();
            if (_a[0]) *reinterpret_cast< string*>(_a[0]) = _r; }  break;
        case 14: _t->NowPlayingBindPlayCallback((*reinterpret_cast< const string(*)>(_a[1]))); break;
        case 15: _t->NowPlayingBindPauseCallback((*reinterpret_cast< const string(*)>(_a[1]))); break;
        case 16: _t->NowPlayingBindStopCallback((*reinterpret_cast< const string(*)>(_a[1]))); break;
        case 17: _t->NowPlayingBindNextCallback((*reinterpret_cast< const string(*)>(_a[1]))); break;
        case 18: _t->NowPlayingBindPreviousCallback((*reinterpret_cast< const string(*)>(_a[1]))); break;
        case 19: _t->NowPlayingBindErrorCallback((*reinterpret_cast< const string(*)>(_a[1]))); break;
        case 20: { string _r = _t->NowPlayingPlay((*reinterpret_cast< const string(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< string*>(_a[0]) = _r; }  break;
        case 21: { string _r = _t->NowPlayingPause();
            if (_a[0]) *reinterpret_cast< string*>(_a[0]) = _r; }  break;
        case 22: { string _r = _t->NowPlayingResume();
            if (_a[0]) *reinterpret_cast< string*>(_a[0]) = _r; }  break;
        case 23: { string _r = _t->NowPlayingStop();
            if (_a[0]) *reinterpret_cast< string*>(_a[0]) = _r; }  break;
        case 24: { string _r = _t->NowPlayingNext();
            if (_a[0]) *reinterpret_cast< string*>(_a[0]) = _r; }  break;
        case 25: { string _r = _t->NowPlayingPrevious();
            if (_a[0]) *reinterpret_cast< string*>(_a[0]) = _r; }  break;
        case 26: { string _r = _t->NowPlayingError((*reinterpret_cast< const string(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< string*>(_a[0]) = _r; }  break;
        case 27: { string _r = _t->NowPlayingGetState();
            if (_a[0]) *reinterpret_cast< string*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
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
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    }
    return _id;
}

// SIGNAL 0
void webworks::NowPlayingNDK::playSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void webworks::NowPlayingNDK::pauseSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void webworks::NowPlayingNDK::stopSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void webworks::NowPlayingNDK::nextSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void webworks::NowPlayingNDK::previousSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void webworks::NowPlayingNDK::errorSignal(const string & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
