/*
 * ardour4-shell: Comprehensive Transport Control Bar Implementation
 * Phase 2.5 — GTK4/Libadwaita migration of Ardour Plus
 *
 * Recreates the EXACT 2-row layout, button placements, sizes, groupings, and digital LED clocks
 * from legacy GTK2 Ardour top panel (editor_map.png) using GTK4 toolkit widgets.
 */

#include "transport_bar.h"
#include <glib/gi18n.h>

#include "ardour/audioengine.h"
#include "ardour/session.h"

struct _ArdourTransportBar {
    GtkWidget parent_instance;

    /* Transport Buttons (Sub-row 0) */
    GtkWidget *btn_click;
    GtkWidget *btn_goto_start;
    GtkWidget *btn_rewind;
    GtkWidget *btn_stop;
    GtkWidget *btn_play;
    GtkWidget *btn_ffwd;
    GtkWidget *btn_goto_end;
    GtkWidget *btn_loop;
    GtkWidget *btn_rec;
    GtkWidget *btn_midi_panic;

    /* Sub-controls (Sub-row 1) */
    GtkWidget *btn_auto_return;
    GtkWidget *btn_follow_range;
    GtkWidget *sync_combo;
    GtkWidget *btn_vs;
    GtkWidget *lbl_shuttle;

    /* Punch & Record Mode */
    GtkWidget *lbl_punch_hdr;
    GtkWidget *btn_punch_in;
    GtkWidget *btn_punch_out;
    GtkWidget *lbl_rec_hdr;
    GtkWidget *rec_mode_combo;

    /* PDC & Latency */
    GtkWidget *btn_disable_pdc;
    GtkWidget *lbl_pdc_latency;

    /* Dual Digital LED Clocks & Sub-buttons */
    GtkWidget *lbl_bbt;
    GtkWidget *btn_tempo;
    GtkWidget *btn_ts;
    GtkWidget *lbl_timecode;
    GtkWidget *btn_tc_mode;

    /* Cue Controls */
    GtkWidget *btn_rec_cues;
    GtkWidget *btn_play_cues;

    /* Range Bounds & Scrubber Stripe */
    GtkWidget *lbl_range_start;
    GtkWidget *lbl_range_end;
    GtkWidget *lbl_range_len;
    GtkWidget *scrubber_bar;

    /* Monitoring Section */
    GtkWidget *btn_mono;
    GtkWidget *btn_dim_all;
    GtkWidget *btn_mute_all;

    /* Safety & Indicator Beacons */
    GtkWidget *btn_clear_solos;
    GtkWidget *btn_audition;
    GtkWidget *btn_feedback_alert;
    GtkWidget *btn_xrun_beacon;

    gboolean is_playing;
    gboolean is_recording;
    gboolean is_looping;
    gboolean is_click_enabled;
    gboolean is_punch_in;
    gboolean is_punch_out;
    gboolean is_auto_return;
    gboolean is_follow_range;
    gboolean is_vari_speed;
    gboolean has_solos;
    gboolean has_mutes;
    guint xrun_count;

    guint clock_timer_id;
    guint frame_count;
};

G_DEFINE_TYPE (ArdourTransportBar, ardour_transport_bar, GTK_TYPE_WIDGET)

static gboolean
on_clock_tick (gpointer user_data)
{
    ArdourTransportBar *self = ARDOUR_TRANSPORT_BAR (user_data);

    if (self->is_playing) {
        self->frame_count += 4;
        guint total_secs = self->frame_count / 30;
        guint frames = self->frame_count % 30;
        guint hours = total_secs / 3600;
        guint mins = (total_secs % 3600) / 60;
        guint secs = total_secs % 60;

        char tc_buf[64];
        g_snprintf (tc_buf, sizeof(tc_buf), "%02u:%02u:%02u:%02u", hours, mins, secs, frames);
        gtk_label_set_text (GTK_LABEL (self->lbl_timecode), tc_buf);

        guint bars = (total_secs / 2) + 1;
        guint beats = ((self->frame_count / 7) % 4) + 1;
        guint ticks = (self->frame_count * 16) % 960;

        char bbt_buf[64];
        g_snprintf (bbt_buf, sizeof(bbt_buf), "%03u | %02u | %03u", bars, beats, ticks);
        gtk_label_set_text (GTK_LABEL (self->lbl_bbt), bbt_buf);

        double progress = (double)(self->frame_count % 1000) / 10.0;
        gtk_level_bar_set_value (GTK_LEVEL_BAR (self->scrubber_bar), progress);
    }
    return G_SOURCE_CONTINUE;
}

/* ── Callbacks ── */

static void
on_click_toggled (GtkButton * /*button*/, gpointer user_data)
{
    ArdourTransportBar *self = ARDOUR_TRANSPORT_BAR (user_data);
    self->is_click_enabled = !self->is_click_enabled;
    if (self->is_click_enabled) {
        gtk_widget_add_css_class (self->btn_click, "accent");
    } else {
        gtk_widget_remove_css_class (self->btn_click, "accent");
    }
}

