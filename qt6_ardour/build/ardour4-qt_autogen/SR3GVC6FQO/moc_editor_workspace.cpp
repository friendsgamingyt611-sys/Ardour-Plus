/****************************************************************************
** Meta object code from reading C++ file 'editor_workspace.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../qml_shell/editor_workspace.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'editor_workspace.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN14TrackListModelE_t {};
} // unnamed namespace

template <> constexpr inline auto TrackListModel::qt_create_metaobjectdata<qt_meta_tag_ZN14TrackListModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "TrackListModel",
        "selectedIndexChanged",
        "",
        "zoomFactorChanged",
        "playheadXChanged",
        "trackCountChanged",
        "markersChanged",
        "addTrackRequested",
        "presetType",
        "markers",
        "QVariantList",
        "addTrack",
        "name",
        "type",
        "colorHex",
        "hasRegions",
        "add_track",
        "removeTrack",
        "index",
        "remove_track",
        "renameTrack",
        "rename_track",
        "selectTrack",
        "select_track",
        "toggleMute",
        "toggle_mute",
        "toggleSolo",
        "toggle_solo",
        "toggleSoloIso",
        "toggle_solo_iso",
        "toggleRecArm",
        "toggle_rec_arm",
        "toggleRecSafe",
        "toggle_rec_safe",
        "togglePhaseInvert",
        "toggle_phase_invert",
        "setFader",
        "db",
        "set_volume",
        "setTrim",
        "set_trim",
        "setPan",
        "pos",
        "set_pan",
        "setPanWidth",
        "width",
        "set_pan_width",
        "togglePanBypass",
        "toggle_pan_bypass",
        "setAutomationMode",
        "mode",
        "set_automation_mode",
        "setVcaGroup",
        "vcaNum",
        "set_vca_group",
        "resetPeakClip",
        "reset_peak_clip",
        "zoom_in",
        "zoom_out",
        "duplicateTrack",
        "jumpToPosition",
        "x",
        "addMarker",
        "setZoom",
        "factor",
        "syncFromSession",
        "colorForType",
        "selectedIndex",
        "zoomFactor",
        "pixelsPerSecond",
        "playheadX",
        "trackCount",
        "TrackRoles",
        "NameRole",
        "TypeRole",
        "ColorHexRole",
        "IsActiveRole",
        "IsMutedRole",
        "IsSoloedRole",
        "IsRecArmedRole",
        "FaderDbRole",
        "TrimDbRole",
        "PanPosRole",
        "HasRegionsRole",
        "HeightPxRole",
        "IsSelectedRole",
        "TrackNumberRole",
        "InputNameRole",
        "OutputNameRole",
        "IsPhaseInvertedRole",
        "IsSoloIsoRole",
        "IsRecSafeRole",
        "PanWidthRole",
        "PanBypassRole",
        "AutoModeRole",
        "VcaGroupRole",
        "PeakLeftDbRole",
        "PeakRightDbRole",
        "PeakClippedRole"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'selectedIndexChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'zoomFactorChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'playheadXChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'trackCountChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'markersChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'addTrackRequested'
        QtMocHelpers::SignalData<void(const QString &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 },
        }}),
        // Method 'markers'
        QtMocHelpers::MethodData<QVariantList() const>(9, 2, QMC::AccessPublic, 0x80000000 | 10),
        // Method 'addTrack'
        QtMocHelpers::MethodData<void(const QString &, const QString &, const QString &, bool)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 12 }, { QMetaType::QString, 13 }, { QMetaType::QString, 14 }, { QMetaType::Bool, 15 },
        }}),
        // Method 'addTrack'
        QtMocHelpers::MethodData<void(const QString &, const QString &, const QString &)>(11, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 12 }, { QMetaType::QString, 13 }, { QMetaType::QString, 14 },
        }}),
        // Method 'addTrack'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(11, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 12 }, { QMetaType::QString, 13 },
        }}),
        // Method 'add_track'
        QtMocHelpers::MethodData<void(const QString &, const QString &, const QString &, bool)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 12 }, { QMetaType::QString, 13 }, { QMetaType::QString, 14 }, { QMetaType::Bool, 15 },
        }}),
        // Method 'add_track'
        QtMocHelpers::MethodData<void(const QString &, const QString &, const QString &)>(16, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 12 }, { QMetaType::QString, 13 }, { QMetaType::QString, 14 },
        }}),
        // Method 'add_track'
        QtMocHelpers::MethodData<void(const QString &, const QString &)>(16, 2, QMC::AccessPublic | QMC::MethodCloned, QMetaType::Void, {{
            { QMetaType::QString, 12 }, { QMetaType::QString, 13 },
        }}),
        // Method 'removeTrack'
        QtMocHelpers::MethodData<void(int)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'remove_track'
        QtMocHelpers::MethodData<void(int)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'renameTrack'
        QtMocHelpers::MethodData<void(int, const QString &)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { QMetaType::QString, 12 },
        }}),
        // Method 'rename_track'
        QtMocHelpers::MethodData<void(int, const QString &)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { QMetaType::QString, 12 },
        }}),
        // Method 'selectTrack'
        QtMocHelpers::MethodData<void(int)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'select_track'
        QtMocHelpers::MethodData<void(int)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'toggleMute'
        QtMocHelpers::MethodData<void(int)>(24, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'toggle_mute'
        QtMocHelpers::MethodData<void(int)>(25, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'toggleSolo'
        QtMocHelpers::MethodData<void(int)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'toggle_solo'
        QtMocHelpers::MethodData<void(int)>(27, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'toggleSoloIso'
        QtMocHelpers::MethodData<void(int)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'toggle_solo_iso'
        QtMocHelpers::MethodData<void(int)>(29, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'toggleRecArm'
        QtMocHelpers::MethodData<void(int)>(30, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'toggle_rec_arm'
        QtMocHelpers::MethodData<void(int)>(31, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'toggleRecSafe'
        QtMocHelpers::MethodData<void(int)>(32, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'toggle_rec_safe'
        QtMocHelpers::MethodData<void(int)>(33, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'togglePhaseInvert'
        QtMocHelpers::MethodData<void(int)>(34, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'toggle_phase_invert'
        QtMocHelpers::MethodData<void(int)>(35, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'setFader'
        QtMocHelpers::MethodData<void(int, double)>(36, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { QMetaType::Double, 37 },
        }}),
        // Method 'set_volume'
        QtMocHelpers::MethodData<void(int, double)>(38, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { QMetaType::Double, 37 },
        }}),
        // Method 'setTrim'
        QtMocHelpers::MethodData<void(int, double)>(39, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { QMetaType::Double, 37 },
        }}),
        // Method 'set_trim'
        QtMocHelpers::MethodData<void(int, double)>(40, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { QMetaType::Double, 37 },
        }}),
        // Method 'setPan'
        QtMocHelpers::MethodData<void(int, double)>(41, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { QMetaType::Double, 42 },
        }}),
        // Method 'set_pan'
        QtMocHelpers::MethodData<void(int, double)>(43, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { QMetaType::Double, 42 },
        }}),
        // Method 'setPanWidth'
        QtMocHelpers::MethodData<void(int, double)>(44, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { QMetaType::Double, 45 },
        }}),
        // Method 'set_pan_width'
        QtMocHelpers::MethodData<void(int, double)>(46, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { QMetaType::Double, 45 },
        }}),
        // Method 'togglePanBypass'
        QtMocHelpers::MethodData<void(int)>(47, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'toggle_pan_bypass'
        QtMocHelpers::MethodData<void(int)>(48, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'setAutomationMode'
        QtMocHelpers::MethodData<void(int, const QString &)>(49, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { QMetaType::QString, 50 },
        }}),
        // Method 'set_automation_mode'
        QtMocHelpers::MethodData<void(int, const QString &)>(51, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { QMetaType::QString, 50 },
        }}),
        // Method 'setVcaGroup'
        QtMocHelpers::MethodData<void(int, int)>(52, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { QMetaType::Int, 53 },
        }}),
        // Method 'set_vca_group'
        QtMocHelpers::MethodData<void(int, int)>(54, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 }, { QMetaType::Int, 53 },
        }}),
        // Method 'resetPeakClip'
        QtMocHelpers::MethodData<void(int)>(55, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'reset_peak_clip'
        QtMocHelpers::MethodData<void(int)>(56, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'zoom_in'
        QtMocHelpers::MethodData<void()>(57, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'zoom_out'
        QtMocHelpers::MethodData<void()>(58, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'duplicateTrack'
        QtMocHelpers::MethodData<void(int)>(59, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 18 },
        }}),
        // Method 'jumpToPosition'
        QtMocHelpers::MethodData<void(double)>(60, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 61 },
        }}),
        // Method 'addMarker'
        QtMocHelpers::MethodData<void(const QString &)>(62, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 12 },
        }}),
        // Method 'setZoom'
        QtMocHelpers::MethodData<void(double)>(63, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 64 },
        }}),
        // Method 'syncFromSession'
        QtMocHelpers::MethodData<void()>(65, 2, QMC::AccessPublic, QMetaType::Void),
        // Method 'colorForType'
        QtMocHelpers::MethodData<QString(const QString &) const>(66, 2, QMC::AccessPublic, QMetaType::QString, {{
            { QMetaType::QString, 13 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'selectedIndex'
        QtMocHelpers::PropertyData<int>(67, QMetaType::Int, QMC::DefaultPropertyFlags, 0),
        // property 'zoomFactor'
        QtMocHelpers::PropertyData<double>(68, QMetaType::Double, QMC::DefaultPropertyFlags, 1),
        // property 'pixelsPerSecond'
        QtMocHelpers::PropertyData<double>(69, QMetaType::Double, QMC::DefaultPropertyFlags, 1),
        // property 'playheadX'
        QtMocHelpers::PropertyData<double>(70, QMetaType::Double, QMC::DefaultPropertyFlags, 2),
        // property 'trackCount'
        QtMocHelpers::PropertyData<int>(71, QMetaType::Int, QMC::DefaultPropertyFlags, 3),
    };
    QtMocHelpers::UintData qt_enums {
        // enum 'TrackRoles'
        QtMocHelpers::EnumData<enum TrackRoles>(72, 72, QMC::EnumFlags{}).add({
            {   73, TrackRoles::NameRole },
            {   74, TrackRoles::TypeRole },
            {   75, TrackRoles::ColorHexRole },
            {   76, TrackRoles::IsActiveRole },
            {   77, TrackRoles::IsMutedRole },
            {   78, TrackRoles::IsSoloedRole },
            {   79, TrackRoles::IsRecArmedRole },
            {   80, TrackRoles::FaderDbRole },
            {   81, TrackRoles::TrimDbRole },
            {   82, TrackRoles::PanPosRole },
            {   83, TrackRoles::HasRegionsRole },
            {   84, TrackRoles::HeightPxRole },
            {   85, TrackRoles::IsSelectedRole },
            {   86, TrackRoles::TrackNumberRole },
            {   87, TrackRoles::InputNameRole },
            {   88, TrackRoles::OutputNameRole },
            {   89, TrackRoles::IsPhaseInvertedRole },
            {   90, TrackRoles::IsSoloIsoRole },
            {   91, TrackRoles::IsRecSafeRole },
            {   92, TrackRoles::PanWidthRole },
            {   93, TrackRoles::PanBypassRole },
            {   94, TrackRoles::AutoModeRole },
            {   95, TrackRoles::VcaGroupRole },
            {   96, TrackRoles::PeakLeftDbRole },
            {   97, TrackRoles::PeakRightDbRole },
            {   98, TrackRoles::PeakClippedRole },
        }),
    };
    return QtMocHelpers::metaObjectData<TrackListModel, qt_meta_tag_ZN14TrackListModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject TrackListModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QAbstractListModel::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14TrackListModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14TrackListModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14TrackListModelE_t>.metaTypes,
    nullptr
} };

void TrackListModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<TrackListModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->selectedIndexChanged(); break;
        case 1: _t->zoomFactorChanged(); break;
        case 2: _t->playheadXChanged(); break;
        case 3: _t->trackCountChanged(); break;
        case 4: _t->markersChanged(); break;
        case 5: _t->addTrackRequested((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: { QVariantList _r = _t->markers();
            if (_a[0]) *reinterpret_cast<QVariantList*>(_a[0]) = std::move(_r); }  break;
        case 7: _t->addTrack((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[4]))); break;
        case 8: _t->addTrack((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 9: _t->addTrack((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 10: _t->add_track((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[4]))); break;
        case 11: _t->add_track((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 12: _t->add_track((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 13: _t->removeTrack((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 14: _t->remove_track((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->renameTrack((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 16: _t->rename_track((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 17: _t->selectTrack((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 18: _t->select_track((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 19: _t->toggleMute((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 20: _t->toggle_mute((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 21: _t->toggleSolo((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 22: _t->toggle_solo((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 23: _t->toggleSoloIso((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 24: _t->toggle_solo_iso((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 25: _t->toggleRecArm((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 26: _t->toggle_rec_arm((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 27: _t->toggleRecSafe((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 28: _t->toggle_rec_safe((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 29: _t->togglePhaseInvert((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 30: _t->toggle_phase_invert((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 31: _t->setFader((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2]))); break;
        case 32: _t->set_volume((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2]))); break;
        case 33: _t->setTrim((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2]))); break;
        case 34: _t->set_trim((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2]))); break;
        case 35: _t->setPan((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2]))); break;
        case 36: _t->set_pan((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2]))); break;
        case 37: _t->setPanWidth((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2]))); break;
        case 38: _t->set_pan_width((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2]))); break;
        case 39: _t->togglePanBypass((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 40: _t->toggle_pan_bypass((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 41: _t->setAutomationMode((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 42: _t->set_automation_mode((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 43: _t->setVcaGroup((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 44: _t->set_vca_group((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 45: _t->resetPeakClip((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 46: _t->reset_peak_clip((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 47: _t->zoom_in(); break;
        case 48: _t->zoom_out(); break;
        case 49: _t->duplicateTrack((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 50: _t->jumpToPosition((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 51: _t->addMarker((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 52: _t->setZoom((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 53: _t->syncFromSession(); break;
        case 54: { QString _r = _t->colorForType((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast<QString*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (TrackListModel::*)()>(_a, &TrackListModel::selectedIndexChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (TrackListModel::*)()>(_a, &TrackListModel::zoomFactorChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (TrackListModel::*)()>(_a, &TrackListModel::playheadXChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (TrackListModel::*)()>(_a, &TrackListModel::trackCountChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (TrackListModel::*)()>(_a, &TrackListModel::markersChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (TrackListModel::*)(const QString & )>(_a, &TrackListModel::addTrackRequested, 5))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<int*>(_v) = _t->selectedIndex(); break;
        case 1: *reinterpret_cast<double*>(_v) = _t->zoomFactor(); break;
        case 2: *reinterpret_cast<double*>(_v) = _t->pixelsPerSecond(); break;
        case 3: *reinterpret_cast<double*>(_v) = _t->playheadX(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->trackCount(); break;
        default: break;
        }
    }
}

const QMetaObject *TrackListModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TrackListModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14TrackListModelE_t>.strings))
        return static_cast<void*>(this);
    return QAbstractListModel::qt_metacast(_clname);
}

int TrackListModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractListModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 55)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 55;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 55)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 55;
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
void TrackListModel::selectedIndexChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void TrackListModel::zoomFactorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void TrackListModel::playheadXChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void TrackListModel::trackCountChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void TrackListModel::markersChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void TrackListModel::addTrackRequested(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}
QT_WARNING_POP
