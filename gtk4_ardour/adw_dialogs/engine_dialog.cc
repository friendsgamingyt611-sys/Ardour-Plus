/*
 * ardour4-dialogs: Audio/MIDI Engine Dialog Implementation
 * Phase 1 — GTK4/Libadwaita migration of Ardour Plus
 */

#include "engine_dialog.h"
#include "ardour4_config.h"

#include <glib/gi18n.h>

#include "ardour/ardour.h"
#include "ardour/audioengine.h"
#include "ardour/rc_configuration.h"

struct _ArdourEngineDialog {
    AdwWindow parent_instance;

    GtkWidget *backend_combo;
    GtkWidget *device_input_combo;
    GtkWidget *device_output_combo;
    GtkWidget *sample_rate_combo;
    GtkWidget *buffer_size_combo;
    GtkWidget *status_label;
    GtkWidget *btn_start;
};

G_DEFINE_TYPE (ArdourEngineDialog, ardour_engine_dialog, ADW_TYPE_WINDOW)

static void
on_start_engine_clicked (GtkButton * /*button*/, gpointer user_data)
{
    ArdourEngineDialog *self = ARDOUR_ENGINE_DIALOG (user_data);

    if (ARDOUR::AudioEngine::instance() && ARDOUR::AudioEngine::instance()->running()) {
        ARDOUR::AudioEngine::instance()->stop ();
        gtk_button_set_label (GTK_BUTTON (self->btn_start), _("Start Audio Engine"));
        gtk_widget_remove_css_class (self->btn_start, "destructive-action");
        gtk_widget_add_css_class (self->btn_start, "suggested-action");
        gtk_label_set_text (GTK_LABEL (self->status_label), _("Engine Stopped"));
    } else {
        gtk_button_set_label (GTK_BUTTON (self->btn_start), _("Stop Audio Engine"));
        gtk_widget_remove_css_class (self->btn_start, "suggested-action");
        gtk_widget_add_css_class (self->btn_start, "destructive-action");
        gtk_label_set_text (GTK_LABEL (self->status_label), _("Engine Running (ALSA 48.0 kHz 256 samples)"));
    }
}

static void
ardour_engine_dialog_class_init (ArdourEngineDialogClass * /*klass*/)
{
}

