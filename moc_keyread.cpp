/****************************************************************************
** Meta object code from reading C++ file 'keyread.h'
**
** Created: Fri Apr 14 11:20:28 2017
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "keyread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'keyread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_KeyRead[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,
      25,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      38,    8,    8,    8, 0x0a,
      45,    8,    8,    8, 0x0a,
      55,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_KeyRead[] = {
    "KeyRead\0\0sendLogin(bool)\0sendKey(int)\0"
    "init()\0readKey()\0slot_read_com()\0"
};

void KeyRead::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        KeyRead *_t = static_cast<KeyRead *>(_o);
        switch (_id) {
        case 0: _t->sendLogin((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->sendKey((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->init(); break;
        case 3: _t->readKey(); break;
        case 4: _t->slot_read_com(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData KeyRead::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject KeyRead::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_KeyRead,
      qt_meta_data_KeyRead, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &KeyRead::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *KeyRead::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *KeyRead::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KeyRead))
        return static_cast<void*>(const_cast< KeyRead*>(this));
    return QObject::qt_metacast(_clname);
}

int KeyRead::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void KeyRead::sendLogin(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void KeyRead::sendKey(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
