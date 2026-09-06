/*
 * ardour4-shell: Main DAW Application Window Header
 * Phase 2 — GTK4/Libadwaita migration of Ardour Plus
 */

#pragma once

#include <adwaita.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define ARDOUR_TYPE_SHELL_WINDOW (ardour_shell_window_get_type ())
G_DECLARE_FINAL_TYPE (ArdourShellWindow, ardour_shell_window, ARDOUR, SHELL_WINDOW, AdwApplicationWindow)

GtkWidget *ardour_shell_window_new (GtkApplication *app);

G_END_DECLS
