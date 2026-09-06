/*
 * ardour4-qt: Track List Model — Qt6 Implementation
 * Ported from adw_shell/editor_workspace.cc (Phase 3.5)
 */

#include "editor_workspace.h"
#include <QVariantMap>
#include <cmath>

/* ── Default track colors — from Phase 3.5 editor_workspace.cc ── */

QString TrackListModel::colorForType(const QString &type) const
{
    if (type == QStringLiteral("midi")) return QStringLiteral("#4a6db5");
    if (type == QStringLiteral("bus"))  return QStringLiteral("#5a7880");
    if (type == QStringLiteral("vca"))  return QStringLiteral("#8a6030");
    return QStringLiteral("#3a8a55"); /* audio default — Ardour green */
}

/* ── Constructor ── */

TrackListModel::TrackListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    /* Starts empty (0 tracks) on blank session per Ardour standard opening flow */
    m_selectedIndex = -1;
    syncFromSession();
}

/* ── QAbstractListModel interface ── */

int TrackListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_tracks.size();
}

QHash<int, QByteArray> TrackListModel::roleNames() const
{
    return {
        { NameRole,            "name"              },
        { TypeRole,            "type"              },
        { ColorHexRole,        "color_hex"         },
        { IsActiveRole,        "is_active"         },
        { IsMutedRole,         "is_muted"          },
        { IsSoloedRole,        "is_soloed"         },
        { IsRecArmedRole,      "is_rec_armed"      },
        { FaderDbRole,         "volume_db"         },
        { TrimDbRole,          "trim_db"           },
        { PanPosRole,          "pan_pos"           },
        { HasRegionsRole,      "has_regions"       },
        { HeightPxRole,        "track_height"      },
        { IsSelectedRole,      "is_selected"       },
        { TrackNumberRole,     "track_number"      },
        { InputNameRole,       "input_name"        },
        { OutputNameRole,      "output_name"       },
        { IsPhaseInvertedRole, "is_phase_inverted" },
        { IsSoloIsoRole,       "is_solo_iso"       },
        { IsRecSafeRole,       "is_rec_safe"       },
        { PanWidthRole,        "pan_width"         },
        { PanBypassRole,       "pan_bypass"        },
        { AutoModeRole,        "auto_mode"         },
        { VcaGroupRole,        "vca_group"         },
        { PeakLeftDbRole,      "peak_left_db"      },
        { PeakRightDbRole,     "peak_right_db"     },
        { PeakClippedRole,     "peak_clipped"      },
    };
}

QVariant TrackListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_tracks.size()) return {};
    const auto &tr = m_tracks.at(index.row());

    switch (role) {
    case NameRole:            return tr.name;
    case TypeRole:            return tr.type;
    case ColorHexRole:        return tr.colorHex;
    case IsActiveRole:        return tr.isActive;
    case IsMutedRole:         return tr.isMuted;
    case IsSoloedRole:        return tr.isSoloed;
    case IsRecArmedRole:      return tr.isRecArmed;
    case FaderDbRole:         return tr.faderDb;
    case TrimDbRole:          return tr.trimDb;
    case PanPosRole:          return tr.panPos;
    case HasRegionsRole:      return tr.hasRegions;
    case HeightPxRole:        return tr.heightPx;
    case IsSelectedRole:      return (index.row() == m_selectedIndex);
    case TrackNumberRole:     return tr.trackNumber;
    case InputNameRole:       return tr.inputName;
    case OutputNameRole:      return tr.outputName;
    case IsPhaseInvertedRole: return tr.isPhaseInverted;
    case IsSoloIsoRole:       return tr.isSoloIso;
    case IsRecSafeRole:       return tr.isRecSafe;
    case PanWidthRole:        return tr.panWidth;
    case PanBypassRole:       return tr.panBypass;
    case AutoModeRole:        return tr.autoMode;
    case VcaGroupRole:        return tr.vcaGroup;
    case PeakLeftDbRole:      return tr.peakLeftDb;
    case PeakRightDbRole:     return tr.peakRightDb;
    case PeakClippedRole:     return tr.peakClipped;
    default:                  return {};
    }
}

