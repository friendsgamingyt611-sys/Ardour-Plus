/****************************************************************************
** Meta object code from reading C++ file 'qt_route.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../qml_shell/models/qt_route.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qt_route.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.2. It"
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
struct qt_meta_tag_ZN7QtRouteE_t {};
} // unnamed namespace

template <> constexpr inline auto QtRoute::qt_create_metaobjectdata<qt_meta_tag_ZN7QtRouteE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "QtRoute",
        "nameChanged",
        "",
        "isMutedChanged",
        "isSoloedChanged",
        "gainChanged",
        "peakMeterLevelChanged",
        "name",
        "isMuted",
        "isSoloed",
        "gain",
        "peakMeterLevel"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'nameChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'isMutedChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'isSoloedChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'gainChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'peakMeterLevelChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'name'
        QtMocHelpers::PropertyData<QString>(7, QMetaType::QString, QMC::DefaultPropertyFlags, 0),
        // property 'isMuted'
        QtMocHelpers::PropertyData<bool>(8, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 1),
        // property 'isSoloed'
        QtMocHelpers::PropertyData<bool>(9, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 2),
        // property 'gain'
        QtMocHelpers::PropertyData<float>(10, QMetaType::Float, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 3),
        // property 'peakMeterLevel'
        QtMocHelpers::PropertyData<float>(11, QMetaType::Float, QMC::DefaultPropertyFlags, 4),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<QtRoute, qt_meta_tag_ZN7QtRouteE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject QtRoute::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7QtRouteE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7QtRouteE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7QtRouteE_t>.metaTypes,
    nullptr
} };

void QtRoute::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<QtRoute *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->nameChanged(); break;
        case 1: _t->isMutedChanged(); break;
        case 2: _t->isSoloedChanged(); break;
        case 3: _t->gainChanged(); break;
        case 4: _t->peakMeterLevelChanged(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (QtRoute::*)()>(_a, &QtRoute::nameChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (QtRoute::*)()>(_a, &QtRoute::isMutedChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (QtRoute::*)()>(_a, &QtRoute::isSoloedChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (QtRoute::*)()>(_a, &QtRoute::gainChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (QtRoute::*)()>(_a, &QtRoute::peakMeterLevelChanged, 4))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->name(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->isMuted(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->isSoloed(); break;
        case 3: *reinterpret_cast<float*>(_v) = _t->gain(); break;
        case 4: *reinterpret_cast<float*>(_v) = _t->peakMeterLevel(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 1: _t->setIsMuted(*reinterpret_cast<bool*>(_v)); break;
        case 2: _t->setIsSoloed(*reinterpret_cast<bool*>(_v)); break;
        case 3: _t->setGain(*reinterpret_cast<float*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *QtRoute::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QtRoute::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7QtRouteE_t>.strings))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "PBD::ScopedConnectionList"))
        return static_cast< PBD::ScopedConnectionList*>(this);
    return QObject::qt_metacast(_clname);
}

int QtRoute::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void QtRoute::nameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QtRoute::isMutedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void QtRoute::isSoloedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void QtRoute::gainChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void QtRoute::peakMeterLevelChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