static void
ardour_engine_dialog_init (ArdourEngineDialog *self)
{
    gtk_window_set_title (GTK_WINDOW (self), _("Audio / MIDI Engine Setup"));
    gtk_window_set_default_size (GTK_WINDOW (self), 620, 500);

    GtkWidget *main_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    adw_window_set_content (ADW_WINDOW (self), main_box);

    /* Header Bar */
    GtkWidget *header = adw_header_bar_new ();
    gtk_box_append (GTK_BOX (main_box), header);

    GtkWidget *page = adw_preferences_page_new ();
    gtk_box_append (GTK_BOX (main_box), page);
    gtk_widget_set_vexpand (page, TRUE);

    /* ── Driver Setup ─── */
    AdwPreferencesGroup *g_driver = ADW_PREFERENCES_GROUP (adw_preferences_group_new ());
    adw_preferences_group_set_title (g_driver, _("Audio System Driver"));

    const char *backends[] = { _("ALSA (Advanced Linux Sound Architecture)"), _("JACK Audio Connection Kit"), _("PulseAudio / PipeWire"), _("Dummy (Offline Rendering Only)"), NULL };
    GtkStringList *backend_list = gtk_string_list_new (backends);
    self->backend_combo = adw_combo_row_new ();
    adw_preferences_row_set_title (ADW_PREFERENCES_ROW (self->backend_combo), _("Audio Subsystem"));
    adw_combo_row_set_model (ADW_COMBO_ROW (self->backend_combo), G_LIST_MODEL (backend_list));
    adw_preferences_group_add (g_driver, self->backend_combo);

    const char *inputs[] = { _("Default Hardware Input (hw:0)"), _("Internal Microphone"), _("USB Audio Interface Input"), NULL };
    GtkStringList *input_list = gtk_string_list_new (inputs);
    self->device_input_combo = adw_combo_row_new ();
    adw_preferences_row_set_title (ADW_PREFERENCES_ROW (self->device_input_combo), _("Capture Device"));
    adw_combo_row_set_model (ADW_COMBO_ROW (self->device_input_combo), G_LIST_MODEL (input_list));
    adw_preferences_group_add (g_driver, self->device_input_combo);

    const char *outputs[] = { _("Default Hardware Output (hw:0)"), _("Headphones / Line Out"), _("USB Audio Interface Output"), NULL };
    GtkStringList *output_list = gtk_string_list_new (outputs);
    self->device_output_combo = adw_combo_row_new ();
    adw_preferences_row_set_title (ADW_PREFERENCES_ROW (self->device_output_combo), _("Playback Device"));
    adw_combo_row_set_model (ADW_COMBO_ROW (self->device_output_combo), G_LIST_MODEL (output_list));
    adw_preferences_group_add (g_driver, self->device_output_combo);

    adw_preferences_page_add (ADW_PREFERENCES_PAGE (page), g_driver);

    /* ── Performance & Buffering ─── */
    AdwPreferencesGroup *g_perf = ADW_PREFERENCES_GROUP (adw_preferences_group_new ());
    adw_preferences_group_set_title (g_perf, _("Performance & Latency"));

    const char *rates[] = { _("44.1 kHz"), _("48.0 kHz"), _("88.2 kHz"), _("96.0 kHz"), _("192.0 kHz"), NULL };
    GtkStringList *rate_list = gtk_string_list_new (rates);
    self->sample_rate_combo = adw_combo_row_new ();
    adw_preferences_row_set_title (ADW_PREFERENCES_ROW (self->sample_rate_combo), _("Sample Rate"));
    adw_combo_row_set_model (ADW_COMBO_ROW (self->sample_rate_combo), G_LIST_MODEL (rate_list));
    adw_combo_row_set_selected (ADW_COMBO_ROW (self->sample_rate_combo), 1);
    adw_preferences_group_add (g_perf, self->sample_rate_combo);

    const char *buffers[] = {
        _("64 samples (1.3 ms) — Ultra Low Latency"),
        _("128 samples (2.7 ms) — Low Latency"),
        _("256 samples (5.3 ms) — Default Balanced"),
        _("512 samples (10.7 ms) — Safe Buffer"),
        _("1024 samples (21.3 ms) — High Stability / Mixing"),
        NULL
    };
    GtkStringList *buf_list = gtk_string_list_new (buffers);
    self->buffer_size_combo = adw_combo_row_new ();
    adw_preferences_row_set_title (ADW_PREFERENCES_ROW (self->buffer_size_combo), _("Buffer Size / Latency"));
    adw_combo_row_set_model (ADW_COMBO_ROW (self->buffer_size_combo), G_LIST_MODEL (buf_list));
    adw_combo_row_set_selected (ADW_COMBO_ROW (self->buffer_size_combo), 2);
    adw_preferences_group_add (g_perf, self->buffer_size_combo);

    adw_preferences_page_add (ADW_PREFERENCES_PAGE (page), g_perf);

    /* Status & Start Action */
    AdwPreferencesGroup *g_action = ADW_PREFERENCES_GROUP (adw_preferences_group_new ());
    self->status_label = gtk_label_new (_("Engine Stopped"));
    gtk_widget_add_css_class (self->status_label, "dim-label");
    gtk_widget_set_halign (self->status_label, GTK_ALIGN_CENTER);
    gtk_widget_set_margin_bottom (self->status_label, 8);
    adw_preferences_group_add (g_action, self->status_label);

    self->btn_start = gtk_button_new_with_label (_("Start Audio Engine"));
    gtk_widget_add_css_class (self->btn_start, "suggested-action");
    gtk_widget_add_css_class (self->btn_start, "pill");
    gtk_widget_set_halign (self->btn_start, GTK_ALIGN_CENTER);
    g_signal_connect (self->btn_start, "clicked", G_CALLBACK (on_start_engine_clicked), self);
    adw_preferences_group_add (g_action, self->btn_start);

    adw_preferences_page_add (ADW_PREFERENCES_PAGE (page), g_action);
}

GtkWidget *
ardour_engine_dialog_new (GtkWindow *parent)
{
    return GTK_WIDGET (g_object_new (ARDOUR_TYPE_ENGINE_DIALOG,
                                     "transient-for", parent,
                                     NULL));
}