bool TrackListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= m_tracks.size()) return false;
    auto &tr = m_tracks[index.row()];
    switch (role) {
    case NameRole:            tr.name            = value.toString();  break;
    case IsMutedRole:         tr.isMuted         = value.toBool();    break;
    case IsSoloedRole:        tr.isSoloed        = value.toBool();    break;
    case IsRecArmedRole:      tr.isRecArmed      = value.toBool();    break;
    case FaderDbRole:         tr.faderDb         = value.toDouble();  break;
    case TrimDbRole:          tr.trimDb          = value.toDouble();  break;
    case PanPosRole:          tr.panPos          = value.toDouble();  break;
    case IsActiveRole:        tr.isActive        = value.toBool();    break;
    case IsPhaseInvertedRole: tr.isPhaseInverted = value.toBool();    break;
    case IsSoloIsoRole:       tr.isSoloIso       = value.toBool();    break;
    case IsRecSafeRole:       tr.isRecSafe       = value.toBool();    break;
    case PanWidthRole:        tr.panWidth        = value.toDouble();  break;
    case PanBypassRole:       tr.panBypass       = value.toBool();    break;
    case AutoModeRole:        tr.autoMode        = value.toString();  break;
    case VcaGroupRole:        tr.vcaGroup        = value.toInt();     break;
    default: return false;
    }
    emit dataChanged(index, index, {role});
    return true;
}

/* ── Sync with live ARDOUR::Session ── */

void TrackListModel::syncFromSession()
{
    ARDOUR::Session *s = ARDOUR::AudioEngine::instance() ? ARDOUR::AudioEngine::instance()->session() : nullptr;
    if (!s) return;

    beginResetModel();
    m_tracks.clear();

    auto routes = s->get_routes();
    if (routes) {
        int trackNo = 1;
        for (const auto &r : *routes) {
            if (!r || r->is_master() || r->is_monitor()) continue;

            TrackData tr;
            tr.trackNumber = trackNo++;
            tr.name = QString::fromStdString(r->name());

            bool isAudio = (std::dynamic_pointer_cast<ARDOUR::AudioTrack>(r) != nullptr);
            bool isMidi  = (std::dynamic_pointer_cast<ARDOUR::MidiTrack>(r)  != nullptr);
            bool isTrack = r->is_track();

            tr.type = isMidi  ? QStringLiteral("midi")
                    : isAudio ? QStringLiteral("audio")
                    : isTrack ? QStringLiteral("audio") : QStringLiteral("bus");
            tr.colorHex = colorForType(tr.type);
            tr.isMuted = r->muted();
            tr.isSoloed = r->soloed();
            tr.isSoloIso = r->solo_isolate_control() ? (r->solo_isolate_control()->get_value() > 0.0) : false;

            std::shared_ptr<ARDOUR::Track> t = std::dynamic_pointer_cast<ARDOUR::Track>(r);
            tr.isRecArmed = (t && t->rec_enable_control()) ? (t->rec_enable_control()->get_value() > 0.0) : false;
            tr.isRecSafe  = (t && t->rec_safe_control())   ? (t->rec_safe_control()->get_value() > 0.0)   : false;

            tr.faderDb = r->gain_control() ? r->gain_control()->get_value() : 0.0;
            tr.trimDb  = r->trim_control() ? r->trim_control()->get_value() : 0.0;
            tr.isPhaseInverted = r->phase_control() ? (r->phase_control()->get_value() > 0.0) : false;
            tr.heightPx = isMidi ? 80 : 64;

            m_tracks.append(tr);
        }
    }
    endResetModel();
    if (!m_tracks.isEmpty() && m_selectedIndex < 0) {
        m_selectedIndex = 0;
        emit selectedIndexChanged();
    }
    emit trackCountChanged();
}


/* ── Marker access for QML ── */

QVariantList TrackListModel::markers() const
{
    QVariantList result;
    for (const auto &m : m_markers) {
        QVariantMap entry;
        entry[QStringLiteral("name")]      = m.name;
        entry[QStringLiteral("positionX")] = m.positionX;
        entry[QStringLiteral("bbtStr")]    = m.bbtStr;
        result.append(entry);
    }
    return result;
}

/* ── Track operations ── */

