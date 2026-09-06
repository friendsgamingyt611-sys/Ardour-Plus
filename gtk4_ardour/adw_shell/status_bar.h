/*
 * ardour4-shell: Bottom DAW Status Bar Header
 * Phase 2 — GTK4/Libadwaita migration of Ardour Plus
 */

#pragma once

#include <adwaita.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define ARDOUR_TYPE_STATUS_BAR (ardour_status_bar_get_type ())
G_DECLARE_FINAL_TYPE (ArdourStatusBar, ardour_status_bar, ARDOUR, STATUS_BAR, GtkWidget)

GtkWidget *ardour_status_bar_new (void);

G_END_DECLS