static void
on_play_clicked (GtkButton * /*button*/, gpointer user_data)
{
    ArdourTransportBar *self = ARDOUR_TRANSPORT_BAR (user_data);
    self->is_playing = !self->is_playing;

    if (self->is_playing) {
        gtk_button_set_icon_name (GTK_BUTTON (self->btn_play), "media-playback-pause-symbolic");
        gtk_widget_add_css_class (self->btn_play, "suggested-action");
    } else {
        gtk_button_set_icon_name (GTK_BUTTON (self->btn_play), "media-playback-start-symbolic");
        gtk_widget_remove_css_class (self->btn_play, "suggested-action");
        if (self->is_auto_return) {
            self->frame_count = 0;
            gtk_label_set_text (GTK_LABEL (self->lbl_timecode), "00:00:00:00");
            gtk_label_set_text (GTK_LABEL (self->lbl_bbt), "001 | 01 | 000");
            gtk_level_bar_set_value (GTK_LEVEL_BAR (self->scrubber_bar), 0.0);
        }
    }
}

static void
on_stop_clicked (GtkButton * /*button*/, gpointer user_data)
{
    ArdourTransportBar *self = ARDOUR_TRANSPORT_BAR (user_data);
    self->is_playing = FALSE;
    self->is_recording = FALSE;
    self->frame_count = 0;

    gtk_button_set_icon_name (GTK_BUTTON (self->btn_play), "media-playback-start-symbolic");
    gtk_widget_remove_css_class (self->btn_play, "suggested-action");
    gtk_widget_remove_css_class (self->btn_rec, "destructive-action");

    gtk_label_set_text (GTK_LABEL (self->lbl_timecode), "00:00:00:00");
    gtk_label_set_text (GTK_LABEL (self->lbl_bbt), "001 | 01 | 000");
    gtk_level_bar_set_value (GTK_LEVEL_BAR (self->scrubber_bar), 0.0);
}

static void
on_rec_clicked (GtkButton * /*button*/, gpointer user_data)
{
    ArdourTransportBar *self = ARDOUR_TRANSPORT_BAR (user_data);
    self->is_recording = !self->is_recording;

    if (self->is_recording) {
        gtk_widget_add_css_class (self->btn_rec, "destructive-action");
    } else {
        gtk_widget_remove_css_class (self->btn_rec, "destructive-action");
    }
}

static void
on_loop_clicked (GtkButton * /*button*/, gpointer user_data)
{
    ArdourTransportBar *self = ARDOUR_TRANSPORT_BAR (user_data);
    self->is_looping = !self->is_looping;

    if (self->is_looping) {
        gtk_widget_add_css_class (self->btn_loop, "accent");
    } else {
        gtk_widget_remove_css_class (self->btn_loop, "accent");
    }
}

static void
on_punch_in_clicked (GtkButton * /*button*/, gpointer user_data)
{
    ArdourTransportBar *self = ARDOUR_TRANSPORT_BAR (user_data);
    self->is_punch_in = !self->is_punch_in;
    if (self->is_punch_in) {
        gtk_widget_add_css_class (self->btn_punch_in, "suggested-action");
    } else {
        gtk_widget_remove_css_class (self->btn_punch_in, "suggested-action");
    }
}

static void
on_punch_out_clicked (GtkButton * /*button*/, gpointer user_data)
{
    ArdourTransportBar *self = ARDOUR_TRANSPORT_BAR (user_data);
    self->is_punch_out = !self->is_punch_out;
    if (self->is_punch_out) {
        gtk_widget_add_css_class (self->btn_punch_out, "suggested-action");
    } else {
        gtk_widget_remove_css_class (self->btn_punch_out, "suggested-action");
    }
}

static void
on_auto_return_clicked (GtkButton * /*button*/, gpointer user_data)
{
    ArdourTransportBar *self = ARDOUR_TRANSPORT_BAR (user_data);
    self->is_auto_return = !self->is_auto_return;
    if (self->is_auto_return) {
        gtk_widget_add_css_class (self->btn_auto_return, "suggested-action");
    } else {
        gtk_widget_remove_css_class (self->btn_auto_return, "suggested-action");
    }
}

static void
on_follow_range_clicked (GtkButton * /*button*/, gpointer user_data)
{
    ArdourTransportBar *self = ARDOUR_TRANSPORT_BAR (user_data);
    self->is_follow_range = !self->is_follow_range;
    if (self->is_follow_range) {
        gtk_widget_add_css_class (self->btn_follow_range, "suggested-action");
    } else {
        gtk_widget_remove_css_class (self->btn_follow_range, "suggested-action");
    }
}