void TrackListModel::addTrack(const QString &name, const QString &type,
                               const QString &colorHex, bool hasRegions)
{
    ARDOUR::Session *s = ARDOUR::AudioEngine::instance() ? ARDOUR::AudioEngine::instance()->session() : nullptr;
    if (s) {
        std::string n = name.toStdString();
        if (type == QStringLiteral("midi")) {
            s->new_midi_track(ARDOUR::ChanCount(ARDOUR::DataType::MIDI, 1),
                              ARDOUR::ChanCount(ARDOUR::DataType::MIDI, 1),
                              false, nullptr, nullptr, nullptr, 1, n,
                              ARDOUR::PresentationInfo::order_t(), ARDOUR::TrackMode::Normal, true);
        } else if (type == QStringLiteral("bus")) {
            s->new_audio_route(1, 2, nullptr, 1, n, ARDOUR::PresentationInfo::Flag(0), ARDOUR::PresentationInfo::order_t());
        } else {
            s->new_audio_track(1, 2, nullptr, 1, n, ARDOUR::PresentationInfo::order_t());
        }
        syncFromSession();
        return;
    }

    static const QString palette[] = {
        QStringLiteral("#3a8a55"), QStringLiteral("#4a6db5"), QStringLiteral("#10b981"),
        QStringLiteral("#8b5cf6"), QStringLiteral("#0288d1"), QStringLiteral("#f59e0b"),
        QStringLiteral("#ef4444"), QStringLiteral("#ec4899")
    };
    static int colorIdx = 0;

    TrackData tr;
    tr.name       = name.isEmpty() ? (type == QStringLiteral("midi") ? QStringLiteral("MIDI") : QStringLiteral("Audio")) : name;
    tr.type       = type.isEmpty() ? QStringLiteral("audio") : type;
    tr.colorHex   = colorHex.isEmpty() ? palette[(colorIdx++) % 8] : colorHex;
    tr.hasRegions = hasRegions;
    tr.heightPx   = (tr.type == QStringLiteral("midi")) ? 80
                  : (tr.type == QStringLiteral("vca"))  ? 42 : 64;

    beginInsertRows(QModelIndex(), m_tracks.size(), m_tracks.size());
    m_tracks.append(tr);
    endInsertRows();

    if (m_selectedIndex < 0) {
        m_selectedIndex = 0;
        emit selectedIndexChanged();
    }
    emit trackCountChanged();
}

void TrackListModel::removeTrack(int index)
{
    if (!indexValid(index)) return;

    ARDOUR::Session *s = ARDOUR::AudioEngine::instance() ? ARDOUR::AudioEngine::instance()->session() : nullptr;
    if (s) {
        auto routes = s->get_routes();
        if (routes && index < static_cast<int>(routes->size())) {
            auto it = routes->begin();
            std::advance(it, index);
            if (it != routes->end() && *it) {
                s->remove_route(*it);
                syncFromSession();
                return;
            }
        }
    }

    beginRemoveRows(QModelIndex(), index, index);
    m_tracks.removeAt(index);
    endRemoveRows();

    if (m_selectedIndex >= m_tracks.size())
        m_selectedIndex = m_tracks.size() - 1;

    emit selectedIndexChanged();
    emit trackCountChanged();
}

void TrackListModel::renameTrack(int index, const QString &name)
{
    if (!indexValid(index) || name.isEmpty()) return;
    m_tracks[index].name = name;
    QModelIndex idx = createIndex(index, 0);
    emit dataChanged(idx, idx, {NameRole});
}

void TrackListModel::selectTrack(int index)
{
    if (index < 0 || index >= m_tracks.size()) return;
    for (int i = 0; i < m_tracks.size(); ++i) {
        bool was = m_tracks[i].isActive;
        m_tracks[i].isActive = (i == index);
        if (m_tracks[i].isActive != was) {
            QModelIndex mi = createIndex(i, 0);
            emit dataChanged(mi, mi, {IsActiveRole});
        }
    }
    m_selectedIndex = index;
    emit selectedIndexChanged();
}

void TrackListModel::duplicateTrack(int index)
{
    if (!indexValid(index)) return;
    TrackData copy = m_tracks.at(index);
    copy.name    = copy.name + QStringLiteral(" (copy)");
    copy.isActive = false;

    beginInsertRows(QModelIndex(), index + 1, index + 1);
    m_tracks.insert(index + 1, copy);
    endInsertRows();
    emit trackCountChanged();
}

/* ── Per-track property setters ── */

void TrackListModel::toggleMute(int index)
{
    if (!indexValid(index)) return;
    m_tracks[index].isMuted = !m_tracks[index].isMuted;

    ARDOUR::Session *s = ARDOUR::AudioEngine::instance() ? ARDOUR::AudioEngine::instance()->session() : nullptr;
    if (s) {
        auto routes = s->get_routes();
        if (routes && index < static_cast<int>(routes->size())) {
            auto it = routes->begin();
            std::advance(it, index);
            if (it != routes->end() && *it && (*it)->mute_control()) {
                (*it)->mute_control()->set_value(m_tracks[index].isMuted ? 1.0 : 0.0, PBD::Controllable::NoGroup);
            }
        }
    }

    QModelIndex mi = createIndex(index, 0);
    emit dataChanged(mi, mi, {IsMutedRole});
}

