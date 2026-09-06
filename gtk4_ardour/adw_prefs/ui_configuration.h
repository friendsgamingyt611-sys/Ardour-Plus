/*
 * ardour4-prefs: UI Configuration Fallback/Stub Header for GTK4
 * Phase 0 — GTK4/Libadwaita migration of Ardour Plus
 */

#pragma once

#include <string>
#include <cstdint>

class UIConfiguration
{
public:
    static UIConfiguration& instance()
    {
        static UIConfiguration inst;
        return inst;
    }

    std::string get_color_file() const { return "ardour-dark.colors"; }
    uint32_t get_font_scale() const { return 1024; }
    bool get_show_secondary_clock() const { return true; }
    bool get_show_toolbar_selclock() const { return true; }
    bool get_show_toolbar_monitor_info() const { return true; }
    bool get_super_rapid_clock_update() const { return false; }
    uint32_t get_waveform_shape() const { return 0; }
    bool get_mixer_strip_rec_enable() const { return true; }
    bool get_save_export_mixer_screenshot() const { return false; }
    bool get_prefer_inline_over_gui() const { return false; }
    bool get_only_copy_imported_files() const { return true; }
    bool get_show_region_names() const { return true; }
    bool get_autoscroll_editor() const { return true; }
    bool get_follow_edits() const { return false; }
    bool get_show_waveforms() const { return true; }
    bool get_show_waveforms_while_recording() const { return true; }
    bool get_use_mouse_position_as_zoom_focus_on_scroll() const { return true; }
    bool get_sound_midi_notes() const { return true; }
    bool get_allow_non_quarter_pulse() const { return false; }
};
