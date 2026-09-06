/****************************************************************************
** Meta object code from reading C++ file 'transport_bar.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../qml_shell/transport_bar.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'transport_bar.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN12TransportBarE_t {};
} // unnamed namespace

template <> constexpr inline auto TransportBar::qt_create_metaobjectdata<qt_meta_tag_ZN12TransportBarE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "TransportBar",
        "isPlayingChanged",
        "",
        "isRecordingChanged",
        "isLoopingChanged",
        "punchEnabledChanged",
        "autoReturnEnabledChanged",
        "followRangeEnabledChanged",
        "clickEnabledChanged",
        "shuttleSpeedChanged",
        "playheadPositionChanged",
        "tempoBpmChanged",
        "bbtStringChanged",
        "timecodeStringChanged",
        "meterStringChanged",
        "play",
        "stop",
        "rewind",
        "fast_forward",
        "toggle_rec",
        "toggle_loop",
        "toggle_punch",
        "toggle_auto_return",
        "toggle_follow_range",
        "toggle_click",
        "onClockTick",
        "isPlaying",
        "isRecording",
        "isLooping",
        "punchEnabled",
        "autoReturnEnabled",
        "followRangeEnabled",
        "clickEnabled",
        "shuttleSpeed",
        "playheadPosition",
        "tempoBpm",
        "bbtString",
        "timecodeString",
        "meterString"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'isPlayingChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'isRecordingChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'isLoopingChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'punchEnabledChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'autoReturnEnabledChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'followRangeEnabledChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'clickEnabledChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'shuttleSpeedChanged'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'playheadPositionChanged'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'tempoBpmChanged'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'bbtStringChanged'
        QtMocHelpers::SignalData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'timecodeStringChanged'
        QtMocHelpers::SignalData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'meterStringChanged'
        QtMocHelpers::SignalData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'play'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'stop'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'rewind'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'fast_forward'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'toggle_rec'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'toggle_loop'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'toggle_punch'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'toggle_auto_return'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'toggle_follow_range'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'toggle_click'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onClockTick'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'isPlaying'
        QtMocHelpers::PropertyData<bool>(26, QMetaType::Bool, QMC::DefaultPropertyFlags, 0),
        // property 'isRecording'
        QtMocHelpers::PropertyData<bool>(27, QMetaType::Bool, QMC::DefaultPropertyFlags, 1),
        // property 'isLooping'
        QtMocHelpers::PropertyData<bool>(28, QMetaType::Bool, QMC::DefaultPropertyFlags, 2),
        // property 'punchEnabled'
        QtMocHelpers::PropertyData<bool>(29, QMetaType::Bool, QMC::DefaultPropertyFlags, 3),
        // property 'autoReturnEnabled'
        QtMocHelpers::PropertyData<bool>(30, QMetaType::Bool, QMC::DefaultPropertyFlags, 4),
        // property 'followRangeEnabled'
        QtMocHelpers::PropertyData<bool>(31, QMetaType::Bool, QMC::DefaultPropertyFlags, 5),
        // property 'clickEnabled'
        QtMocHelpers::PropertyData<bool>(32, QMetaType::Bool, QMC::DefaultPropertyFlags, 6),
        // property 'shuttleSpeed'
        QtMocHelpers::PropertyData<double>(33, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 7),
        // property 'playheadPosition'
        QtMocHelpers::PropertyData<double>(34, QMetaType::Double, QMC::DefaultPropertyFlags, 8),
        // property 'tempoBpm'
        QtMocHelpers::PropertyData<double>(35, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet, 9),
        // property 'bbtString'
        QtMocHelpers::PropertyData<QString>(36, QMetaType::QString, QMC::DefaultPropertyFlags, 10),
        // property 'timecodeString'
        QtMocHelpers::PropertyData<QString>(37, QMetaType::QString, QMC::DefaultPropertyFlags, 11),
        // property 'meterString'
        QtMocHelpers::PropertyData<QString>(38, QMetaType::QString, QMC::DefaultPropertyFlags, 12),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<TransportBar, qt_meta_tag_ZN12TransportBarE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject TransportBar::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12TransportBarE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12TransportBarE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12TransportBarE_t>.metaTypes,
    nullptr
} };

void TransportBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TransportBar *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->isPlayingChanged(); break;
        case 1: _t->isRecordingChanged(); break;
        case 2: _t->isLoopingChanged(); break;
        case 3: _t->punchEnabledChanged(); break;
        case 4: _t->autoReturnEnabledChanged(); break;
        case 5: _t->followRangeEnabledChanged(); break;
        case 6: _t->clickEnabledChanged(); break;
        case 7: _t->shuttleSpeedChanged(); break;
        case 8: _t->playheadPositionChanged(); break;
        case 9: _t->tempoBpmChanged(); break;
        case 10: _t->bbtStringChanged(); break;
        case 11: _t->timecodeStringChanged(); break;
        case 12: _t->meterStringChanged(); break;
        case 13: _t->play(); break;
        case 14: _t->stop(); break;
        case 15: _t->rewind(); break;
        case 16: _t->fast_forward(); break;
        case 17: _t->toggle_rec(); break;
        case 18: _t->toggle_loop(); break;
        case 19: _t->toggle_punch(); break;
        case 20: _t->toggle_auto_return(); break;
        case 21: _t->toggle_follow_range(); break;
        case 22: _t->toggle_click(); break;
        case 23: _t->onClockTick(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (TransportBar::*)()>(_a, &TransportBar::isPlayingChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (TransportBar::*)()>(_a, &TransportBar::isRecordingChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (TransportBar::*)()>(_a, &TransportBar::isLoopingChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (TransportBar::*)()>(_a, &TransportBar::punchEnabledChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (TransportBar::*)()>(_a, &TransportBar::autoReturnEnabledChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (TransportBar::*)()>(_a, &TransportBar::followRangeEnabledChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (TransportBar::*)()>(_a, &TransportBar::clickEnabledChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (TransportBar::*)()>(_a, &TransportBar::shuttleSpeedChanged, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (TransportBar::*)()>(_a, &TransportBar::playheadPositionChanged, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (TransportBar::*)()>(_a, &TransportBar::tempoBpmChanged, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (TransportBar::*)()>(_a, &TransportBar::bbtStringChanged, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (TransportBar::*)()>(_a, &TransportBar::timecodeStringChanged, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (TransportBar::*)()>(_a, &TransportBar::meterStringChanged, 12))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<bool*>(_v) = _t->isPlaying(); break;
        case 1: *reinterpret_cast<bool*>(_v) = _t->isRecording(); break;
        case 2: *reinterpret_cast<bool*>(_v) = _t->isLooping(); break;
        case 3: *reinterpret_cast<bool*>(_v) = _t->punchEnabled(); break;
        case 4: *reinterpret_cast<bool*>(_v) = _t->autoReturnEnabled(); break;
        case 5: *reinterpret_cast<bool*>(_v) = _t->followRangeEnabled(); break;
        case 6: *reinterpret_cast<bool*>(_v) = _t->clickEnabled(); break;
        case 7: *reinterpret_cast<double*>(_v) = _t->shuttleSpeed(); break;
        case 8: *reinterpret_cast<double*>(_v) = _t->playheadPosition(); break;
        case 9: *reinterpret_cast<double*>(_v) = _t->tempoBpm(); break;
        case 10: *reinterpret_cast<QString*>(_v) = _t->bbtString(); break;
        case 11: *reinterpret_cast<QString*>(_v) = _t->timecodeString(); break;
        case 12: *reinterpret_cast<QString*>(_v) = _t->meterString(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 7: _t->setShuttleSpeed(*reinterpret_cast<double*>(_v)); break;
        case 9: _t->setTempoBpm(*reinterpret_cast<double*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *TransportBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TransportBar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12TransportBarE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TransportBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 24)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 24;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void TransportBar::isPlayingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TransportBar::isRecordingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TransportBar::isLoopingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TransportBar::punchEnabledChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void TransportBar::autoReturnEnabledChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void TransportBar::followRangeEnabledChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void TransportBar::clickEnabledChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void TransportBar::shuttleSpeedChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void TransportBar::playheadPositionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void TransportBar::tempoBpmChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void TransportBar::bbtStringChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void TransportBar::timecodeStringChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void TransportBar::meterStringChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}
QT_WARNING_POP
