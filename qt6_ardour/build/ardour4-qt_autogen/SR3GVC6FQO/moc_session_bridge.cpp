/****************************************************************************
** Meta object code from reading C++ file 'session_bridge.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../qml_shell/session_bridge.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'session_bridge.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13SessionBridgeE_t {};
} // unnamed namespace

template <> constexpr inline auto SessionBridge::qt_create_metaobjectdata<qt_meta_tag_ZN13SessionBridgeE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "SessionBridge",
        "sampleRateChanged",
        "",
        "bufferSizeChanged",
        "dspLoadChanged",
        "engineRunningChanged",
        "sessionNameChanged",
        "diskSpaceFreeChanged",
        "tcFormatChanged",
        "mouseModeChanged",
        "editModeChanged",
        "snapEnabledChanged",
        "gridUnitChanged",
        "punchInChanged",
        "punchOutChanged",
        "workspaceModeChanged",
        "requestAddTrackDialog",
        "routesChanged",
        "newSession",
        "name",
        "loadSession",
        "path",
        "saveSession",
        "exportMaster",
        "outputPath",
        "format",
        "triggerMenuAction",
        "category",
        "action",
        "setMouseMode",
        "mode",
        "set_mouse_mode",
        "setEditMode",
        "set_edit_mode",
        "toggleSnap",
        "toggle_snap",
        "setGridUnit",
        "unit",
        "set_grid_unit",
        "togglePunchIn",
        "toggle_punch_in",
        "togglePunchOut",
        "toggle_punch_out",
        "setWorkspaceMode",
        "set_workspace_mode",
        "onTelemetryTick",
        "routes",
        "QVariantList",
        "sampleRate",
        "bufferSize",
        "bufferLatencyMs",
        "dspLoad",
        "engineRunning",
        "sessionName",
        "diskSpaceFree",
        "tcFormat",
        "mouseMode",
        "editMode",
        "snapEnabled",
        "gridUnit",
        "punchIn",
        "punchOut",
        "workspaceMode"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'sampleRateChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'bufferSizeChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'dspLoadChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'engineRunningChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'sessionNameChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'diskSpaceFreeChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'tcFormatChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'mouseModeChanged'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'editModeChanged'
        QtMocHelpers::SignalData<void()>(10, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'snapEnabledChanged'
        QtMocHelpers::SignalData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'gridUnitChanged'
        QtMocHelpers::SignalData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'punchInChanged'
        QtMocHelpers::SignalData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'punchOutChanged'
        QtMocHelpers::SignalData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'workspaceModeChanged'
        QtMocHelpers::SignalData<void()>(15, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'requestAddTrackDialog'
        QtMocHelpers::SignalData<void()>(16, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'routesChanged'
        QtMocHelpers::SignalData<void()>(17, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'newSession'
        QtMocHelpers::SlotData<void(const QString &)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 19 },
        }}),
        // Slot 'loadSession'
        QtMocHelpers::SlotData<void(const QString &)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 21 },
        }}),
        // Slot 'saveSession'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'exportMaster'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 24 }, { QMetaType::QString, 25 },
        }}),
        // Slot 'triggerMenuAction'
        QtMocHelpers::SlotData<void(const QString &, const QString &)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 27 }, { QMetaType::QString, 28 },
        }}),
        // Slot 'setMouseMode'
        QtMocHelpers::SlotData<void(int)>(29, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 30 },
        }}),
        // Slot 'set_mouse_mode'
        QtMocHelpers::SlotData<void(int)>(31, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 30 },
        }}),
        // Slot 'setEditMode'
        QtMocHelpers::SlotData<void(int)>(32, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 30 },
        }}),
        // Slot 'set_edit_mode'
        QtMocHelpers::SlotData<void(int)>(33, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 30 },
        }}),
        // Slot 'toggleSnap'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'toggle_snap'
        QtMocHelpers::SlotData<void()>(35, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setGridUnit'
        QtMocHelpers::SlotData<void(const QString &)>(36, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 37 },
        }}),
        // Slot 'set_grid_unit'
        QtMocHelpers::SlotData<void(const QString &)>(38, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 37 },
        }}),
        // Slot 'togglePunchIn'
        QtMocHelpers::SlotData<void()>(39, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'toggle_punch_in'
        QtMocHelpers::SlotData<void()>(40, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'togglePunchOut'
        QtMocHelpers::SlotData<void()>(41, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'toggle_punch_out'
        QtMocHelpers::SlotData<void()>(42, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setWorkspaceMode'
        QtMocHelpers::SlotData<void(int)>(43, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 30 },
        }}),
        // Slot 'set_workspace_mode'
        QtMocHelpers::SlotData<void(int)>(44, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 30 },
        }}),
        // Slot 'onTelemetryTick'
        QtMocHelpers::SlotData<void()>(45, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'routes'
        QtMocHelpers::PropertyData<QVariantList>(46, 0x80000000 | 47, QMC::DefaultPropertyFlags | QMC::EnumOrFlag, 15),
        // property 'sampleRate'
        QtMocHelpers::PropertyData<double>(48, QMetaType::Double, QMC::DefaultPropertyFlags, 0),
        // property 'bufferSize'
        QtMocHelpers::PropertyData<int>(49, QMetaType::Int, QMC::DefaultPropertyFlags, 1),
        // property 'bufferLatencyMs'
        QtMocHelpers::PropertyData<double>(50, QMetaType::Double, QMC::DefaultPropertyFlags, 1),
        // property 'dspLoad'
        QtMocHelpers::PropertyData<double>(51, QMetaType::Double, QMC::DefaultPropertyFlags, 2),
        // property 'engineRunning'
        QtMocHelpers::PropertyData<bool>(52, QMetaType::Bool, QMC::DefaultPropertyFlags, 3),
        // property 'sessionName'
        QtMocHelpers::PropertyData<QString>(53, QMetaType::QString, QMC::DefaultPropertyFlags, 4),
        // property 'diskSpaceFree'
        QtMocHelpers::PropertyData<QString>(54, QMetaType::QString, QMC::DefaultPropertyFlags, 5),
        // property 'tcFormat'
        QtMocHelpers::PropertyData<QString>(55, QMetaType::QString, QMC::DefaultPropertyFlags, 6),
        // property 'mouseMode'
        QtMocHelpers::PropertyData<int>(56, QMetaType::Int, QMC::DefaultPropertyFlags, 7),
        // property 'editMode'
        QtMocHelpers::PropertyData<int>(57, QMetaType::Int, QMC::DefaultPropertyFlags, 8),
        // property 'snapEnabled'
        QtMocHelpers::PropertyData<bool>(58, QMetaType::Bool, QMC::DefaultPropertyFlags, 9),
        // property 'gridUnit'
        QtMocHelpers::PropertyData<QString>(59, QMetaType::QString, QMC::DefaultPropertyFlags, 10),
        // property 'punchIn'
        QtMocHelpers::PropertyData<bool>(60, QMetaType::Bool, QMC::DefaultPropertyFlags, 11),
        // property 'punchOut'
        QtMocHelpers::PropertyData<bool>(61, QMetaType::Bool, QMC::DefaultPropertyFlags, 12),
        // property 'workspaceMode'
        QtMocHelpers::PropertyData<int>(62, QMetaType::Int, QMC::DefaultPropertyFlags, 13),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SessionBridge, qt_meta_tag_ZN13SessionBridgeE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject SessionBridge::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13SessionBridgeE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13SessionBridgeE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13SessionBridgeE_t>.metaTypes,
    nullptr
} };

void SessionBridge::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SessionBridge *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->sampleRateChanged(); break;
        case 1: _t->bufferSizeChanged(); break;
        case 2: _t->dspLoadChanged(); break;
        case 3: _t->engineRunningChanged(); break;
        case 4: _t->sessionNameChanged(); break;
        case 5: _t->diskSpaceFreeChanged(); break;
        case 6: _t->tcFormatChanged(); break;
        case 7: _t->mouseModeChanged(); break;
        case 8: _t->editModeChanged(); break;
        case 9: _t->snapEnabledChanged(); break;
        case 10: _t->gridUnitChanged(); break;
        case 11: _t->punchInChanged(); break;
        case 12: _t->punchOutChanged(); break;
        case 13: _t->workspaceModeChanged(); break;
        case 14: _t->requestAddTrackDialog(); break;
        case 15: _t->routesChanged(); break;
        case 16: _t->newSession((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 17: _t->loadSession((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 18: _t->saveSession(); break;
        case 19: _t->exportMaster((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 20: _t->triggerMenuAction((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 21: _t->setMouseMode((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 22: _t->set_mouse_mode((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 23: _t->setEditMode((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 24: _t->set_edit_mode((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 25: _t->toggleSnap(); break;
        case 26: _t->toggle_snap(); break;
        case 27: _t->setGridUnit((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 28: _t->set_grid_unit((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 29: _t->togglePunchIn(); break;
        case 30: _t->toggle_punch_in(); break;
        case 31: _t->togglePunchOut(); break;
        case 32: _t->toggle_punch_out(); break;
        case 33: _t->setWorkspaceMode((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 34: _t->set_workspace_mode((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 35: _t->onTelemetryTick(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (SessionBridge::*)()>(_a, &SessionBridge::sampleRateChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (SessionBridge::*)()>(_a, &SessionBridge::bufferSizeChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (SessionBridge::*)()>(_a, &SessionBridge::dspLoadChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (SessionBridge::*)()>(_a, &SessionBridge::engineRunningChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (SessionBridge::*)()>(_a, &SessionBridge::sessionNameChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (SessionBridge::*)()>(_a, &SessionBridge::diskSpaceFreeChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (SessionBridge::*)()>(_a, &SessionBridge::tcFormatChanged, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (SessionBridge::*)()>(_a, &SessionBridge::mouseModeChanged, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (SessionBridge::*)()>(_a, &SessionBridge::editModeChanged, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (SessionBridge::*)()>(_a, &SessionBridge::snapEnabledChanged, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (SessionBridge::*)()>(_a, &SessionBridge::gridUnitChanged, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (SessionBridge::*)()>(_a, &SessionBridge::punchInChanged, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (SessionBridge::*)()>(_a, &SessionBridge::punchOutChanged, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (SessionBridge::*)()>(_a, &SessionBridge::workspaceModeChanged, 13))
            return;
        if (QtMocHelpers::indexOfMethod<void (SessionBridge::*)()>(_a, &SessionBridge::requestAddTrackDialog, 14))
            return;
        if (QtMocHelpers::indexOfMethod<void (SessionBridge::*)()>(_a, &SessionBridge::routesChanged, 15))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QVariantList*>(_v) = _t->routes(); break;
        case 1: *reinterpret_cast<double*>(_v) = _t->sampleRate(); break;
        case 2: *reinterpret_cast<int*>(_v) = _t->bufferSize(); break;
        case 3: *reinterpret_cast<double*>(_v) = _t->bufferLatencyMs(); break;
        case 4: *reinterpret_cast<double*>(_v) = _t->dspLoad(); break;
        case 5: *reinterpret_cast<bool*>(_v) = _t->engineRunning(); break;
        case 6: *reinterpret_cast<QString*>(_v) = _t->sessionName(); break;
        case 7: *reinterpret_cast<QString*>(_v) = _t->diskSpaceFree(); break;
        case 8: *reinterpret_cast<QString*>(_v) = _t->tcFormat(); break;
        case 9: *reinterpret_cast<int*>(_v) = _t->mouseMode(); break;
        case 10: *reinterpret_cast<int*>(_v) = _t->editMode(); break;
        case 11: *reinterpret_cast<bool*>(_v) = _t->snapEnabled(); break;
        case 12: *reinterpret_cast<QString*>(_v) = _t->gridUnit(); break;
        case 13: *reinterpret_cast<bool*>(_v) = _t->punchIn(); break;
        case 14: *reinterpret_cast<bool*>(_v) = _t->punchOut(); break;
        case 15: *reinterpret_cast<int*>(_v) = _t->workspaceMode(); break;
        default: break;
        }
    }
}

const QMetaObject *SessionBridge::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SessionBridge::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13SessionBridgeE_t>.strings))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "PBD::ScopedConnectionList"))
        return static_cast< PBD::ScopedConnectionList*>(this);
    return QObject::qt_metacast(_clname);
}

int SessionBridge::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 36)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 36;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 36)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 36;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void SessionBridge::sampleRateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SessionBridge::bufferSizeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SessionBridge::dspLoadChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void SessionBridge::engineRunningChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void SessionBridge::sessionNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void SessionBridge::diskSpaceFreeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void SessionBridge::tcFormatChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void SessionBridge::mouseModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void SessionBridge::editModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void SessionBridge::snapEnabledChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void SessionBridge::gridUnitChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}

// SIGNAL 11
void SessionBridge::punchInChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void SessionBridge::punchOutChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void SessionBridge::workspaceModeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void SessionBridge::requestAddTrackDialog()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}

// SIGNAL 15
void SessionBridge::routesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 15, nullptr);
}
QT_WARNING_POP