static void
on_vs_clicked (GtkButton * /*button*/, gpointer user_data)
{
    ArdourTransportBar *self = ARDOUR_TRANSPORT_BAR (user_data);
    self->is_vari_speed = !self->is_vari_speed;
    if (self->is_vari_speed) {
        gtk_widget_add_css_class (self->btn_vs, "accent");
        gtk_label_set_text (GTK_LABEL (self->lbl_shuttle), "1.2x");
    } else {
        gtk_widget_remove_css_class (self->btn_vs, "accent");
        gtk_label_set_text (GTK_LABEL (self->lbl_shuttle), "1.0x");
    }
}

static void
on_rewind_clicked (GtkButton * /*button*/, gpointer user_data)
{
    ArdourTransportBar *self = ARDOUR_TRANSPORT_BAR (user_data);
    if (self->frame_count >= 150) {
        self->frame_count -= 150;
    } else {
        self->frame_count = 0;
    }
}

static void
on_ffwd_clicked (GtkButton * /*button*/, gpointer user_data)
{
    ArdourTransportBar *self = ARDOUR_TRANSPORT_BAR (user_data);
    self->frame_count += 150;
}

static void
on_goto_start_clicked (GtkButton * /*button*/, gpointer user_data)
{
    ArdourTransportBar *self = ARDOUR_TRANSPORT_BAR (user_data);
    self->frame_count = 0;
    gtk_label_set_text (GTK_LABEL (self->lbl_timecode), "00:00:00:00");
    gtk_label_set_text (GTK_LABEL (self->lbl_bbt), "001 | 01 | 000");
    gtk_level_bar_set_value (GTK_LEVEL_BAR (self->scrubber_bar), 0.0);
}

static void
on_goto_end_clicked (GtkButton * /*button*/, gpointer user_data)
{
    ArdourTransportBar *self = ARDOUR_TRANSPORT_BAR (user_data);
    self->frame_count = 1800; // 1 min marker
    gtk_label_set_text (GTK_LABEL (self->lbl_timecode), "00:01:00:00");
    gtk_label_set_text (GTK_LABEL (self->lbl_bbt), "031 | 01 | 000");
    gtk_level_bar_set_value (GTK_LEVEL_BAR (self->scrubber_bar), 100.0);
}

static void
on_midi_panic_clicked (GtkButton * /*button*/, gpointer /*user_data*/)
{
    g_print ("GTK4 Ardour Transport: MIDI Panic triggered\n");
}

/* ── Public API Implementation ── */

void
ardour_transport_bar_trigger_xrun (ArdourTransportBar *self)
{
    g_return_if_fail (ARDOUR_IS_TRANSPORT_BAR (self));
    self->xrun_count++;
    char buf[32];
    g_snprintf (buf, sizeof(buf), "XRUN: %u", self->xrun_count);
    gtk_button_set_label (GTK_BUTTON (self->btn_xrun_beacon), buf);
    gtk_widget_remove_css_class (self->btn_xrun_beacon, "flat");
    gtk_widget_add_css_class (self->btn_xrun_beacon, "destructive-action");
}

void
ardour_transport_bar_set_solo_active (ArdourTransportBar *self, gboolean active)
{
    g_return_if_fail (ARDOUR_IS_TRANSPORT_BAR (self));
    self->has_solos = active;
    gtk_widget_set_visible (self->btn_clear_solos, active);
}

void
ardour_transport_bar_set_mute_active (ArdourTransportBar *self, gboolean active)
{
    g_return_if_fail (ARDOUR_IS_TRANSPORT_BAR (self));
    self->has_mutes = active;
    gtk_widget_set_visible (self->btn_mute_all, active);
}

/* ── Lifecycle ── */

static void
ardour_transport_bar_dispose (GObject *object)
{
    ArdourTransportBar *self = ARDOUR_TRANSPORT_BAR (object);

    if (self->clock_timer_id > 0) {
        g_source_remove (self->clock_timer_id);
        self->clock_timer_id = 0;
    }

    GtkWidget *child = gtk_widget_get_first_child (GTK_WIDGET (self));
    while (child) {
        GtkWidget *next = gtk_widget_get_next_sibling (child);
        gtk_widget_unparent (child);
        child = next;
    }

    G_OBJECT_CLASS (ardour_transport_bar_parent_class)->dispose (object);
}

static void
ardour_transport_bar_class_init (ArdourTransportBarClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    object_class->dispose = ardour_transport_bar_dispose;

    gtk_widget_class_set_layout_manager_type (GTK_WIDGET_CLASS (klass), GTK_TYPE_BIN_LAYOUT);
}

