#include "qt_route.h"
#include <QColor>

QtRoute::QtRoute(std::shared_ptr<ARDOUR::Route> route, QObject *parent)
    : QObject(parent), _route(route)
{
    // Start polling timer for UI updates (30 Hz)
    connect(&_meterTimer, &QTimer::timeout, this, &QtRoute::on_meter_update);
    _meterTimer.start(33); // ~30 FPS
}

QtRoute::~QtRoute()
{
}

QString QtRoute::name() const
{
    if (_route) {
        return QString::fromStdString(_route->name());
    }
    return QStringLiteral("Unknown");
}

bool QtRoute::isMuted() const
{
    if (_route && _route->mute_control()) {
        return _route->mute_control()->muted();
    }
    return false;
}

void QtRoute::setIsMuted(bool muted)
{
    if (_route && _route->mute_control()) {
        _route->mute_control()->set_value(muted ? 1.0 : 0.0, PBD::Controllable::NoGroup);
        emit isMutedChanged();
    }
}

bool QtRoute::isSoloed() const
{
    if (_route && _route->solo_control()) {
        return _route->soloed();
    }
    return false;
}

void QtRoute::setIsSoloed(bool soloed)
{
    if (_route && _route->solo_control()) {
        _route->solo_control()->set_value(soloed ? 1.0 : 0.0, PBD::Controllable::NoGroup);
        emit isSoloedChanged();
    }
}

float QtRoute::gain() const
{
    if (_route && _route->main_outs() && _route->main_outs()->gain_control()) {
        return _route->main_outs()->gain_control()->get_value();
    }
    return 1.0f; // 0 dB
}

void QtRoute::setGain(float gain)
{
    if (_route && _route->main_outs() && _route->main_outs()->gain_control()) {
        _route->main_outs()->gain_control()->set_value(gain, PBD::Controllable::NoGroup);
        emit gainChanged();
    }
}

float QtRoute::peakMeterLevel() const
{
    if (_route && _route->peak_meter()) {
        // Return meter level for channel 0, MeterPeak
        return _route->peak_meter()->meter_level(0, ARDOUR::MeterPeak);
    }
    return 0.0f;
}

void QtRoute::on_meter_update()
{
    // Meter polling (always updates quickly)
    emit peakMeterLevelChanged();
    
    // Also poll state changes (Mute/Solo/Gain) that might have changed from other controllers (e.g. Mackie Control, GTK window)
    static bool lastMute = false;
    static bool lastSolo = false;
    static float lastGain = 1.0f;
    
    bool currentMute = isMuted();
    if (currentMute != lastMute) {
        lastMute = currentMute;
        emit isMutedChanged();
    }
    
    bool currentSolo = isSoloed();
    if (currentSolo != lastSolo) {
        lastSolo = currentSolo;
        emit isSoloedChanged();
    }
    
    float currentGain = gain();
    if (currentGain != lastGain) {
        lastGain = currentGain;
        emit gainChanged();
    }
}
