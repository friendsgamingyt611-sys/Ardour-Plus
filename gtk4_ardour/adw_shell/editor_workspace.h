/*
 * ardour4-shell: Comprehensive Multitrack Timeline Editor Workspace Header
 * Phase 3.5 — GTK4/Libadwaita migration of Ardour Plus
 */

#pragma once

#include <adwaita.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define ARDOUR_TYPE_EDITOR_WORKSPACE (ardour_editor_workspace_get_type ())
G_DECLARE_FINAL_TYPE (ArdourEditorWorkspace, ardour_editor_workspace, ARDOUR, EDITOR_WORKSPACE, GtkWidget)

/* Construction */
GtkWidget *ardour_editor_workspace_new (void);

/* Track management — core DAW flow */
void ardour_editor_workspace_add_track         (ArdourEditorWorkspace *self, const char *name, const char *type);
void ardour_editor_workspace_select_track      (ArdourEditorWorkspace *self, int index);
void ardour_editor_workspace_remove_track      (ArdourEditorWorkspace *self, int index);
void ardour_editor_workspace_rename_track      (ArdourEditorWorkspace *self, int index, const char *new_name);

/* Add Track dialog (full two-panel dialog matching legacy AddRouteDialog) */
void ardour_editor_workspace_show_add_dialog   (ArdourEditorWorkspace *self,
                                                GtkWindow             *parent,
                                                const char            *preset_type);

/* Timeline markers */
void ardour_editor_workspace_add_marker        (ArdourEditorWorkspace *self, const char *name);

/* Playhead & view */
void   ardour_editor_workspace_jump_to_position (ArdourEditorWorkspace *self, double x);
void   ardour_editor_workspace_set_zoom         (ArdourEditorWorkspace *self, double zoom_factor);
double ardour_editor_workspace_get_current_zoom (ArdourEditorWorkspace *self);
int    ardour_editor_workspace_get_selected_track (ArdourEditorWorkspace *self);

G_END_DECLS
