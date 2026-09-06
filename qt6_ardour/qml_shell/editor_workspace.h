/*
 * ardour4-qt: Track List Model — Qt6 Port Header
 * Phase Qt6-A
 */

#pragma once
#include <QAbstractListModel>
#include <QObject>
#include <QString>
#include <QVariantList>
#include <QVector>

#pragma push_macro("emit")
#pragma push_macro("slots")
#pragma push_macro("signals")
#undef emit
#undef slots
#undef signals

#include "ardour/audioengine.h"
#include "ardour/session.h"
#include "ardour/route.h"
#include "ardour/audio_track.h"
#include "ardour/midi_track.h"
#include "ardour/phase_control.h"
#include "ardour/solo_isolate_control.h"
#include "ardour/automation_control.h"

#pragma pop_macro("signals")
#pragma pop_macro("slots")
#pragma pop_macro("emit")



struct TrackData {
    QString name;
    QString type;          /* "audio", "midi", "bus", "vca" */
    QString colorHex;
    bool    isActive        = false;
    bool    isMuted         = false;
    bool    isSoloed        = false;
    bool    isSoloIso       = false;
    bool    isRecArmed      = false;
    bool    isRecSafe       = false;
    bool    isPhaseInverted  = false;
    double  faderDb         = 0.0;
    double  trimDb          = 0.0;
    double  panPos          = 0.0;  /* -1.0 (L) → +1.0 (R) */
    double  panWidth        = 100.0; /* 0..100 % */
    bool    panBypass       = false;
    QString inputName       = QStringLiteral("system:capture_1");
    QString outputName      = QStringLiteral("master/in_1");
    QString autoMode        = QStringLiteral("Manual");
    int     vcaGroup        = 0; /* 0 = None, 1..8 */
    double  peakLeftDb      = -60.0;
    double  peakRightDb     = -60.0;
    bool    peakClipped     = false;
    bool    hasRegions      = false;
    int     heightPx        = 64;
    int     trackNumber     = 1;
};

struct MarkerData {
    QString name;
    double  positionX = 0.0;
    QString bbtStr;
};

class TrackListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int    selectedIndex    READ selectedIndex    NOTIFY selectedIndexChanged)
    Q_PROPERTY(double zoomFactor       READ zoomFactor       NOTIFY zoomFactorChanged)
    Q_PROPERTY(double pixelsPerSecond READ pixelsPerSecond  NOTIFY zoomFactorChanged)
    Q_PROPERTY(double playheadX        READ playheadX        NOTIFY playheadXChanged)
    Q_PROPERTY(int    trackCount       READ trackCount       NOTIFY trackCountChanged)