static void
ardour_transport_bar_init (ArdourTransportBar *self)
{
    self->is_playing = FALSE;
    self->is_recording = FALSE;
    self->is_looping = FALSE;
    self->is_click_enabled = FALSE;
    self->is_punch_in = FALSE;
    self->is_punch_out = FALSE;
    self->is_auto_return = FALSE;
    self->is_follow_range = FALSE;
    self->is_vari_speed = FALSE;
    self->has_solos = FALSE;
    self->has_mutes = FALSE;
    self->xrun_count = 0;
    self->frame_count = 0;

    /* Custom CSS for LED Clocks and DAW transport styling */
    GtkCssProvider *css = gtk_css_provider_new ();
    gtk_css_provider_load_from_data (css,
        ".daw-top-bar { background-color: #1a1d21; border-bottom: 1px solid #282d34; padding: 3px 6px; }\n"
        ".led-clock-box { background-color: #0b0e11; border: 1px solid #262c35; border-radius: 3px; padding: 2px 8px; }\n"
        ".led-clock-text { font-family: 'DejaVu Sans Mono', 'Liberation Mono', monospace; font-size: 15px; font-weight: bold; color: #00ffcc; }\n"
        ".range-readout-box { background-color: #14171a; border: 1px solid #242930; border-radius: 3px; padding: 2px 6px; }\n"
        ".range-readout-text { font-family: monospace; font-size: 11px; color: #abb2bf; }\n"
        ".daw-btn-compact { min-height: 24px; padding: 1px 6px; font-size: 11px; }\n", -1);
    gtk_style_context_add_provider_for_display (
        gdk_display_get_default (),
        GTK_STYLE_PROVIDER (css),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref (css);

    /* Container: 2-Row Structured Grid matching legacy GTK2 Ardour top panel */
    GtkWidget *grid = gtk_grid_new ();
    gtk_grid_set_column_spacing (GTK_GRID (grid), 6);
    gtk_grid_set_row_spacing (GTK_GRID (grid), 2);
    gtk_widget_add_css_class (grid, "daw-top-bar");
    gtk_widget_set_parent (grid, GTK_WIDGET (self));

    int col = 0;

    /* ════════════ COLUMN 0: TRANSPORT BUTTONS & SUB-OPTIONS ════════════ */
    /* Sub-row 0: Main Transport Control Row */
    GtkWidget *grp_t = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_widget_add_css_class (grp_t, "linked");

    self->btn_click = gtk_button_new_from_icon_name ("audio-volume-high-symbolic");
    gtk_widget_set_tooltip_text (self->btn_click, _("Toggle Metronome Click"));
    g_signal_connect (self->btn_click, "clicked", G_CALLBACK (on_click_toggled), self);
    gtk_box_append (GTK_BOX (grp_t), self->btn_click);

    self->btn_goto_start = gtk_button_new_from_icon_name ("media-skip-backward-symbolic");
    gtk_widget_set_tooltip_text (self->btn_goto_start, _("Go to Start of Session"));
    g_signal_connect (self->btn_goto_start, "clicked", G_CALLBACK (on_goto_start_clicked), self);
    gtk_box_append (GTK_BOX (grp_t), self->btn_goto_start);

    self->btn_rewind = gtk_button_new_from_icon_name ("media-seek-backward-symbolic");
    gtk_widget_set_tooltip_text (self->btn_rewind, _("Rewind Transport"));
    g_signal_connect (self->btn_rewind, "clicked", G_CALLBACK (on_rewind_clicked), self);
    gtk_box_append (GTK_BOX (grp_t), self->btn_rewind);

    self->btn_stop = gtk_button_new_from_icon_name ("media-playback-stop-symbolic");
    gtk_widget_set_tooltip_text (self->btn_stop, _("Stop Transport"));
    g_signal_connect (self->btn_stop, "clicked", G_CALLBACK (on_stop_clicked), self);
    gtk_box_append (GTK_BOX (grp_t), self->btn_stop);

    self->btn_play = gtk_button_new_from_icon_name ("media-playback-start-symbolic");
    gtk_widget_set_tooltip_text (self->btn_play, _("Play / Pause Transport"));
    g_signal_connect (self->btn_play, "clicked", G_CALLBACK (on_play_clicked), self);
    gtk_box_append (GTK_BOX (grp_t), self->btn_play);

    self->btn_ffwd = gtk_button_new_from_icon_name ("media-seek-forward-symbolic");
    gtk_widget_set_tooltip_text (self->btn_ffwd, _("Fast Forward Transport"));
    g_signal_connect (self->btn_ffwd, "clicked", G_CALLBACK (on_ffwd_clicked), self);
    gtk_box_append (GTK_BOX (grp_t), self->btn_ffwd);

    self->btn_goto_end = gtk_button_new_from_icon_name ("media-skip-forward-symbolic");
    gtk_widget_set_tooltip_text (self->btn_goto_end, _("Go to End of Session"));
    g_signal_connect (self->btn_goto_end, "clicked", G_CALLBACK (on_goto_end_clicked), self);
    gtk_box_append (GTK_BOX (grp_t), self->btn_goto_end);

    self->btn_loop = gtk_button_new_from_icon_name ("media-playlist-repeat-symbolic");
    gtk_widget_set_tooltip_text (self->btn_loop, _("Play Loop Range"));
    g_signal_connect (self->btn_loop, "clicked", G_CALLBACK (on_loop_clicked), self);
    gtk_box_append (GTK_BOX (grp_t), self->btn_loop);

    self->btn_rec = gtk_button_new_from_icon_name ("media-record-symbolic");
    gtk_widget_set_tooltip_text (self->btn_rec, _("Global Master Record Arm"));
    g_signal_connect (self->btn_rec, "clicked", G_CALLBACK (on_rec_clicked), self);
    gtk_box_append (GTK_BOX (grp_t), self->btn_rec);

    self->btn_midi_panic = gtk_button_new_from_icon_name ("dialog-warning-symbolic");
    gtk_widget_set_tooltip_text (self->btn_midi_panic, _("MIDI Panic: Send All-Notes-Off"));
    g_signal_connect (self->btn_midi_panic, "clicked", G_CALLBACK (on_midi_panic_clicked), self);
    gtk_box_append (GTK_BOX (grp_t), self->btn_midi_panic);

    gtk_grid_attach (GTK_GRID (grid), grp_t, col, 0, 1, 1);

    /* Sub-row 1: Sub-layout (Auto Return | Follow Range | Sync Dropdown | Vari-Speed & Shuttle) */
    GtkWidget *grp_sub = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);

    self->btn_auto_return = gtk_button_new_with_label (_("Auto Return"));
    gtk_widget_add_css_class (self->btn_auto_return, "daw-btn-compact");
    gtk_widget_set_tooltip_text (self->btn_auto_return, _("Snap playhead back to start position on stop"));
    g_signal_connect (self->btn_auto_return, "clicked", G_CALLBACK (on_auto_return_clicked), self);
    gtk_box_append (GTK_BOX (grp_sub), self->btn_auto_return);

    self->btn_follow_range = gtk_button_new_with_label (_("Follow Range"));
    gtk_widget_add_css_class (self->btn_follow_range, "daw-btn-compact");
    gtk_widget_set_tooltip_text (self->btn_follow_range, _("Force transport playhead to track selection range"));
    g_signal_connect (self->btn_follow_range, "clicked", G_CALLBACK (on_follow_range_clicked), self);
    gtk_box_append (GTK_BOX (grp_sub), self->btn_follow_range);

    const char *sync_options[] = { _("INT / M-Clk"), _("JACK Sync"), _("MTC Sync"), _("LTC Sync"), NULL };
    self->sync_combo = gtk_drop_down_new_from_strings (sync_options);
    gtk_widget_set_tooltip_text (self->sync_combo, _("Transport Synchronization Master Source"));
    gtk_box_append (GTK_BOX (grp_sub), self->sync_combo);

    self->btn_vs = gtk_button_new_with_label (_("VS"));
    gtk_widget_add_css_class (self->btn_vs, "daw-btn-compact");
    gtk_widget_set_tooltip_text (self->btn_vs, _("Toggle Vari-Speed Shuttle Multiplier"));
    g_signal_connect (self->btn_vs, "clicked", G_CALLBACK (on_vs_clicked), self);
    gtk_box_append (GTK_BOX (grp_sub), self->btn_vs);

    self->lbl_shuttle = gtk_label_new ("1.0x");
    gtk_widget_add_css_class (self->lbl_shuttle, "monospace");
    gtk_box_append (GTK_BOX (grp_sub), self->lbl_shuttle);

    gtk_grid_attach (GTK_GRID (grid), grp_sub, col, 1, 1, 1);
    col++;

    /* Separator 1 */
    gtk_grid_attach (GTK_GRID (grid), gtk_separator_new (GTK_ORIENTATION_VERTICAL), col++, 0, 1, 2);

    /* ════════════ COLUMN 2: PUNCH & RECORD MODE ════════════ */
    /* Sub-row 0: Punch Labels & In/Out Toggles */
    GtkWidget *grp_punch = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 3);
    self->lbl_punch_hdr = gtk_label_new (_("Punch:"));
    gtk_widget_add_css_class (self->lbl_punch_hdr, "dim-label");
    gtk_box_append (GTK_BOX (grp_punch), self->lbl_punch_hdr);

    self->btn_punch_in = gtk_button_new_with_label (_("In"));
    gtk_widget_add_css_class (self->btn_punch_in, "daw-btn-compact");
    gtk_widget_set_tooltip_text (self->btn_punch_in, _("Toggle Punch-In Bound"));
    g_signal_connect (self->btn_punch_in, "clicked", G_CALLBACK (on_punch_in_clicked), self);
    gtk_box_append (GTK_BOX (grp_punch), self->btn_punch_in);

    self->btn_punch_out = gtk_button_new_with_label (_("Out"));
    gtk_widget_add_css_class (self->btn_punch_out, "daw-btn-compact");
    gtk_widget_set_tooltip_text (self->btn_punch_out, _("Toggle Punch-Out Bound"));
    g_signal_connect (self->btn_punch_out, "clicked", G_CALLBACK (on_punch_out_clicked), self);
    gtk_box_append (GTK_BOX (grp_punch), self->btn_punch_out);

    gtk_grid_attach (GTK_GRID (grid), grp_punch, col, 0, 1, 1);

    /* Sub-row 1: Record Mode Dropdown */
    GtkWidget *grp_rec_mode = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 3);
    self->lbl_rec_hdr = gtk_label_new (_("Rec:"));
    gtk_widget_add_css_class (self->lbl_rec_hdr, "dim-label");
    gtk_box_append (GTK_BOX (grp_rec_mode), self->lbl_rec_hdr);

    const char *rec_modes[] = { _("Layered"), _("Non-destructive"), _("Sound on Sound"), NULL };
    self->rec_mode_combo = gtk_drop_down_new_from_strings (rec_modes);
    gtk_widget_set_tooltip_text (self->rec_mode_combo, _("Record Mode Strategy"));
    gtk_box_append (GTK_BOX (grp_rec_mode), self->rec_mode_combo);

    gtk_grid_attach (GTK_GRID (grid), grp_rec_mode, col, 1, 1, 1);
    col++;

    /* Separator 2 */
    gtk_grid_attach (GTK_GRID (grid), gtk_separator_new (GTK_ORIENTATION_VERTICAL), col++, 0, 1, 2);

    /* ════════════ COLUMN 4: PDC LATENCY SECTION ════════════ */
    self->btn_disable_pdc = gtk_button_new_with_label (_("Disable PDC"));
    gtk_widget_add_css_class (self->btn_disable_pdc, "daw-btn-compact");
    gtk_widget_set_tooltip_text (self->btn_disable_pdc, _("Disable Plugin Delay Compensation"));
    gtk_grid_attach (GTK_GRID (grid), self->btn_disable_pdc, col, 0, 1, 1);

    self->lbl_pdc_latency = gtk_label_new (_("I/O Latency: 537.3 ms"));
    gtk_widget_add_css_class (self->lbl_pdc_latency, "dim-label");
    gtk_widget_set_halign (self->lbl_pdc_latency, GTK_ALIGN_CENTER);
    gtk_grid_attach (GTK_GRID (grid), self->lbl_pdc_latency, col, 1, 1, 1);
    col++;

    /* Separator 3 */
    gtk_grid_attach (GTK_GRID (grid), gtk_separator_new (GTK_ORIENTATION_VERTICAL), col++, 0, 1, 2);

    /* ════════════ COLUMN 6: PRIMARY CLOCK (BBT & TEMPO/METER) ════════════ */
    GtkWidget *card_bbt = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_add_css_class (card_bbt, "led-clock-box");

    self->lbl_bbt = gtk_label_new ("040 | 01 | 1341");
    gtk_widget_add_css_class (self->lbl_bbt, "led-clock-text");
    gtk_box_append (GTK_BOX (card_bbt), self->lbl_bbt);
    gtk_grid_attach (GTK_GRID (grid), card_bbt, col, 0, 1, 1);

    GtkWidget *grp_tempo = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
    self->btn_tempo = gtk_button_new_with_label ("120.000 BPM");
    gtk_widget_add_css_class (self->btn_tempo, "daw-btn-compact");
    gtk_box_append (GTK_BOX (grp_tempo), self->btn_tempo);

    self->btn_ts = gtk_button_new_with_label ("4/4 TS");
    gtk_widget_add_css_class (self->btn_ts, "daw-btn-compact");
    gtk_box_append (GTK_BOX (grp_tempo), self->btn_ts);

    gtk_grid_attach (GTK_GRID (grid), grp_tempo, col, 1, 1, 1);
    col++;

    /* Separator 4 */
    gtk_grid_attach (GTK_GRID (grid), gtk_separator_new (GTK_ORIENTATION_VERTICAL), col++, 0, 1, 2);

    /* ════════════ COLUMN 8: SECONDARY CLOCK (TIMECODE) ════════════ */
    GtkWidget *card_tc = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_add_css_class (card_tc, "led-clock-box");

    self->lbl_timecode = gtk_label_new ("00:01:18:10");
    gtk_widget_add_css_class (self->lbl_timecode, "led-clock-text");
    gtk_box_append (GTK_BOX (card_tc), self->lbl_timecode);
    gtk_grid_attach (GTK_GRID (grid), card_tc, col, 0, 1, 1);

    self->btn_tc_mode = gtk_button_new_with_label ("INT / M-Clk");
    gtk_widget_add_css_class (self->btn_tc_mode, "daw-btn-compact");
    gtk_grid_attach (GTK_GRID (grid), self->btn_tc_mode, col, 1, 1, 1);
    col++;

    /* Separator 5 */
    gtk_grid_attach (GTK_GRID (grid), gtk_separator_new (GTK_ORIENTATION_VERTICAL), col++, 0, 1, 2);

    /* ════════════ COLUMN 10: CUE CONTROLS & RANGE BOUNDS ════════════ */
    GtkWidget *grp_cues = gtk_box_new (GTK_ORIENTATION_VERTICAL, 1);
    self->btn_rec_cues = gtk_button_new_with_label (_("Rec Cues"));
    gtk_widget_add_css_class (self->btn_rec_cues, "daw-btn-compact");
    gtk_box_append (GTK_BOX (grp_cues), self->btn_rec_cues);

    self->btn_play_cues = gtk_button_new_with_label (_("Play Cues"));
    gtk_widget_add_css_class (self->btn_play_cues, "daw-btn-compact");
    gtk_box_append (GTK_BOX (grp_cues), self->btn_play_cues);

    gtk_grid_attach (GTK_GRID (grid), grp_cues, col, 0, 1, 2);
    col++;

    /* Separator 6 */
    gtk_grid_attach (GTK_GRID (grid), gtk_separator_new (GTK_ORIENTATION_VERTICAL), col++, 0, 1, 2);

    /* ════════════ COLUMN 12: RANGE BOUNDS & MINI SCRUBBER STRIPE ════════════ */
    GtkWidget *grp_range = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_widget_add_css_class (grp_range, "range-readout-box");

    self->lbl_range_start = gtk_label_new (_("Start: 00:00:00:00"));
    gtk_widget_add_css_class (self->lbl_range_start, "range-readout-text");
    gtk_box_append (GTK_BOX (grp_range), self->lbl_range_start);

    self->lbl_range_end = gtk_label_new (_("End: 00:01:00:00"));
    gtk_widget_add_css_class (self->lbl_range_end, "range-readout-text");
    gtk_box_append (GTK_BOX (grp_range), self->lbl_range_end);

    self->lbl_range_len = gtk_label_new (_("Len: 00:01:00:00"));
    gtk_widget_add_css_class (self->lbl_range_len, "range-readout-text");
    gtk_box_append (GTK_BOX (grp_range), self->lbl_range_len);

    gtk_grid_attach (GTK_GRID (grid), grp_range, col, 0, 1, 1);

    /* Mini Timeline Scrubber Stripe across bottom of Range Box */
    self->scrubber_bar = gtk_level_bar_new_for_interval (0.0, 100.0);
    gtk_level_bar_set_value (GTK_LEVEL_BAR (self->scrubber_bar), 0.0);
    gtk_widget_set_hexpand (self->scrubber_bar, TRUE);
    gtk_widget_set_valign (self->scrubber_bar, GTK_ALIGN_CENTER);
    gtk_widget_set_tooltip_text (self->scrubber_bar, _("Mini Transport Timeline Overview Scrubber"));
    gtk_grid_attach (GTK_GRID (grid), self->scrubber_bar, col, 1, 1, 1);
    col++;

    /* Separator 7 */
    gtk_grid_attach (GTK_GRID (grid), gtk_separator_new (GTK_ORIENTATION_VERTICAL), col++, 0, 1, 2);

    /* ════════════ COLUMN 14: AUDIO MONITORING SECTION ════════════ */
    GtkWidget *grp_mon = gtk_box_new (GTK_ORIENTATION_VERTICAL, 1);
    self->btn_mono = gtk_button_new_with_label (_("Mono"));
    gtk_widget_add_css_class (self->btn_mono, "daw-btn-compact");
    gtk_box_append (GTK_BOX (grp_mon), self->btn_mono);

    self->btn_dim_all = gtk_button_new_with_label (_("Dim All"));
    gtk_widget_add_css_class (self->btn_dim_all, "daw-btn-compact");
    gtk_box_append (GTK_BOX (grp_mon), self->btn_dim_all);

    self->btn_mute_all = gtk_button_new_with_label (_("Mute All"));
    gtk_widget_add_css_class (self->btn_mute_all, "daw-btn-compact");
    gtk_box_append (GTK_BOX (grp_mon), self->btn_mute_all);

    gtk_grid_attach (GTK_GRID (grid), grp_mon, col, 0, 1, 2);
    col++;

    /* Separator 8 */
    gtk_grid_attach (GTK_GRID (grid), gtk_separator_new (GTK_ORIENTATION_VERTICAL), col++, 0, 1, 2);

    /* ════════════ COLUMN 16: SAFETY & ALERT BEACONS ════════════ */
    GtkWidget *grp_alerts = gtk_box_new (GTK_ORIENTATION_VERTICAL, 1);

    self->btn_clear_solos = gtk_button_new_with_label (_("Solo"));
    gtk_widget_add_css_class (self->btn_clear_solos, "daw-btn-compact");
    gtk_box_append (GTK_BOX (grp_alerts), self->btn_clear_solos);

    self->btn_audition = gtk_button_new_with_label (_("Audition"));
    gtk_widget_add_css_class (self->btn_audition, "daw-btn-compact");
    gtk_box_append (GTK_BOX (grp_alerts), self->btn_audition);

    self->btn_feedback_alert = gtk_button_new_with_label (_("Feedback"));
    gtk_widget_add_css_class (self->btn_feedback_alert, "daw-btn-compact");
    gtk_box_append (GTK_BOX (grp_alerts), self->btn_feedback_alert);

    self->btn_xrun_beacon = gtk_button_new_with_label (_("XRUN: 0"));
    gtk_widget_add_css_class (self->btn_xrun_beacon, "daw-btn-compact");
    gtk_widget_add_css_class (self->btn_xrun_beacon, "flat");
    gtk_box_append (GTK_BOX (grp_alerts), self->btn_xrun_beacon);

    gtk_grid_attach (GTK_GRID (grid), grp_alerts, col, 0, 1, 2);

    /* Start Clock Timer (10Hz tick for UI clock update) */
    self->clock_timer_id = g_timeout_add (100, on_clock_tick, self);
}

