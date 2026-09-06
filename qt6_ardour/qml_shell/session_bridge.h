/*
 * ardour4-qt: Session & Engine Bridge — Qt6 Core Port
 * Phase 5.1 — Session Setup & Audio Engine Telemetry
 */

#pragma once
#include <QObject>
#include <QString>
#include <QTimer>
#include <QVariantList>

#pragma push_macro("emit")
#pragma push_macro("slots")
#pragma push_macro("signals")
#undef emit
#undef slots
#undef signals

#include "ardour/audioengine.h"
#include "ardour/session.h"

#pragma pop_macro("signals")
#pragma pop_macro("slots")
#pragma pop_macro("emit")


#include "pbd/signals.h"

class SessionBridge : public QObject, public PBD::ScopedConnectionList
{
    Q_OBJECT

    Q_PROPERTY(QVariantList routes READ routes NOTIFY routesChanged)

    Q_PROPERTY(double sampleRate      READ sampleRate      NOTIFY sampleRateChanged)
    Q_PROPERTY(int    bufferSize      READ bufferSize      NOTIFY bufferSizeChanged)
    Q_PROPERTY(double bufferLatencyMs READ bufferLatencyMs NOTIFY bufferSizeChanged)
    Q_PROPERTY(double dspLoad         READ dspLoad         NOTIFY dspLoadChanged)
    Q_PROPERTY(bool   engineRunning   READ engineRunning   NOTIFY engineRunningChanged)
    Q_PROPERTY(QString sessionName    READ sessionName     NOTIFY sessionNameChanged)
    Q_PROPERTY(QString diskSpaceFree  READ diskSpaceFree   NOTIFY diskSpaceFreeChanged)
    Q_PROPERTY(QString tcFormat       READ tcFormat        NOTIFY tcFormatChanged)

    Q_PROPERTY(int     mouseMode      READ mouseMode       NOTIFY mouseModeChanged)
    Q_PROPERTY(int     editMode       READ editMode        NOTIFY editModeChanged)
    Q_PROPERTY(bool    snapEnabled    READ snapEnabled     NOTIFY snapEnabledChanged)
    Q_PROPERTY(QString gridUnit       READ gridUnit        NOTIFY gridUnitChanged)
    Q_PROPERTY(bool    punchIn        READ punchIn         NOTIFY punchInChanged)
    Q_PROPERTY(bool    punchOut       READ punchOut        NOTIFY punchOutChanged)
    Q_PROPERTY(int     workspaceMode  READ workspaceMode   NOTIFY workspaceModeChanged)

public:
    explicit SessionBridge(QObject *parent = nullptr);
    ~SessionBridge() override = default;

    double  sampleRate()      const { return m_sampleRate;      }
    int     bufferSize()      const { return m_bufferSize;      }
    double  bufferLatencyMs() const { return (m_sampleRate > 0) ? ((double)m_bufferSize / m_sampleRate * 1000.0) : 0.0; }
    double  dspLoad()         const { return m_dspLoad;         }
    bool    engineRunning()   const { return m_engineRunning;   }
    QString sessionName()     const { return m_sessionName;     }
    QString diskSpaceFree()   const { return m_diskSpaceFree;   }
    QString tcFormat()        const { return m_tcFormat;        }
    QVariantList routes()     const { return m_routes;          }

    int     mouseMode()       const { return m_mouseMode;       }
    int     editMode()        const { return m_editMode;        }
    bool    snapEnabled()     const { return m_snapEnabled;     }
    QString gridUnit()        const { return m_gridUnit;        }
    bool    punchIn()         const { return m_punchIn;         }
    bool    punchOut()        const { return m_punchOut;        }
    int     workspaceMode()   const { return m_workspaceMode;   }

public slots:
    Q_INVOKABLE void newSession(const QString &name);
    Q_INVOKABLE void loadSession(const QString &path);
    Q_INVOKABLE void saveSession();
    Q_INVOKABLE void exportMaster(const QString &outputPath, const QString &format);
    Q_INVOKABLE void triggerMenuAction(const QString &category, const QString &action);

    Q_INVOKABLE void setMouseMode(int mode);
    Q_INVOKABLE void set_mouse_mode(int mode) { setMouseMode(mode); }

    Q_INVOKABLE void setEditMode(int mode);
    Q_INVOKABLE void set_edit_mode(int mode) { setEditMode(mode); }

    Q_INVOKABLE void toggleSnap();
    Q_INVOKABLE void toggle_snap() { toggleSnap(); }

    Q_INVOKABLE void setGridUnit(const QString &unit);
    Q_INVOKABLE void set_grid_unit(const QString &unit) { setGridUnit(unit); }

    Q_INVOKABLE void togglePunchIn();
    Q_INVOKABLE void toggle_punch_in() { togglePunchIn(); }

    Q_INVOKABLE void togglePunchOut();
    Q_INVOKABLE void toggle_punch_out() { togglePunchOut(); }

    Q_INVOKABLE void setWorkspaceMode(int mode);
    Q_INVOKABLE void set_workspace_mode(int mode) { setWorkspaceMode(mode); }

signals:
    void sampleRateChanged();
    void bufferSizeChanged();
    void dspLoadChanged();
    void engineRunningChanged();
    void sessionNameChanged();
    void diskSpaceFreeChanged();
    void tcFormatChanged();
    void mouseModeChanged();
    void editModeChanged();
    void snapEnabledChanged();
    void gridUnitChanged();
    void punchInChanged();
    void punchOutChanged();
    void workspaceModeChanged();
    void requestAddTrackDialog();
    void routesChanged();


private slots:
    void onTelemetryTick();

private:
    void syncRoutes();

    QVariantList m_routes;
    double  m_sampleRate      = 48000.0;
    int     m_bufferSize      = 256;
    double  m_dspLoad         = 0.0;
    bool    m_engineRunning   = false;
    QString m_sessionName     = QStringLiteral("Blank Session");
    QString m_diskSpaceFree   = QStringLiteral("142.8 GB (44 hrs @ 24/48)");
    QString m_tcFormat        = QStringLiteral("30 fps (Drop)");

    int     m_mouseMode       = 0;
    int     m_editMode        = 0;
    bool    m_snapEnabled     = true;
    QString m_gridUnit        = QStringLiteral("1/16 Note");
    bool    m_punchIn         = false;
    bool    m_punchOut        = false;
    int     m_workspaceMode   = 0;

    QTimer *m_telemetryTimer  = nullptr;
};

