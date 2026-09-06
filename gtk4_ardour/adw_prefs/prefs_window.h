/*
 * ardour4-prefs: Phase 0 Preferences Window
 * Copyright (C) 2026 Ardour Plus Contributors
 *
 * Standalone GTK4/Libadwaita binary that presents Ardour's preferences
 * using AdwPreferencesWindow. Reads and writes the same ardour.rc config
 * file that the GTK2 binary uses — no config format changes.
 *
 * Build: see meson.build in this directory
 * Run:   ./builddir/ardour4-prefs
 */

#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

/* ── Forward declaration of the app struct ─────────────────── */
typedef struct _ArdourPrefsApp ArdourPrefsApp;

/* ── Application entry point ───────────────────────────────── */
int  ardour_prefs_run  (int argc, char **argv);

/* ── Window builder — called once app is activated ─────────── */
GtkWidget *ardour_prefs_window_new (AdwApplication *app);

/* ── Page builders (one per preferences category) ──────────── */
AdwPreferencesPage *ardour_prefs_page_general     (void);
AdwPreferencesPage *ardour_prefs_page_appearance  (void);
AdwPreferencesPage *ardour_prefs_page_editor      (void);
AdwPreferencesPage *ardour_prefs_page_mixer       (void);
AdwPreferencesPage *ardour_prefs_page_transport   (void);
AdwPreferencesPage *ardour_prefs_page_midi        (void);
AdwPreferencesPage *ardour_prefs_page_plugins     (void);
AdwPreferencesPage *ardour_prefs_page_surfaces    (void);
AdwPreferencesPage *ardour_prefs_page_video       (void);
AdwPreferencesPage *ardour_prefs_page_performance (void);

G_END_DECLS