GtkWidget *
ardour_transport_bar_new (void)
{
    return GTK_WIDGET (g_object_new (ARDOUR_TYPE_TRANSPORT_BAR, NULL));
}

/* ════════════════════════════════════════════════════════════════════════════
 * PUBLIC TOGGLE ACTION API — called by shell_window.cc GAction handlers
 * ════════════════════════════════════════════════════════════════════════════ */

void
ardour_transport_bar_toggle_play (ArdourTransportBar *self)
{
    g_return_if_fail (ARDOUR_IS_TRANSPORT_BAR (self));
    self->is_playing = !self->is_playing;
    if (self->is_playing) {
        gtk_button_set_label (GTK_BUTTON (self->btn_play), "▶");
        gtk_widget_add_css_class (self->btn_play, "suggested-action");
    } else {
        gtk_button_set_label (GTK_BUTTON (self->btn_play), "▶");
        gtk_widget_remove_css_class (self->btn_play, "suggested-action");
    }
}

void
ardour_transport_bar_stop (ArdourTransportBar *self)
{
    g_return_if_fail (ARDOUR_IS_TRANSPORT_BAR (self));
    self->is_playing = FALSE;
    gtk_widget_remove_css_class (self->btn_play, "suggested-action");
}

void
ardour_transport_bar_toggle_rec (ArdourTransportBar *self)
{
    g_return_if_fail (ARDOUR_IS_TRANSPORT_BAR (self));
    self->is_recording = !self->is_recording;
    if (self->is_recording) {
        gtk_widget_add_css_class (self->btn_rec, "destructive-action");
    } else {
        gtk_widget_remove_css_class (self->btn_rec, "destructive-action");
    }
}

