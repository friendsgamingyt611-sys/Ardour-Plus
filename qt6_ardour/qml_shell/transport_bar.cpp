/*
 * ardour4-qt: Transport Bar — Qt6 Implementation
 * Phase Qt6-A
 */

#include "transport_bar.h"
#include <algorithm>

TransportBar::TransportBar(QObject *parent)
    : QObject(parent)
{
    m_clockTimer = new QTimer(this);
    m_clockTimer->setInterval(50);
    connect(m_clockTimer, &QTimer::timeout, this, &TransportBar::onClockTick);
    m_clockTimer->start();
}

void TransportBar::play()
{
    m_isPlaying = true;
    emit isPlayingChanged();
    if (ARDOUR::AudioEngine::instance() && ARDOUR::AudioEngine::instance()->session()) {
        ARDOUR::AudioEngine::instance()->session()->request_transport_speed(1.0);
    }
}

void TransportBar::stop()
{
    m_isPlaying = false;
    emit isPlayingChanged();
    if (ARDOUR::AudioEngine::instance() && ARDOUR::AudioEngine::instance()->session()) {
        ARDOUR::AudioEngine::instance()->session()->request_transport_speed(0.0);
        if (m_autoReturnEnabled) {
            ARDOUR::AudioEngine::instance()->session()->request_locate(0);
        }
    } else if (m_autoReturnEnabled) {
        m_playheadPosition = 0.0;
        emit playheadPositionChanged();
    }
}

void TransportBar::rewind()
{
    if (ARDOUR::AudioEngine::instance() && ARDOUR::AudioEngine::instance()->session()) {
        auto *s = ARDOUR::AudioEngine::instance()->session();
        samplepos_t cur = s->transport_sample();
        samplepos_t step = s->sample_rate() * 5;
        s->request_locate((cur > step) ? (cur - step) : 0);
    } else {
        m_playheadPosition = qMax(0.0, m_playheadPosition - 5.0);
        emit playheadPositionChanged();
    }
}

void TransportBar::fast_forward()
{
    if (ARDOUR::AudioEngine::instance() && ARDOUR::AudioEngine::instance()->session()) {
        auto *s = ARDOUR::AudioEngine::instance()->session();
        s->request_locate(s->transport_sample() + s->sample_rate() * 5);
    } else {
        m_playheadPosition += 5.0;
        emit playheadPositionChanged();
    }
}

void TransportBar::toggle_rec()
{
    m_isRecording = !m_isRecording;
    emit isRecordingChanged();
    if (ARDOUR::AudioEngine::instance() && ARDOUR::AudioEngine::instance()->session()) {
        ARDOUR::AudioEngine::instance()->session()->maybe_enable_record();
    }
}

void TransportBar::toggle_loop()
{
    m_isLooping = !m_isLooping;
    emit isLoopingChanged();
    if (ARDOUR::AudioEngine::instance() && ARDOUR::AudioEngine::instance()->session()) {
        ARDOUR::AudioEngine::instance()->session()->request_play_loop(m_isLooping);
    }
}

void TransportBar::toggle_punch()
{
    m_punchEnabled = !m_punchEnabled;
    emit punchEnabledChanged();
}

void TransportBar::toggle_auto_return()
{
    m_autoReturnEnabled = !m_autoReturnEnabled;
    emit autoReturnEnabledChanged();
}

void TransportBar::toggle_follow_range()
{
    m_followRangeEnabled = !m_followRangeEnabled;
    emit followRangeEnabledChanged();
}

void TransportBar::toggle_click()
{
    m_clickEnabled = !m_clickEnabled;
    emit clickEnabledChanged();
}

void TransportBar::setShuttleSpeed(double speed)
{
    if (qFuzzyCompare(m_shuttleSpeed, speed)) return;
    m_shuttleSpeed = speed;
    emit shuttleSpeedChanged();
    if (m_isPlaying && ARDOUR::AudioEngine::instance() && ARDOUR::AudioEngine::instance()->session()) {
        ARDOUR::AudioEngine::instance()->session()->request_transport_speed(m_shuttleSpeed);
    }
}

void TransportBar::setTempoBpm(double bpm)
{
    if (qFuzzyCompare(m_tempoBpm, bpm)) return;
    m_tempoBpm = bpm;
    emit tempoBpmChanged();
}

void TransportBar::onClockTick()
{
    if (ARDOUR::AudioEngine::instance() && ARDOUR::AudioEngine::instance()->session()) {
        auto *s = ARDOUR::AudioEngine::instance()->session();
        bool playing = (s->transport_speed() != 0.0);
        if (playing != m_isPlaying) {
            m_isPlaying = playing;
            emit isPlayingChanged();
        }

        samplepos_t pos = s->transport_sample();
        m_playheadPosition = static_cast<double>(pos) / s->sample_rate();
        emit playheadPositionChanged();

        Timecode::Time t;
        s->timecode_time(pos, t);
        char tc_buf[64];
        snprintf(tc_buf, sizeof(tc_buf), "%02d:%02d:%02d:%02d", t.hours, t.minutes, t.seconds, t.frames);
        m_timecodeString = QString::fromUtf8(tc_buf);
        emit timecodeStringChanged();
        return;
    }

    /* Fallback clock tick simulation when no engine session attached */
    if (!m_isPlaying) return;

    m_frameCount++;
    m_playheadPosition += 0.05 * m_shuttleSpeed;
    emit playheadPositionChanged();

    int total_subbeats = m_frameCount;
    int beats_per_bar  = 4;
    int subbeats_per_beat = 5;

    int total_beats = total_subbeats / subbeats_per_beat;
    int subbeat     = (total_subbeats % subbeats_per_beat) * (1000 / subbeats_per_beat);
    int beat        = (total_beats % beats_per_bar) + 1;
    int bar         = (total_beats / beats_per_bar) + 1;

    m_bbtString = QStringLiteral("%1|%2|%3")
                      .arg(bar,  3, 10, QLatin1Char('0'))
                      .arg(beat, 2, 10, QLatin1Char('0'))
                      .arg(subbeat, 3, 10, QLatin1Char('0'));
    emit bbtStringChanged();

    int total_frames = m_frameCount * 25 / 10;
    int ff = total_frames % 25;
    int ss = (total_frames / 25) % 60;
    int mm = (total_frames / 25 / 60) % 60;
    int hh = (total_frames / 25 / 3600);

    m_timecodeString = QStringLiteral("%1:%2:%3:%4")
                          .arg(hh, 2, 10, QLatin1Char('0'))
                          .arg(mm, 2, 10, QLatin1Char('0'))
                          .arg(ss, 2, 10, QLatin1Char('0'))
                          .arg(ff, 2, 10, QLatin1Char('0'));
    emit timecodeStringChanged();
}