void TrackListModel::toggleSolo(int index)
{
    if (!indexValid(index)) return;
    m_tracks[index].isSoloed = !m_tracks[index].isSoloed;

    ARDOUR::Session *s = ARDOUR::AudioEngine::instance() ? ARDOUR::AudioEngine::instance()->session() : nullptr;
    if (s) {
        auto routes = s->get_routes();
        if (routes && index < static_cast<int>(routes->size())) {
            auto it = routes->begin();
            std::advance(it, index);
            if (it != routes->end() && *it && (*it)->solo_control()) {
                (*it)->solo_control()->set_value(m_tracks[index].isSoloed ? 1.0 : 0.0, PBD::Controllable::NoGroup);
            }
        }
    }

    QModelIndex mi = createIndex(index, 0);
    emit dataChanged(mi, mi, {IsSoloedRole});
}

void TrackListModel::toggleRecArm(int index)
{
    if (!indexValid(index)) return;
    m_tracks[index].isRecArmed = !m_tracks[index].isRecArmed;

    ARDOUR::Session *s = ARDOUR::AudioEngine::instance() ? ARDOUR::AudioEngine::instance()->session() : nullptr;
    if (s) {
        auto routes = s->get_routes();
        if (routes && index < static_cast<int>(routes->size())) {
            auto it = routes->begin();
            std::advance(it, index);
            if (it != routes->end() && *it) {
                std::shared_ptr<ARDOUR::Track> t = std::dynamic_pointer_cast<ARDOUR::Track>(*it);
                if (t && t->rec_enable_control()) {
                    t->rec_enable_control()->set_value(m_tracks[index].isRecArmed ? 1.0 : 0.0, PBD::Controllable::NoGroup);
                }

            }
        }
    }

    QModelIndex mi = createIndex(index, 0);
    emit dataChanged(mi, mi, {IsRecArmedRole});
}

void TrackListModel::setFader(int index, double db)
{
    if (!indexValid(index)) return;
    m_tracks[index].faderDb = db;

    ARDOUR::Session *s = ARDOUR::AudioEngine::instance() ? ARDOUR::AudioEngine::instance()->session() : nullptr;
    if (s) {
        auto routes = s->get_routes();
        if (routes && index < static_cast<int>(routes->size())) {
            auto it = routes->begin();
            std::advance(it, index);
            if (it != routes->end() && *it && (*it)->gain_control()) {
                (*it)->gain_control()->set_value(db, PBD::Controllable::NoGroup);
            }
        }
    }

    QModelIndex mi = createIndex(index, 0);
    emit dataChanged(mi, mi, {FaderDbRole});
}

void TrackListModel::setPan(int index, double pos)
{
    if (!indexValid(index)) return;
    m_tracks[index].panPos = pos;

    ARDOUR::Session *s = ARDOUR::AudioEngine::instance() ? ARDOUR::AudioEngine::instance()->session() : nullptr;
    if (s) {
        auto routes = s->get_routes();
        if (routes && index < static_cast<int>(routes->size())) {
            auto it = routes->begin();
            std::advance(it, index);
            if (it != routes->end() && *it && (*it)->pan_azimuth_control()) {
                (*it)->pan_azimuth_control()->set_value(pos, PBD::Controllable::NoGroup);
            }
        }
    }

    QModelIndex mi = createIndex(index, 0);
    emit dataChanged(mi, mi, {PanPosRole});
}

void TrackListModel::toggleSoloIso(int index)
{
    if (!indexValid(index)) return;
    m_tracks[index].isSoloIso = !m_tracks[index].isSoloIso;

    ARDOUR::Session *s = ARDOUR::AudioEngine::instance() ? ARDOUR::AudioEngine::instance()->session() : nullptr;
    if (s) {
        auto routes = s->get_routes();
        if (routes && index < static_cast<int>(routes->size())) {
            auto it = routes->begin();
            std::advance(it, index);
            if (it != routes->end() && *it && (*it)->solo_isolate_control()) {
                (*it)->solo_isolate_control()->set_value(m_tracks[index].isSoloIso ? 1.0 : 0.0, PBD::Controllable::NoGroup);
            }
        }
    }

    QModelIndex mi = createIndex(index, 0);
    emit dataChanged(mi, mi, {IsSoloIsoRole});
}

