#pragma once

#include <QObject>
#include <QString>
#include <QTimer>
#include <memory>

#pragma push_macro("emit")
#pragma push_macro("slots")
#pragma push_macro("signals")
#undef emit
#undef slots
#undef signals

#include "ardour/route.h"
#include "ardour/session.h"
#include "ardour/amp.h"
#include "ardour/meter.h"
#include "pbd/signals.h"

#pragma pop_macro("signals")
#pragma pop_macro("slots")
#pragma pop_macro("emit")

class QtRoute : public QObject, public PBD::ScopedConnectionList
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(bool isMuted READ isMuted WRITE setIsMuted NOTIFY isMutedChanged)
    Q_PROPERTY(bool isSoloed READ isSoloed WRITE setIsSoloed NOTIFY isSoloedChanged)
    Q_PROPERTY(float gain READ gain WRITE setGain NOTIFY gainChanged)
    Q_PROPERTY(float peakMeterLevel READ peakMeterLevel NOTIFY peakMeterLevelChanged)

public:
    explicit QtRoute(std::shared_ptr<ARDOUR::Route> route, QObject *parent = nullptr);
    ~QtRoute() override;

    QString name() const;
    
    bool isMuted() const;
    void setIsMuted(bool muted);
    
    bool isSoloed() const;
    void setIsSoloed(bool soloed);
    
    float gain() const;
    void setGain(float gain);

    float peakMeterLevel() const;

    std::shared_ptr<ARDOUR::Route> route() const { return _route; }

signals:
    void nameChanged();
    void isMutedChanged();
    void isSoloedChanged();
    void gainChanged();
    void peakMeterLevelChanged();

private:
    std::shared_ptr<ARDOUR::Route> _route;
    QTimer _meterTimer;

    // Polling function for meters and state
    void on_meter_update();
};
