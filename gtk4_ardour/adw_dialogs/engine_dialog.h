/*
 * ardour4-dialogs: Audio/MIDI Engine Dialog Header
 * Phase 1 — GTK4/Libadwaita migration of Ardour Plus
 */

#pragma once

#include <adwaita.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define ARDOUR_TYPE_ENGINE_DIALOG (ardour_engine_dialog_get_type ())
G_DECLARE_FINAL_TYPE (ArdourEngineDialog, ardour_engine_dialog, ARDOUR, ENGINE_DIALOG, AdwWindow)

GtkWidget *ardour_engine_dialog_new (GtkWindow *parent);

G_END_DECLS
