/****************************************************************************
** Meta object code from reading C++ file 'ckernel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.11)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ckernel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ckernel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.11. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CKernel_t {
    QByteArrayData data[19];
    char stringdata0[204];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CKernel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CKernel_t qt_meta_stringdata_CKernel = {
    {
QT_MOC_LITERAL(0, 0, 7), // "CKernel"
QT_MOC_LITERAL(1, 8, 17), // "slot_transmitData"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 5), // "char*"
QT_MOC_LITERAL(4, 33, 4), // "data"
QT_MOC_LITERAL(5, 38, 3), // "len"
QT_MOC_LITERAL(6, 42, 4), // "from"
QT_MOC_LITERAL(7, 47, 20), // "slot_registerMassage"
QT_MOC_LITERAL(8, 68, 4), // "name"
QT_MOC_LITERAL(9, 73, 3), // "tel"
QT_MOC_LITERAL(10, 77, 8), // "password"
QT_MOC_LITERAL(11, 86, 17), // "slot_loginMassage"
QT_MOC_LITERAL(12, 104, 17), // "slot_closeProcess"
QT_MOC_LITERAL(13, 122, 12), // "slot_offlien"
QT_MOC_LITERAL(14, 135, 19), // "slot_showChatDialog"
QT_MOC_LITERAL(15, 155, 8), // "friendId"
QT_MOC_LITERAL(16, 164, 16), // "slot_sendMessage"
QT_MOC_LITERAL(17, 181, 7), // "content"
QT_MOC_LITERAL(18, 189, 14) // "slot_addFriend"

    },
    "CKernel\0slot_transmitData\0\0char*\0data\0"
    "len\0from\0slot_registerMassage\0name\0"
    "tel\0password\0slot_loginMassage\0"
    "slot_closeProcess\0slot_offlien\0"
    "slot_showChatDialog\0friendId\0"
    "slot_sendMessage\0content\0slot_addFriend"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CKernel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,   54,    2, 0x0a /* Public */,
       7,    3,   61,    2, 0x0a /* Public */,
      11,    2,   68,    2, 0x0a /* Public */,
      12,    0,   73,    2, 0x0a /* Public */,
      13,    0,   74,    2, 0x0a /* Public */,
      14,    1,   75,    2, 0x0a /* Public */,
      16,    2,   78,    2, 0x0a /* Public */,
      18,    0,   83,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int, QMetaType::ULong,    4,    5,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    8,    9,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    9,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   15,   17,
    QMetaType::Void,

       0        // eod
};

void CKernel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CKernel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slot_transmitData((*reinterpret_cast< char*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< ulong(*)>(_a[3]))); break;
        case 1: _t->slot_registerMassage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 2: _t->slot_loginMassage((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->slot_closeProcess(); break;
        case 4: _t->slot_offlien(); break;
        case 5: _t->slot_showChatDialog((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->slot_sendMessage((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 7: _t->slot_addFriend(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CKernel::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_CKernel.data,
    qt_meta_data_CKernel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CKernel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CKernel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CKernel.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CKernel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
