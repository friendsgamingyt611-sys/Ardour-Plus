/*
 * ardour4-qt: Session & Engine Bridge — Qt6 Core Port Implementation
 * Phase 5.1
 */

#include "session_bridge.h"
#include "models/qt_route.h"
#include <QRandomGenerator>
#include <sys/statvfs.h>

SessionBridge::SessionBridge(QObject *parent)
    : QObject(parent)
{
    /* 500ms telemetry update timer */
    m_telemetryTimer = new QTimer(this);
    m_telemetryTimer->setInterval(500);
    connect(m_telemetryTimer, &QTimer::timeout, this, &SessionBridge::onTelemetryTick);
    m_telemetryTimer->start();
    onTelemetryTick();
}

void SessionBridge::syncRoutes()
{
    // Clean up old routes
    for (QVariant v : m_routes) {
        if (QObject *obj = v.value<QObject*>()) {
            obj->deleteLater();
        }
    }
    m_routes.clear();

    if (ARDOUR::AudioEngine::instance() && ARDOUR::AudioEngine::instance()->session()) {
        auto routes = ARDOUR::AudioEngine::instance()->session()->get_routes();
        if (routes) {
            for (auto const& r : *routes) {
                // Ignore Master out, Monitor out etc for now unless we want them. 
                // Wait, standard tracks are normal routes. We can just add all of them.
                m_routes.append(QVariant::fromValue((QObject*)new QtRoute(r, this)));
            }
        }
    }
    emit routesChanged();
}


void SessionBridge::newSession(const QString &name)
{
    m_sessionName = name.isEmpty() ? QStringLiteral("Untitled Session") : name;
    emit sessionNameChanged();
}

void SessionBridge::loadSession(const QString &path)
{
    Q_UNUSED(path);
    emit sessionNameChanged();
}

void SessionBridge::saveSession()
{
    /* XML Session serialization trigger */
    if (ARDOUR::AudioEngine::instance() && ARDOUR::AudioEngine::instance()->session()) {
        ARDOUR::AudioEngine::instance()->session()->save_state("");
    }
}

void SessionBridge::exportMaster(const QString &outputPath, const QString &format)
{
    Q_UNUSED(outputPath);
    Q_UNUSED(format);
    /* Master bus bouncing / stem export trigger */
}

void SessionBridge::triggerMenuAction(const QString &category, const QString &action)
{
    if (action == QStringLiteral("New Session")) {
        newSession(QStringLiteral("New Session"));
    } else if (action == QStringLiteral("Save")) {
        saveSession();
    } else if (action == QStringLiteral("Export")) {
        exportMaster(QStringLiteral("export.wav"), QStringLiteral("wav"));
    } else if (category == QStringLiteral("Track") && action == QStringLiteral("Add Track/Bus")) {
        emit requestAddTrackDialog();
    }
}

void SessionBridge::setMouseMode(int mode)
{
    if (m_mouseMode == mode) return;
    m_mouseMode = mode;
    emit mouseModeChanged();
}

void SessionBridge::setEditMode(int mode)
{
    if (m_editMode == mode) return;
    m_editMode = mode;
    emit editModeChanged();
}

void SessionBridge::toggleSnap()
{
    m_snapEnabled = !m_snapEnabled;
    emit snapEnabledChanged();
}

void SessionBridge::setGridUnit(const QString &unit)
{
    if (m_gridUnit == unit) return;
    m_gridUnit = unit;
    emit gridUnitChanged();
}

void SessionBridge::togglePunchIn()
{
    m_punchIn = !m_punchIn;
    if (ARDOUR::AudioEngine::instance() && ARDOUR::AudioEngine::instance()->session()) {
        ARDOUR::AudioEngine::instance()->session()->config.set_punch_in(m_punchIn);
    }
    emit punchInChanged();
}

void SessionBridge::togglePunchOut()
{
    m_punchOut = !m_punchOut;
    if (ARDOUR::AudioEngine::instance() && ARDOUR::AudioEngine::instance()->session()) {
        ARDOUR::AudioEngine::instance()->session()->config.set_punch_out(m_punchOut);
    }
    emit punchOutChanged();
}

void SessionBridge::setWorkspaceMode(int mode)
{
    if (m_workspaceMode == mode) return;
    m_workspaceMode = mode;
    emit workspaceModeChanged();
}

void SessionBridge::onTelemetryTick()
{
    if (ARDOUR::AudioEngine::instance()) {
        bool running = ARDOUR::AudioEngine::instance()->running();
        if (running != m_engineRunning) {
            m_engineRunning = running;
            emit engineRunningChanged();
        }

        uint32_t sr = ARDOUR::AudioEngine::instance()->sample_rate();
        if (sr > 0 && sr != m_sampleRate) {
            m_sampleRate = sr;
            emit sampleRateChanged();
        }

        uint32_t buf = ARDOUR::AudioEngine::instance()->samples_per_cycle();
        if (buf > 0 && buf != m_bufferSize) {
            m_bufferSize = buf;
            emit bufferSizeChanged();
        }

        float load = ARDOUR::AudioEngine::instance()->get_dsp_load();
        if (std::abs(load - m_dspLoad) > 0.01) {
            m_dspLoad = load;
            emit dspLoadChanged();
        }

        ARDOUR::Session *s = ARDOUR::AudioEngine::instance()->session();
        if (s) {
            QString name = QString::fromStdString(s->name());
            if (name != m_sessionName) {
                m_sessionName = name;
                emit sessionNameChanged();
            }
            
            auto routes = s->get_routes();
            if (routes && routes->size() != (size_t)m_routes.size()) {
                syncRoutes();
            }
        } else if (m_routes.size() > 0) {
            syncRoutes();
        }
    }

    /* Real Filesystem Disk Space Calculation */
    struct statvfs stat;
    if (statvfs(".", &stat) == 0) {
        double freeBytes = (double)stat.f_bavail * stat.f_frsize;
        double freeGb = freeBytes / (1024.0 * 1024.0 * 1024.0);
        double hrs = freeGb / 0.62; /* ~0.62 GB/hr for stereo 24/48 */
        QString formatted = QStringLiteral("%1 GB (%2 hrs @ 24/48)")
                               .arg(freeGb, 0, 'f', 1)
                               .arg(qMax(0, (int)hrs));
        if (formatted != m_diskSpaceFree) {
            m_diskSpaceFree = formatted;
            emit diskSpaceFreeChanged();
        }
    }
}
