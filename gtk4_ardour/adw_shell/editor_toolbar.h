/*
 * ardour4-shell: Secondary Editor Control Strip Header
 * Phase 3.5 — GTK4/Libadwaita migration of Ardour Plus
 */

#pragma once

#include <adwaita.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define ARDOUR_TYPE_EDITOR_TOOLBAR (ardour_editor_toolbar_get_type ())
G_DECLARE_FINAL_TYPE (ArdourEditorToolbar, ardour_editor_toolbar, ARDOUR, EDITOR_TOOLBAR, GtkWidget)

GtkWidget *ardour_editor_toolbar_new (void);

/* Inject the workspace widget reference so toolbar zoom/tool buttons
 * can call ardour_editor_workspace_set_zoom() directly. Called by
 * shell_window.cc after constructing both widgets. */
void ardour_editor_toolbar_set_workspace (ArdourEditorToolbar *self,
                                          GtkWidget           *workspace);

G_END_DECLS