void
ardour_transport_bar_toggle_loop (ArdourTransportBar *self)
{
    g_return_if_fail (ARDOUR_IS_TRANSPORT_BAR (self));
    self->is_looping = !self->is_looping;
    if (self->is_looping) {
        gtk_widget_add_css_class (self->btn_loop, "suggested-action");
    } else {
        gtk_widget_remove_css_class (self->btn_loop, "suggested-action");
    }
}

void
ardour_transport_bar_toggle_click (ArdourTransportBar *self)
{
    g_return_if_fail (ARDOUR_IS_TRANSPORT_BAR (self));
    self->is_click_enabled = !self->is_click_enabled;
    if (self->is_click_enabled) {
        gtk_widget_add_css_class (self->btn_click, "suggested-action");
    } else {
        gtk_widget_remove_css_class (self->btn_click, "suggested-action");
    }
}

void
ardour_transport_bar_toggle_punch_in (ArdourTransportBar *self)
{
    g_return_if_fail (ARDOUR_IS_TRANSPORT_BAR (self));
    self->is_punch_in = !self->is_punch_in;
    if (self->is_punch_in) {
        gtk_widget_add_css_class (self->btn_punch_in, "destructive-action");
    } else {
        gtk_widget_remove_css_class (self->btn_punch_in, "destructive-action");
    }
}

void
ardour_transport_bar_toggle_punch_out (ArdourTransportBar *self)
{
    g_return_if_fail (ARDOUR_IS_TRANSPORT_BAR (self));
    self->is_punch_out = !self->is_punch_out;
    if (self->is_punch_out) {
        gtk_widget_add_css_class (self->btn_punch_out, "destructive-action");
    } else {
        gtk_widget_remove_css_class (self->btn_punch_out, "destructive-action");
    }
}

void
ardour_transport_bar_midi_panic (ArdourTransportBar *self)
{
    g_return_if_fail (ARDOUR_IS_TRANSPORT_BAR (self));
    /* Flash the MIDI panic button red briefly */
    gtk_widget_add_css_class (self->btn_midi_panic, "destructive-action");
    g_timeout_add (400, [](gpointer data) -> gboolean {
        GtkWidget *btn = GTK_WIDGET (data);
        gtk_widget_remove_css_class (btn, "destructive-action");
        return G_SOURCE_REMOVE;
    }, self->btn_midi_panic);
}