public:
    enum TrackRoles {
        NameRole = Qt::UserRole + 1,
        TypeRole,
        ColorHexRole,
        IsActiveRole,
        IsMutedRole,
        IsSoloedRole,
        IsRecArmedRole,
        FaderDbRole,
        TrimDbRole,
        PanPosRole,
        HasRegionsRole,
        HeightPxRole,
        IsSelectedRole,
        TrackNumberRole,
        InputNameRole,
        OutputNameRole,
        IsPhaseInvertedRole,
        IsSoloIsoRole,
        IsRecSafeRole,
        PanWidthRole,
        PanBypassRole,
        AutoModeRole,
        VcaGroupRole,
        PeakLeftDbRole,
        PeakRightDbRole,
        PeakClippedRole,
    };
    Q_ENUM(TrackRoles)

    explicit TrackListModel(QObject *parent = nullptr);

    int     rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool    setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

    int    selectedIndex() const { return m_selectedIndex; }
    double zoomFactor() const { return m_zoomFactor; }
    double pixelsPerSecond() const { return 50.0 * m_zoomFactor; }
    double playheadX() const { return m_playheadX; }
    int    trackCount() const { return m_tracks.size(); }

    Q_INVOKABLE QVariantList markers() const;

    /* CamelCase and snake_case API compatibility */
    Q_INVOKABLE void addTrack(const QString &name, const QString &type, const QString &colorHex = QString(), bool hasRegions = false);
    Q_INVOKABLE void add_track(const QString &name, const QString &type, const QString &colorHex = QString(), bool hasRegions = false) { addTrack(name, type, colorHex, hasRegions); }

    Q_INVOKABLE void removeTrack(int index);
    Q_INVOKABLE void remove_track(int index) { removeTrack(index); }

    Q_INVOKABLE void renameTrack(int index, const QString &name);
    Q_INVOKABLE void rename_track(int index, const QString &name) { renameTrack(index, name); }

    Q_INVOKABLE void selectTrack(int index);
    Q_INVOKABLE void select_track(int index) { selectTrack(index); }

    Q_INVOKABLE void toggleMute(int index);
    Q_INVOKABLE void toggle_mute(int index) { toggleMute(index); }

    Q_INVOKABLE void toggleSolo(int index);
    Q_INVOKABLE void toggle_solo(int index) { toggleSolo(index); }

    Q_INVOKABLE void toggleSoloIso(int index);
    Q_INVOKABLE void toggle_solo_iso(int index) { toggleSoloIso(index); }

    Q_INVOKABLE void toggleRecArm(int index);
    Q_INVOKABLE void toggle_rec_arm(int index) { toggleRecArm(index); }

    Q_INVOKABLE void toggleRecSafe(int index);
    Q_INVOKABLE void toggle_rec_safe(int index) { toggleRecSafe(index); }

    Q_INVOKABLE void togglePhaseInvert(int index);
    Q_INVOKABLE void toggle_phase_invert(int index) { togglePhaseInvert(index); }

    Q_INVOKABLE void setFader(int index, double db);
    Q_INVOKABLE void set_volume(int index, double db) { setFader(index, db); }

    Q_INVOKABLE void setTrim(int index, double db);
    Q_INVOKABLE void set_trim(int index, double db) { setTrim(index, db); }

    Q_INVOKABLE void setPan(int index, double pos);
    Q_INVOKABLE void set_pan(int index, double pos) { setPan(index, pos); }

    Q_INVOKABLE void setPanWidth(int index, double width);
    Q_INVOKABLE void set_pan_width(int index, double width) { setPanWidth(index, width); }

    Q_INVOKABLE void togglePanBypass(int index);
    Q_INVOKABLE void toggle_pan_bypass(int index) { togglePanBypass(index); }

    Q_INVOKABLE void setAutomationMode(int index, const QString &mode);
    Q_INVOKABLE void set_automation_mode(int index, const QString &mode) { setAutomationMode(index, mode); }

    Q_INVOKABLE void setVcaGroup(int index, int vcaNum);
    Q_INVOKABLE void set_vca_group(int index, int vcaNum) { setVcaGroup(index, vcaNum); }

    Q_INVOKABLE void resetPeakClip(int index);
    Q_INVOKABLE void reset_peak_clip(int index) { resetPeakClip(index); }

    Q_INVOKABLE void zoom_in() { setZoom(m_zoomFactor * 1.25); }
    Q_INVOKABLE void zoom_out() { setZoom(m_zoomFactor / 1.25); }
    Q_INVOKABLE void duplicateTrack(int index);
    Q_INVOKABLE void jumpToPosition(double x);
    Q_INVOKABLE void addMarker(const QString &name);
    Q_INVOKABLE void setZoom(double factor);
    Q_INVOKABLE void syncFromSession();

    Q_INVOKABLE QString colorForType(const QString &type) const;


signals:
    void selectedIndexChanged();
    void zoomFactorChanged();
    void playheadXChanged();
    void trackCountChanged();
    void markersChanged();
    void addTrackRequested(const QString &presetType);

private:
    bool indexValid(int index) const { return index >= 0 && index < m_tracks.size(); }

    QVector<TrackData>  m_tracks;
    QVector<MarkerData> m_markers;
    int    m_selectedIndex = -1;
    double m_zoomFactor    = 1.0;
    double m_playheadX     = 0.0;
};