void TrackListModel::toggleRecSafe(int index)
{
    if (!indexValid(index)) return;
    m_tracks[index].isRecSafe = !m_tracks[index].isRecSafe;

    ARDOUR::Session *s = ARDOUR::AudioEngine::instance() ? ARDOUR::AudioEngine::instance()->session() : nullptr;
    if (s) {
        auto routes = s->get_routes();
        if (routes && index < static_cast<int>(routes->size())) {
            auto it = routes->begin();
            std::advance(it, index);
            if (it != routes->end() && *it) {
                std::shared_ptr<ARDOUR::Track> t = std::dynamic_pointer_cast<ARDOUR::Track>(*it);
                if (t && t->rec_safe_control()) {
                    t->rec_safe_control()->set_value(m_tracks[index].isRecSafe ? 1.0 : 0.0, PBD::Controllable::NoGroup);
                }
            }
        }
    }

    QModelIndex mi = createIndex(index, 0);
    emit dataChanged(mi, mi, {IsRecSafeRole});
}

void TrackListModel::togglePhaseInvert(int index)
{
    if (!indexValid(index)) return;
    m_tracks[index].isPhaseInverted = !m_tracks[index].isPhaseInverted;

    ARDOUR::Session *s = ARDOUR::AudioEngine::instance() ? ARDOUR::AudioEngine::instance()->session() : nullptr;
    if (s) {
        auto routes = s->get_routes();
        if (routes && index < static_cast<int>(routes->size())) {
            auto it = routes->begin();
            std::advance(it, index);
            if (it != routes->end() && *it && (*it)->phase_control()) {
                (*it)->phase_control()->set_value(m_tracks[index].isPhaseInverted ? 1.0 : 0.0, PBD::Controllable::NoGroup);
            }
        }
    }

    QModelIndex mi = createIndex(index, 0);
    emit dataChanged(mi, mi, {IsPhaseInvertedRole});
}

void TrackListModel::setTrim(int index, double db)
{
    if (!indexValid(index)) return;
    m_tracks[index].trimDb = db;

    ARDOUR::Session *s = ARDOUR::AudioEngine::instance() ? ARDOUR::AudioEngine::instance()->session() : nullptr;
    if (s) {
        auto routes = s->get_routes();
        if (routes && index < static_cast<int>(routes->size())) {
            auto it = routes->begin();
            std::advance(it, index);
            if (it != routes->end() && *it && (*it)->trim_control()) {
                (*it)->trim_control()->set_value(db, PBD::Controllable::NoGroup);
            }
        }
    }

    QModelIndex mi = createIndex(index, 0);
    emit dataChanged(mi, mi, {TrimDbRole});
}

void TrackListModel::setPanWidth(int index, double width)
{
    if (!indexValid(index)) return;
    m_tracks[index].panWidth = width;

    QModelIndex mi = createIndex(index, 0);
    emit dataChanged(mi, mi, {PanWidthRole});
}

void TrackListModel::togglePanBypass(int index)
{
    if (!indexValid(index)) return;
    m_tracks[index].panBypass = !m_tracks[index].panBypass;

    QModelIndex mi = createIndex(index, 0);
    emit dataChanged(mi, mi, {PanBypassRole});
}

void TrackListModel::setAutomationMode(int index, const QString &mode)
{
    if (!indexValid(index)) return;
    m_tracks[index].autoMode = mode;

    QModelIndex mi = createIndex(index, 0);
    emit dataChanged(mi, mi, {AutoModeRole});
}

void TrackListModel::setVcaGroup(int index, int vcaNum)
{
    if (!indexValid(index)) return;
    m_tracks[index].vcaGroup = vcaNum;

    QModelIndex mi = createIndex(index, 0);
    emit dataChanged(mi, mi, {VcaGroupRole});
}

void TrackListModel::resetPeakClip(int index)
{
    if (!indexValid(index)) return;
    m_tracks[index].peakClipped = false;

    QModelIndex mi = createIndex(index, 0);
    emit dataChanged(mi, mi, {PeakClippedRole});
}


/* ── View state ── */

void TrackListModel::setZoom(double factor)
{
    double clamped = qBound(0.1, factor, 8.0);
    if (qFuzzyCompare(clamped, m_zoomFactor)) return;
    m_zoomFactor = clamped;
    emit zoomFactorChanged();
}

void TrackListModel::jumpToPosition(double x)
{
    m_playheadX = x;
    emit playheadXChanged();
}

void TrackListModel::addMarker(const QString &name)
{
    /* BBT string from playhead position */
    int bar = (int)(m_playheadX / 40.0) + 1;
    MarkerData m;
    m.name       = name.isEmpty() ? QStringLiteral("Marker %1").arg(m_markers.size() + 1) : name;
    m.positionX  = m_playheadX;
    m.bbtStr     = QStringLiteral("%1|01|0000").arg(bar, 3, 10, QLatin1Char('0'));
    m_markers.append(m);
    emit markersChanged();
}

