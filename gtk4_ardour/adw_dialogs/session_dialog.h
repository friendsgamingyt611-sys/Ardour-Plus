/*
 * ardour4-dialogs: Session Setup Dialog Header
 * Phase 1 — GTK4/Libadwaita migration of Ardour Plus
 */

#pragma once

#include <adwaita.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define ARDOUR_TYPE_SESSION_DIALOG (ardour_session_dialog_get_type ())
G_DECLARE_FINAL_TYPE (ArdourSessionDialog, ardour_session_dialog, ARDOUR, SESSION_DIALOG, AdwWindow)

GtkWidget *ardour_session_dialog_new (GtkWindow *parent);

G_END_DECLS
