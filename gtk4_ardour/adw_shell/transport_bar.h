/*
 * ardour4-shell: Comprehensive Transport Control & Header Bar Header
 * Phase 3.5 — GTK4/Libadwaita migration of Ardour Plus
 */

#pragma once

#include <adwaita.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define ARDOUR_TYPE_TRANSPORT_BAR (ardour_transport_bar_get_type ())
G_DECLARE_FINAL_TYPE (ArdourTransportBar, ardour_transport_bar, ARDOUR, TRANSPORT_BAR, GtkWidget)

/* Construction */
GtkWidget *ardour_transport_bar_new (void);

/* State setters (called by shell or libardour engine callbacks) */
void ardour_transport_bar_set_playing   (ArdourTransportBar *self, gboolean playing);
void ardour_transport_bar_set_recording (ArdourTransportBar *self, gboolean recording);
void ardour_transport_bar_trigger_xrun  (ArdourTransportBar *self);
void ardour_transport_bar_set_solo_active (ArdourTransportBar *self, gboolean active);
void ardour_transport_bar_set_mute_active (ArdourTransportBar *self, gboolean active);

/* Public action toggles — called by GAction handlers in shell_window.cc */
void ardour_transport_bar_toggle_play    (ArdourTransportBar *self);
void ardour_transport_bar_stop           (ArdourTransportBar *self);
void ardour_transport_bar_toggle_rec     (ArdourTransportBar *self);
void ardour_transport_bar_toggle_loop    (ArdourTransportBar *self);
void ardour_transport_bar_toggle_click   (ArdourTransportBar *self);
void ardour_transport_bar_toggle_punch_in  (ArdourTransportBar *self);
void ardour_transport_bar_toggle_punch_out (ArdourTransportBar *self);
void ardour_transport_bar_midi_panic     (ArdourTransportBar *self);

G_END_DECLS
