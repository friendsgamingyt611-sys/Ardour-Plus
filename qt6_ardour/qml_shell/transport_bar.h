/*
 * ardour4-qt: Transport Bar — Qt6 Port Header
 * Phase Qt6-A
 */

#pragma once
#include <QObject>
#include <QString>
#include <QTimer>

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


class TransportBar : public QObject

{
    Q_OBJECT

    Q_PROPERTY(bool isPlaying           READ isPlaying           NOTIFY isPlayingChanged)
    Q_PROPERTY(bool isRecording         READ isRecording         NOTIFY isRecordingChanged)
    Q_PROPERTY(bool isLooping           READ isLooping           NOTIFY isLoopingChanged)
    Q_PROPERTY(bool punchEnabled        READ punchEnabled        NOTIFY punchEnabledChanged)
    Q_PROPERTY(bool autoReturnEnabled   READ autoReturnEnabled   NOTIFY autoReturnEnabledChanged)
    Q_PROPERTY(bool followRangeEnabled  READ followRangeEnabled  NOTIFY followRangeEnabledChanged)
    Q_PROPERTY(bool clickEnabled        READ clickEnabled        NOTIFY clickEnabledChanged)
    Q_PROPERTY(double shuttleSpeed      READ shuttleSpeed WRITE setShuttleSpeed NOTIFY shuttleSpeedChanged)
    Q_PROPERTY(double playheadPosition READ playheadPosition    NOTIFY playheadPositionChanged)
    Q_PROPERTY(double tempoBpm          READ tempoBpm WRITE setTempoBpm NOTIFY tempoBpmChanged)
    Q_PROPERTY(QString bbtString        READ bbtString           NOTIFY bbtStringChanged)
    Q_PROPERTY(QString timecodeString   READ timecodeString      NOTIFY timecodeStringChanged)
    Q_PROPERTY(QString meterString      READ meterString         NOTIFY meterStringChanged)

public:
    explicit TransportBar(QObject *parent = nullptr);
    ~TransportBar() override = default;

    bool isPlaying() const { return m_isPlaying; }
    bool isRecording() const { return m_isRecording; }
    bool isLooping() const { return m_isLooping; }
    bool punchEnabled() const { return m_punchEnabled; }
    bool autoReturnEnabled() const { return m_autoReturnEnabled; }
    bool followRangeEnabled() const { return m_followRangeEnabled; }
    bool clickEnabled() const { return m_clickEnabled; }
    double shuttleSpeed() const { return m_shuttleSpeed; }
    double playheadPosition() const { return m_playheadPosition; }
    double tempoBpm() const { return m_tempoBpm; }
    QString bbtString() const { return m_bbtString; }
    QString timecodeString() const { return m_timecodeString; }
    QString meterString() const { return m_meterString; }

    void setShuttleSpeed(double speed);
    void setTempoBpm(double bpm);

public slots:
    Q_INVOKABLE void play();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void rewind();
    Q_INVOKABLE void fast_forward();
    Q_INVOKABLE void toggle_rec();
    Q_INVOKABLE void toggle_loop();
    Q_INVOKABLE void toggle_punch();
    Q_INVOKABLE void toggle_auto_return();
    Q_INVOKABLE void toggle_follow_range();
    Q_INVOKABLE void toggle_click();

signals:
    void isPlayingChanged();
    void isRecordingChanged();
    void isLoopingChanged();
    void punchEnabledChanged();
    void autoReturnEnabledChanged();
    void followRangeEnabledChanged();
    void clickEnabledChanged();
    void shuttleSpeedChanged();
    void playheadPositionChanged();
    void tempoBpmChanged();
    void bbtStringChanged();
    void timecodeStringChanged();
    void meterStringChanged();

private slots:
    void onClockTick();

private:
    bool m_isPlaying = false;
    bool m_isRecording = false;
    bool m_isLooping = false;
    bool m_punchEnabled = false;
    bool m_autoReturnEnabled = false;
    bool m_followRangeEnabled = false;
    bool m_clickEnabled = false;
    double m_shuttleSpeed = 1.0;
    double m_playheadPosition = 0.0;
    double m_tempoBpm = 120.0;
    QString m_bbtString = QStringLiteral("001|01|000");
    QString m_timecodeString = QStringLiteral("00:00:00:00");
    QString m_meterString = QStringLiteral("4/4");

    QTimer *m_clockTimer = nullptr;
    int m_frameCount = 0;
};
