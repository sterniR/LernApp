/****************************************************************************
** Meta object code from reading C++ file 'networtbackend.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../networtbackend.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networtbackend.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN14NetwortBackendE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN14NetwortBackendE = QtMocHelpers::stringData(
    "NetwortBackend",
    "QML.Element",
    "auto",
    "dataFileFromFtpServerChanged",
    "",
    "getFTPFileList",
    "ftpUrl",
    "username",
    "password",
    "getDirFtp",
    "size_t",
    "contents",
    "size",
    "nmemb",
    "userp",
    "parseFTPList",
    "response",
    "refreshServer",
    "ThemeDatabaseSelected",
    "db",
    "dataFileFromFtpServer"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN14NetwortBackendE[] = {

 // content:
      12,       // revision
       0,       // classname
       1,   14, // classinfo
       6,   16, // methods
       1,   76, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // classinfo: key, value
       1,    2,

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       3,    0,   52,    4, 0x06,    2 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    3,   53,    4, 0x0a,    3 /* Public */,
       9,    4,   60,    4, 0x0a,    7 /* Public */,
      15,    1,   69,    4, 0x0a,   12 /* Public */,
      17,    0,   72,    4, 0x0a,   14 /* Public */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      18,    1,   73,    4, 0x02,   15 /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::QStringList, QMetaType::QString, QMetaType::QString, QMetaType::QString,    6,    7,    8,
    0x80000000 | 10, QMetaType::VoidStar, 0x80000000 | 10, 0x80000000 | 10, QMetaType::VoidStar,   11,   12,   13,   14,
    QMetaType::QStringList, QMetaType::QString,   16,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void, QMetaType::QString,   19,

 // properties: name, type, flags, notifyId, revision
      20, QMetaType::QStringList, 0x00015903, uint(0), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject NetwortBackend::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ZN14NetwortBackendE.offsetsAndSizes,
    qt_meta_data_ZN14NetwortBackendE,
    qt_static_metacall,
    nullptr,
    qt_metaTypeArray<
        // property 'dataFileFromFtpServer'
        QStringList,
        // Q_OBJECT / Q_GADGET
        NetwortBackend,
        // method 'dataFileFromFtpServerChanged'
        void,
        // method 'getFTPFileList'
        QStringList,
        const QString &,
        const QString &,
        const QString &,
        // method 'getDirFtp'
        size_t,
        void *,
        size_t,
        size_t,
        void *,
        // method 'parseFTPList'
        QStringList,
        const QString &,
        // method 'refreshServer'
        void,
        // method 'ThemeDatabaseSelected'
        void,
        const QString &
    >,
    nullptr
} };

void NetwortBackend::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<NetwortBackend *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->dataFileFromFtpServerChanged(); break;
        case 1: { QStringList _r = _t->getFTPFileList((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[3])));
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 2: { size_t _r = _t->getDirFtp((*reinterpret_cast< std::add_pointer_t<void*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<size_t>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<size_t>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<void*>>(_a[4])));
            if (_a[0]) *reinterpret_cast< size_t*>(_a[0]) = std::move(_r); }  break;
        case 3: { QStringList _r = _t->parseFTPList((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 4: _t->refreshServer(); break;
        case 5: _t->ThemeDatabaseSelected((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (NetwortBackend::*)();
            if (_q_method_type _q_method = &NetwortBackend::dataFileFromFtpServerChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QStringList*>(_v) = _t->dataFileFromFtpServer(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setDataFileFromFtpServer(*reinterpret_cast< QStringList*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *NetwortBackend::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetwortBackend::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN14NetwortBackendE.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NetwortBackend::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void NetwortBackend::dataFileFromFtpServerChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
