/*
 * ardour4-shell: Comprehensive Multitrack Timeline Editor Workspace Implementation
 * Phase 3.5 — GTK4/Libadwaita migration of Ardour Plus
 *
 * Phase 3.5 changes:
 * - Starts with empty session (no placeholder tracks)
 * - Empty track lanes by default (no fake sine wave unless track has regions)
 * - Synchronized vertical scrolling (track headers share vadjustment with canvas)
 * - Real two-panel Add Track / Bus dialog (GtkPaned + GtkListBox + settings form)
 * - Right-click context menu on track headers (Rename, Duplicate, Remove)
 * - Name dialog before adding markers
 * - ardour_editor_workspace_remove_track() / rename_track() public API
 */

#include "editor_workspace.h"
#include <glib/gi18n.h>
#include <math.h>
#include <vector>
#include <string>

/* ════════════════════════════════════════════════════════════════════════════
 * DATA STRUCTURES
 * ════════════════════════════════════════════════════════════════════════════ */

struct TrackData {
    std::string name;
    std::string type;        /* "audio", "midi", "bus", "vca" */
    std::string color_hex;   /* "#rrggbb" color accent */
    bool is_active;
    bool is_muted;
    bool is_soloed;
    bool is_rec_armed;
    double fader_db;
    double trim_db;
    double pan_pos;          /* -1.0 (L) to +1.0 (R) */
    bool has_regions;        /* true = draw placeholder waveform blocks */
    int height_px;           /* track lane + header height in pixels */
};

struct MarkerData {
    std::string name;
    double position_x;
    std::string bbt_str;
};

struct _ArdourEditorWorkspace {
    GtkWidget parent_instance;

    /* Top-level layout */
    GtkWidget *main_hbox;

    /* Left inspector panel */
    GtkWidget *inspector_box;
    GtkWidget *lbl_inspector_title;
    GtkWidget *fader_scale;
    GtkWidget *pan_scale;
    GtkWidget *lbl_fader_readout;
    GtkWidget *meter_bar;

    /* Center canvas area */
    GtkWidget *canvas_vbox;
    GtkWidget *ruler_area;

    /* Track headers scroll (left of canvas, shares vadjustment) */
    GtkWidget *headers_scroll;
    GtkWidget *tracks_vbox;

    /* Timeline canvas scroll (right, horizontal only + shared vertical) */
    GtkWidget *timeline_scroll;
    GtkWidget *canvas_draw_area;

    /* Right sidebar */
    GtkWidget *sidebar_box;

    /* Data models */
    std::vector<TrackData>   tracks;
    std::vector<MarkerData>  markers;
    int    selected_track_index;
    double playhead_x;
    double zoom_factor;
};

G_DEFINE_TYPE (ArdourEditorWorkspace, ardour_editor_workspace, GTK_TYPE_WIDGET)

/* Forward declarations */
static void rebuild_track_headers         (ArdourEditorWorkspace *self);
static void update_inspector_for_selected (ArdourEditorWorkspace *self);
static void rebuild_sidebar_markers       (ArdourEditorWorkspace *self);
static int  total_tracks_height           (ArdourEditorWorkspace *self);

/* ════════════════════════════════════════════════════════════════════════════
 * SECTION 1 — CANVAS DRAW (empty lanes + regions only if has_regions=true)
 * ════════════════════════════════════════════════════════════════════════════ */

static int
total_tracks_height (ArdourEditorWorkspace *self)
{
    int h = 0;
    for (const auto &tr : self->tracks)
        h += tr.height_px;
    return MAX (h, 300); /* minimum canvas height */
}

static void
parse_color_hex (const char *hex, double *r, double *g, double *b)
{
    /* Parse "#rrggbb" → [0,1] doubles */
    unsigned int rv = 0, gv = 0, bv = 0;
    if (hex && hex[0] == '#') {
        sscanf (hex + 1, "%02x%02x%02x", &rv, &gv, &bv);
    }
    *r = rv / 255.0;
    *g = gv / 255.0;
    *b = bv / 255.0;
}

static void
on_canvas_draw (GtkDrawingArea * /*da*/, cairo_t *cr, int width, int height, gpointer user_data)
{
    ArdourEditorWorkspace *self = ARDOUR_EDITOR_WORKSPACE (user_data);

    /* Dark canvas background */
    cairo_set_source_rgb (cr, 0.11, 0.12, 0.14);
    cairo_paint (cr);

    /* Beat grid — vertical lines */
    double step = 40.0 * self->zoom_factor;
    if (step < 8.0) step = 8.0;

    cairo_set_source_rgba (cr, 0.22, 0.25, 0.29, 0.5);
    cairo_set_line_width (cr, 1.0);
    for (double x = 0; x < width; x += step) {
        cairo_move_to (cr, x + 0.5, 0);
        cairo_line_to (cr, x + 0.5, height);
        cairo_stroke (cr);
    }
    /* Major bar lines (every 4 beats) */
    cairo_set_source_rgba (cr, 0.32, 0.37, 0.43, 0.7);
    for (double x = 0; x < width; x += step * 4) {
        cairo_move_to (cr, x + 0.5, 0);
        cairo_line_to (cr, x + 0.5, height);
        cairo_stroke (cr);
    }

    /* Render each track lane */
    int y = 0;
    for (size_t i = 0; i < self->tracks.size (); ++i) {
        const auto &tr = self->tracks[i];
        int th = tr.height_px;

        /* Lane background — darker for inactive tracks */
        double br = self->selected_track_index == (int)i ? 0.17 : 0.13;
        cairo_set_source_rgb (cr, br, br + 0.01, br + 0.02);
        cairo_rectangle (cr, 0, y, width, th);
        cairo_fill (cr);

        /* Mute dim overlay */
        if (tr.is_muted) {
            cairo_set_source_rgba (cr, 0.04, 0.04, 0.04, 0.65);
            cairo_rectangle (cr, 0, y, width, th);
            cairo_fill (cr);
        }

        /* Color accent left border stripe (3 px) */
        double cr_, cg, cb;
        parse_color_hex (tr.color_hex.c_str (), &cr_, &cg, &cb);
        cairo_set_source_rgb (cr, cr_, cg, cb);
        cairo_rectangle (cr, 0, y, 3, th);
        cairo_fill (cr);

        /* ── Draw placeholder waveform/notes only if track has regions ── */
        if (tr.has_regions) {
            if (tr.type == "audio") {
                /* Region block */
                double rx = 20.0 * self->zoom_factor;
                double rw = 480.0 * self->zoom_factor;
                cairo_set_source_rgba (cr, 0.18, 0.30, 0.22, 0.85);
                cairo_rectangle (cr, rx, y + 3, rw, th - 6);
                cairo_fill (cr);

                /* Region border */
                cairo_set_source_rgb (cr, cr_, cg, cb);
                cairo_set_line_width (cr, 1.0);
                cairo_rectangle (cr, rx, y + 3, rw, th - 6);
                cairo_stroke (cr);

                /* Stereo waveform inside region */
                cairo_set_source_rgb (cr, cr_ * 1.3, cg * 1.2, cb * 1.3);
                cairo_set_line_width (cr, 1.0);
                double mid = y + th / 2.0;
                for (double px = rx + 4; px < rx + rw - 4; px += 2.0) {
                    double amp = (sin (px * 0.055) * 0.42 + cos (px * 0.11) * 0.28) * ((th - 12) / 4.5);
                    cairo_move_to (cr, px, mid - amp);
                    cairo_line_to (cr, px, mid + amp);
                    cairo_stroke (cr);
                }

            } else if (tr.type == "midi") {
                /* MIDI region block */
                double rx = 20.0 * self->zoom_factor;
                double rw = 360.0 * self->zoom_factor;
                cairo_set_source_rgba (cr, 0.18, 0.20, 0.28, 0.85);
                cairo_rectangle (cr, rx, y + 3, rw, th - 6);
                cairo_fill (cr);
                cairo_set_source_rgb (cr, cr_, cg, cb);
                cairo_rectangle (cr, rx, y + 3, rw, th - 6);
                cairo_stroke (cr);

                /* Piano roll note grid lines */
                cairo_set_source_rgba (cr, 0.22, 0.25, 0.30, 0.45);
                cairo_set_line_width (cr, 0.5);
                for (int py = y + 6; py < y + th - 3; py += 8) {
                    cairo_move_to (cr, rx, py + 0.5);
                    cairo_line_to (cr, rx + rw, py + 0.5);
                    cairo_stroke (cr);
                }

                /* MIDI notes */
                cairo_set_source_rgb (cr, cr_, cg, cb);
                double note_data[][4] = {
                    { 40, y + 18.0, 32, 5 }, { 80, y + 30.0, 24, 5 }, { 120, y + 22.0, 38, 5 },
                    { 165, y + 46.0, 28, 5 }, { 205, y + 14.0, 46, 5 }, { 260, y + 38.0, 32, 5 }
                };
                for (int n = 0; n < 6; n++) {
                    cairo_rectangle (cr,
                        rx + note_data[n][0] * self->zoom_factor,
                        note_data[n][1],
                        note_data[n][2] * self->zoom_factor,
                        note_data[n][3]);
                    cairo_fill (cr);
                }
            }
        }

        /* Punch In/Out overlay (always rendered) */
        double punch_in_x  = 240.0 * self->zoom_factor;
        double punch_out_x = 400.0 * self->zoom_factor;
        if (i == 0) { /* draw punch region once at canvas level */
            cairo_set_source_rgba (cr, 0.88, 0.20, 0.20, 0.08);
            cairo_rectangle (cr, punch_in_x, 0, punch_out_x - punch_in_x, height);
            cairo_fill (cr);
        }

        /* Lane bottom divider */
        cairo_set_source_rgb (cr, 0.20, 0.22, 0.26);
        cairo_set_line_width (cr, 1.0);
        cairo_move_to (cr, 0, y + th + 0.5);
        cairo_line_to (cr, width, y + th + 0.5);
        cairo_stroke (cr);

        y += th;
    }

    /* Punch boundary lines */
    {
        double punch_in_x  = 240.0 * self->zoom_factor;
        double punch_out_x = 400.0 * self->zoom_factor;
        cairo_set_source_rgba (cr, 0.92, 0.24, 0.24, 0.8);
        cairo_set_line_width (cr, 1.5);
        cairo_move_to (cr, punch_in_x + 0.5, 0);
        cairo_line_to (cr, punch_in_x + 0.5, height);
        cairo_stroke (cr);
        cairo_move_to (cr, punch_out_x + 0.5, 0);
        cairo_line_to (cr, punch_out_x + 0.5, height);
        cairo_stroke (cr);
    }

    /* Red playhead line */
    double ph_x = self->playhead_x * self->zoom_factor;
    cairo_set_source_rgb (cr, 1.0, 0.20, 0.20);
    cairo_set_line_width (cr, 2.0);
    cairo_move_to (cr, ph_x + 0.5, 0);
    cairo_line_to (cr, ph_x + 0.5, height);
    cairo_stroke (cr);

    /* Playhead triangle cap */
    cairo_move_to (cr, ph_x - 5.0, 0);
    cairo_line_to (cr, ph_x + 6.0, 0);
    cairo_line_to (cr, ph_x + 0.5, 9.0);
    cairo_close_path (cr);
    cairo_fill (cr);
}

static void
on_canvas_click (GtkGestureClick * /*g*/, int /*np*/, double x, double /*y*/, gpointer user_data)
{
    ArdourEditorWorkspace *self = ARDOUR_EDITOR_WORKSPACE (user_data);
    ardour_editor_workspace_jump_to_position (self, x / self->zoom_factor);
}

/* ════════════════════════════════════════════════════════════════════════════
 * SECTION 2 — RULER DRAW
 * ════════════════════════════════════════════════════════════════════════════ */

static void
on_ruler_draw (GtkDrawingArea * /*da*/, cairo_t *cr, int width, int /*h*/, gpointer user_data)
{
    ArdourEditorWorkspace *self = ARDOUR_EDITOR_WORKSPACE (user_data);

    cairo_set_source_rgb (cr, 0.14, 0.16, 0.18);
    cairo_paint (cr);

    cairo_set_source_rgb (cr, 0.24, 0.28, 0.32);
    cairo_set_line_width (cr, 1.0);
    int ruler_h = 16;
    for (int r = 1; r <= 6; r++) {
        cairo_move_to (cr, 0, r * ruler_h + 0.5);
        cairo_line_to (cr, width, r * ruler_h + 0.5);
        cairo_stroke (cr);
    }

    cairo_select_font_face (cr, "monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (cr, 10.0);

    /* Timecode row */
    cairo_set_source_rgb (cr, 0.68, 0.73, 0.78);
    cairo_move_to (cr, 8, 12);
    cairo_show_text (cr, "Timecode   00:00:00:00         00:02:00:00         00:04:00:00");

    /* Bars:Beats row */
    cairo_move_to (cr, 8, 28);
    cairo_show_text (cr, "Bars:Beats   1     5     9     13    17    21    25    29    33");

    /* Tempo / Time Sig row */
    cairo_set_source_rgb (cr, 0.30, 0.82, 0.92);
    cairo_move_to (cr, 8, 44);
    cairo_show_text (cr, "Tempo / TS  [ 4/4 ]  120.000 BPM");

    /* Loop range */
    double step = 40.0 * self->zoom_factor;
    cairo_set_source_rgb (cr, 0.22, 0.58, 0.82);
    cairo_rectangle (cr, 80 * self->zoom_factor, 50, 240 * self->zoom_factor, 12);
    cairo_fill (cr);
    cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
    cairo_move_to (cr, 84 * self->zoom_factor, 60);
    cairo_show_text (cr, "Loop Range");

    /* Punch range */
    cairo_set_source_rgb (cr, 0.82, 0.24, 0.24);
    cairo_rectangle (cr, 240 * self->zoom_factor, 50, 160 * self->zoom_factor, 12);
    cairo_fill (cr);
    cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
    cairo_move_to (cr, 244 * self->zoom_factor, 60);
    cairo_show_text (cr, "Punch");

    /* Start/End markers */
    cairo_set_source_rgb (cr, 0.28, 0.82, 0.38);
    cairo_rectangle (cr, 18 * self->zoom_factor, 66, 36, 12);
    cairo_fill (cr);
    cairo_set_source_rgb (cr, 0.08, 0.08, 0.08);
    cairo_move_to (cr, 20 * self->zoom_factor, 76);
    cairo_show_text (cr, "start");

    cairo_set_source_rgb (cr, 0.28, 0.82, 0.38);
    cairo_rectangle (cr, 560 * self->zoom_factor, 66, 30, 12);
    cairo_fill (cr);
    cairo_set_source_rgb (cr, 0.08, 0.08, 0.08);
    cairo_move_to (cr, 562 * self->zoom_factor, 76);
    cairo_show_text (cr, "end");

    /* Dynamic markers */
    for (const auto &m : self->markers) {
        double mx = m.position_x * self->zoom_factor;
        cairo_set_source_rgb (cr, 0.72, 0.42, 0.92);
        cairo_rectangle (cr, mx, 82, 70, 12);
        cairo_fill (cr);
        cairo_set_source_rgb (cr, 1.0, 1.0, 1.0);
        cairo_move_to (cr, mx + 3, 92);
        cairo_show_text (cr, m.name.c_str ());
    }

    /* Playhead cap on ruler */
    double ph_x = self->playhead_x * self->zoom_factor;
    cairo_set_source_rgb (cr, 1.0, 0.20, 0.20);
    cairo_move_to (cr, ph_x - 5.0, 96);
    cairo_line_to (cr, ph_x + 6.0, 96);
    cairo_line_to (cr, ph_x + 0.5, 88.0);
    cairo_close_path (cr);
    cairo_fill (cr);

    (void) step; /* suppress unused warning */
}

static void
on_ruler_click (GtkGestureClick * /*g*/, int /*np*/, double x, double /*y*/, gpointer user_data)
{
    ArdourEditorWorkspace *self = ARDOUR_EDITOR_WORKSPACE (user_data);
    ardour_editor_workspace_jump_to_position (self, x / self->zoom_factor);
}

/* ════════════════════════════════════════════════════════════════════════════
 * SECTION 3 — TRACK HEADER WIDGETS
 * ════════════════════════════════════════════════════════════════════════════ */

static void
on_track_header_clicked (GtkGestureClick * /*g*/, int /*np*/, double /*x*/, double /*y*/, gpointer user_data)
{
    GtkWidget *box = GTK_WIDGET (user_data);
    ArdourEditorWorkspace *self = ARDOUR_EDITOR_WORKSPACE (g_object_get_data (G_OBJECT (box), "workspace"));
    int idx = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (box), "track_index"));
    ardour_editor_workspace_select_track (self, idx);
}

static void
on_track_mute_toggled (GtkButton *button, gpointer user_data)
{
    GtkWidget *box = GTK_WIDGET (user_data);
    ArdourEditorWorkspace *self = ARDOUR_EDITOR_WORKSPACE (g_object_get_data (G_OBJECT (box), "workspace"));
    int idx = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (box), "track_index"));
    if (idx < 0 || idx >= (int)self->tracks.size ()) return;
    self->tracks[idx].is_muted = !self->tracks[idx].is_muted;
    if (self->tracks[idx].is_muted)
        gtk_widget_add_css_class    (GTK_WIDGET (button), "destructive-action");
    else
        gtk_widget_remove_css_class (GTK_WIDGET (button), "destructive-action");
    gtk_widget_queue_draw (self->canvas_draw_area);
}

static void
on_track_solo_toggled (GtkButton *button, gpointer user_data)
{
    GtkWidget *box = GTK_WIDGET (user_data);
    ArdourEditorWorkspace *self = ARDOUR_EDITOR_WORKSPACE (g_object_get_data (G_OBJECT (box), "workspace"));
    int idx = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (box), "track_index"));
    if (idx < 0 || idx >= (int)self->tracks.size ()) return;
    self->tracks[idx].is_soloed = !self->tracks[idx].is_soloed;
    if (self->tracks[idx].is_soloed)
        gtk_widget_add_css_class    (GTK_WIDGET (button), "suggested-action");
    else
        gtk_widget_remove_css_class (GTK_WIDGET (button), "suggested-action");
    gtk_widget_queue_draw (self->canvas_draw_area);
}

static void
on_track_rec_toggled (GtkButton *button, gpointer user_data)
{
    GtkWidget *box = GTK_WIDGET (user_data);
    ArdourEditorWorkspace *self = ARDOUR_EDITOR_WORKSPACE (g_object_get_data (G_OBJECT (box), "workspace"));
    int idx = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (box), "track_index"));
    if (idx < 0 || idx >= (int)self->tracks.size ()) return;
    self->tracks[idx].is_rec_armed = !self->tracks[idx].is_rec_armed;
    if (self->tracks[idx].is_rec_armed)
        gtk_widget_add_css_class    (GTK_WIDGET (button), "destructive-action");
    else
        gtk_widget_remove_css_class (GTK_WIDGET (button), "destructive-action");
    gtk_widget_queue_draw (self->canvas_draw_area);
}

/* ── Right-click context menu ────────────────────────────────────────────── */

typedef struct {
    ArdourEditorWorkspace *workspace;
    int index;
    GtkWidget *popover;
} TrackContextData;

static void
on_context_rename_clicked (GtkButton * /*btn*/, gpointer user_data)
{
    TrackContextData *ctx = (TrackContextData *) user_data;
    gtk_popover_popdown (GTK_POPOVER (ctx->popover));

    /* Show rename dialog */
    GtkWindow *parent = GTK_WINDOW (gtk_widget_get_root (GTK_WIDGET (ctx->workspace)));
    GtkWidget *dialog = gtk_dialog_new_with_buttons (
        _("Rename Track"), parent,
        (GtkDialogFlags)(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
        _("Cancel"), GTK_RESPONSE_CANCEL,
        _("Rename"),  GTK_RESPONSE_ACCEPT,
        NULL);

    GtkWidget *content = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
    gtk_box_set_spacing (GTK_BOX (content), 8);
    gtk_widget_set_margin_start  (content, 12);
    gtk_widget_set_margin_end    (content, 12);
    gtk_widget_set_margin_top    (content, 8);
    gtk_widget_set_margin_bottom (content, 8);

    GtkWidget *lbl = gtk_label_new (_("New track name:"));
    gtk_widget_set_halign (lbl, GTK_ALIGN_START);
    gtk_box_append (GTK_BOX (content), lbl);

    GtkWidget *entry = gtk_entry_new ();
    if (ctx->index >= 0 && ctx->index < (int)ctx->workspace->tracks.size ())
        gtk_editable_set_text (GTK_EDITABLE (entry), ctx->workspace->tracks[ctx->index].name.c_str ());
    gtk_entry_set_activates_default (GTK_ENTRY (entry), TRUE);
    gtk_box_append (GTK_BOX (content), entry);

    gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_ACCEPT);
    gtk_widget_show (dialog);

    g_signal_connect (dialog, "response", G_CALLBACK (+[](GtkDialog *d, int resp, gpointer ud) {
        TrackContextData *c = (TrackContextData *) ud;
        if (resp == GTK_RESPONSE_ACCEPT) {
            GtkWidget *content = gtk_dialog_get_content_area (d);
            GtkWidget *entry = gtk_widget_get_last_child (content);
            const char *text = gtk_editable_get_text (GTK_EDITABLE (entry));
            if (text && text[0] != '\0')
                ardour_editor_workspace_rename_track (c->workspace, c->index, text);
        }
        gtk_window_destroy (GTK_WINDOW (d));
        g_free (c);
    }), ctx);
}

static void
on_context_duplicate_clicked (GtkButton * /*btn*/, gpointer user_data)
{
    TrackContextData *ctx = (TrackContextData *) user_data;
    gtk_popover_popdown (GTK_POPOVER (ctx->popover));
    ArdourEditorWorkspace *self = ctx->workspace;
    int idx = ctx->index;
    g_free (ctx);

    if (idx < 0 || idx >= (int)self->tracks.size ()) return;
    TrackData copy = self->tracks[idx];
    copy.name = copy.name + " (copy)";
    copy.is_active = false;
    self->tracks.insert (self->tracks.begin () + idx + 1, copy);
    rebuild_track_headers (self);
    gtk_widget_queue_draw (self->canvas_draw_area);
}

static void
on_context_remove_clicked (GtkButton * /*btn*/, gpointer user_data)
{
    TrackContextData *ctx = (TrackContextData *) user_data;
    gtk_popover_popdown (GTK_POPOVER (ctx->popover));
    ArdourEditorWorkspace *self = ctx->workspace;
    int idx = ctx->index;
    g_free (ctx);

    ardour_editor_workspace_remove_track (self, idx);
}

static void
show_track_context_menu (ArdourEditorWorkspace *self, GtkWidget *box, int index,
                          double x, double y)
{
    GtkWidget *popover = gtk_popover_new ();
    gtk_widget_set_parent (popover, box);

    GdkRectangle rect = { (int)x, (int)y, 1, 1 };
    gtk_popover_set_pointing_to (GTK_POPOVER (popover), &rect);
    gtk_popover_set_has_arrow (GTK_POPOVER (popover), FALSE);

    GtkWidget *menu_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
    gtk_widget_set_margin_top    (menu_box, 4);
    gtk_widget_set_margin_bottom (menu_box, 4);
    gtk_widget_set_margin_start  (menu_box, 4);
    gtk_widget_set_margin_end    (menu_box, 4);
    gtk_popover_set_child (GTK_POPOVER (popover), menu_box);

    const char *items[] = { _("Rename Track…"), _("Duplicate Track"), _("Remove Track") };
    GCallback   cbs[]   = {
        G_CALLBACK (on_context_rename_clicked),
        G_CALLBACK (on_context_duplicate_clicked),
        G_CALLBACK (on_context_remove_clicked)
    };

    for (int i = 0; i < 3; i++) {
        TrackContextData *ctx = g_new (TrackContextData, 1);
        ctx->workspace = self;
        ctx->index     = index;
        ctx->popover   = popover;

        GtkWidget *btn = gtk_button_new_with_label (items[i]);
        gtk_widget_add_css_class (btn, "flat");
        gtk_widget_set_halign (btn, GTK_ALIGN_FILL);
        if (i == 2)
            gtk_widget_add_css_class (btn, "destructive-action");
        g_signal_connect (btn, "clicked", cbs[i], ctx);
        gtk_box_append (GTK_BOX (menu_box), btn);
    }

    gtk_popover_popup (GTK_POPOVER (popover));
}

static void
on_track_secondary_click (GtkGestureClick * /*g*/, int /*np*/, double x, double y, gpointer user_data)
{
    GtkWidget *box = GTK_WIDGET (user_data);
    ArdourEditorWorkspace *self = ARDOUR_EDITOR_WORKSPACE (g_object_get_data (G_OBJECT (box), "workspace"));
    int idx = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (box), "track_index"));
    show_track_context_menu (self, box, idx, x, y);
}

/* ── Build one track header row ──────────────────────────────────────────── */

static GtkWidget *
create_interactive_track_header (ArdourEditorWorkspace *self, int index)
{
    const auto &tr = self->tracks[index];
    double cr_, cg, cb;
    parse_color_hex (tr.color_hex.c_str (), &cr_, &cg, &cb);

    GtkWidget *box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 3);
    gtk_widget_set_size_request (box, 200, tr.height_px);

    if (tr.is_active)
        gtk_widget_add_css_class (box, "card");

    g_object_set_data (G_OBJECT (box), "workspace",   self);
    g_object_set_data (G_OBJECT (box), "track_index", GINT_TO_POINTER (index));

    /* Left-click to select */
    GtkGesture *click = gtk_gesture_click_new ();
    gtk_gesture_single_set_button (GTK_GESTURE_SINGLE (click), GDK_BUTTON_PRIMARY);
    g_signal_connect (click, "pressed", G_CALLBACK (on_track_header_clicked), box);
    gtk_widget_add_controller (box, GTK_EVENT_CONTROLLER (click));

    /* Right-click context menu */
    GtkGesture *rclick = gtk_gesture_click_new ();
    gtk_gesture_single_set_button (GTK_GESTURE_SINGLE (rclick), GDK_BUTTON_SECONDARY);
    g_signal_connect (rclick, "pressed", G_CALLBACK (on_track_secondary_click), box);
    gtk_widget_add_controller (box, GTK_EVENT_CONTROLLER (rclick));

    /* Color accent stripe (4 px) */
    GtkWidget *stripe = gtk_drawing_area_new ();
    gtk_widget_set_size_request (stripe, 4, tr.height_px);
    gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (stripe),
        +[](GtkDrawingArea *, cairo_t *c, int, int h, gpointer ud) {
            double r, g, b;
            parse_color_hex ((const char *) ud, &r, &g, &b);
            cairo_set_source_rgb (c, r, g, b);
            cairo_paint (c);
        },
        g_strdup (tr.color_hex.c_str ()), g_free);
    gtk_box_append (GTK_BOX (box), stripe);

    /* Controls vbox */
    GtkWidget *vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
    gtk_widget_set_margin_start  (vbox, 3);
    gtk_widget_set_margin_end    (vbox, 3);
    gtk_widget_set_margin_top    (vbox, 2);
    gtk_widget_set_margin_bottom (vbox, 2);
    gtk_widget_set_hexpand (vbox, TRUE);

    /* Row 1: Name + M/S/R buttons */
    GtkWidget *r1 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 3);

    GtkWidget *lbl_name = gtk_label_new (tr.name.c_str ());
    gtk_widget_add_css_class (lbl_name, "bold");
    gtk_widget_set_hexpand  (lbl_name, TRUE);
    gtk_widget_set_halign   (lbl_name, GTK_ALIGN_START);
    gtk_label_set_ellipsize (GTK_LABEL (lbl_name), PANGO_ELLIPSIZE_END);
    gtk_box_append (GTK_BOX (r1), lbl_name);

    GtkWidget *grp_ms = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_add_css_class (grp_ms, "linked");

    GtkWidget *btn_m = gtk_button_new_with_label ("M");
    gtk_widget_add_css_class (btn_m, "daw-btn-compact");
    if (tr.is_muted) gtk_widget_add_css_class (btn_m, "destructive-action");
    g_signal_connect (btn_m, "clicked", G_CALLBACK (on_track_mute_toggled), box);
    gtk_box_append (GTK_BOX (grp_ms), btn_m);

    GtkWidget *btn_s = gtk_button_new_with_label ("S");
    gtk_widget_add_css_class (btn_s, "daw-btn-compact");
    if (tr.is_soloed) gtk_widget_add_css_class (btn_s, "suggested-action");
    g_signal_connect (btn_s, "clicked", G_CALLBACK (on_track_solo_toggled), box);
    gtk_box_append (GTK_BOX (grp_ms), btn_s);

    if (tr.type != "bus" && tr.type != "vca") {
        GtkWidget *btn_rec = gtk_button_new_with_label ("⏺");
        gtk_widget_add_css_class (btn_rec, "daw-btn-compact");
        if (tr.is_rec_armed) gtk_widget_add_css_class (btn_rec, "destructive-action");
        g_signal_connect (btn_rec, "clicked", G_CALLBACK (on_track_rec_toggled), box);
        gtk_box_append (GTK_BOX (grp_ms), btn_rec);
    }

    gtk_box_append (GTK_BOX (r1), grp_ms);
    gtk_box_append (GTK_BOX (vbox), r1);

    /* Row 2: type label + fader/pan readout */
    GtkWidget *r2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 4);

    char type_buf[32];
    g_snprintf (type_buf, sizeof(type_buf), "[ %s ]", tr.type.c_str ());
    GtkWidget *lbl_type = gtk_label_new (type_buf);
    gtk_widget_add_css_class (lbl_type, "dim-label");
    gtk_box_append (GTK_BOX (r2), lbl_type);

    if (tr.type != "vca") {
        char f_buf[48];
        g_snprintf (f_buf, sizeof(f_buf), "%.1f dB  Pan: %.0f",
                    tr.fader_db, tr.pan_pos * 100.0);
        GtkWidget *lbl_fader = gtk_label_new (f_buf);
        gtk_widget_add_css_class (lbl_fader, "monospace");
        gtk_widget_add_css_class (lbl_fader, "dim-label");
        gtk_box_append (GTK_BOX (r2), lbl_fader);
    }

    gtk_box_append (GTK_BOX (vbox), r2);
    gtk_box_append (GTK_BOX (box), vbox);

    return box;
}

static void
rebuild_track_headers (ArdourEditorWorkspace *self)
{
    /* Clear all children */
    GtkWidget *child = gtk_widget_get_first_child (self->tracks_vbox);
    while (child) {
        GtkWidget *next = gtk_widget_get_next_sibling (child);
        gtk_widget_unparent (child);
        child = next;
    }

    for (size_t i = 0; i < self->tracks.size (); ++i) {
        GtkWidget *th = create_interactive_track_header (self, (int)i);
        gtk_box_append (GTK_BOX (self->tracks_vbox), th);
    }

    /* Add Track button at the bottom */
    GtkWidget *btn_add = gtk_button_new_with_label (_("+ Add Track / Bus…"));
    gtk_widget_add_css_class (btn_add, "daw-btn-compact");
    gtk_widget_set_margin_top (btn_add, 4);
    g_signal_connect_swapped (btn_add, "clicked", G_CALLBACK (+[](ArdourEditorWorkspace *ws) {
        GtkWindow *parent = GTK_WINDOW (gtk_widget_get_root (GTK_WIDGET (ws)));
        ardour_editor_workspace_show_add_dialog (ws, parent, "Audio Tracks");
    }), self);
    gtk_box_append (GTK_BOX (self->tracks_vbox), btn_add);

    /* Update canvas size request to match total track height */
    gtk_widget_set_size_request (self->canvas_draw_area, -1, total_tracks_height (self));
}

/* ════════════════════════════════════════════════════════════════════════════
 * SECTION 4 — INSPECTOR PANEL
 * ════════════════════════════════════════════════════════════════════════════ */

static void
on_fader_changed (GtkRange *range, gpointer user_data)
{
    ArdourEditorWorkspace *self = ARDOUR_EDITOR_WORKSPACE (user_data);
    int idx = self->selected_track_index;
    if (idx < 0 || idx >= (int)self->tracks.size ()) return;
    double val = gtk_range_get_value (range);
    self->tracks[idx].fader_db = val;
    char buf[64];
    g_snprintf (buf, sizeof(buf), "Fader: %.2f dB\nTrim:  %.2f dB", val, self->tracks[idx].trim_db);
    gtk_label_set_text (GTK_LABEL (self->lbl_fader_readout), buf);
    double pct = CLAMP ((val + 60.0) / 66.0, 0.0, 1.0);
    gtk_level_bar_set_value (GTK_LEVEL_BAR (self->meter_bar), pct);
}

static void
on_pan_changed (GtkRange *range, gpointer user_data)
{
    ArdourEditorWorkspace *self = ARDOUR_EDITOR_WORKSPACE (user_data);
    int idx = self->selected_track_index;
    if (idx < 0 || idx >= (int)self->tracks.size ()) return;
    self->tracks[idx].pan_pos = gtk_range_get_value (range);
}

static void
update_inspector_for_selected (ArdourEditorWorkspace *self)
{
    int idx = self->selected_track_index;
    if (idx < 0 || idx >= (int)self->tracks.size ()) {
        gtk_label_set_text (GTK_LABEL (self->lbl_inspector_title), _("No track selected"));
        return;
    }
    const auto &tr = self->tracks[idx];
    char title_buf[128];
    g_snprintf (title_buf, sizeof(title_buf), "%s", tr.name.c_str ());
    gtk_label_set_text (GTK_LABEL (self->lbl_inspector_title), title_buf);
    gtk_range_set_value (GTK_RANGE (self->fader_scale), tr.fader_db);
    gtk_range_set_value (GTK_RANGE (self->pan_scale), tr.pan_pos);
    char buf[64];
    g_snprintf (buf, sizeof(buf), "Fader: %.2f dB\nTrim:  %.2f dB", tr.fader_db, tr.trim_db);
    gtk_label_set_text (GTK_LABEL (self->lbl_fader_readout), buf);
    double pct = CLAMP ((tr.fader_db + 60.0) / 66.0, 0.0, 1.0);
    gtk_level_bar_set_value (GTK_LEVEL_BAR (self->meter_bar), pct);
}

static GtkWidget *
create_track_inspector_panel (ArdourEditorWorkspace *self)
{
    GtkWidget *vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 4);
    gtk_widget_set_size_request (vbox, 220, -1);
    gtk_widget_set_hexpand (vbox, FALSE);
    gtk_widget_add_css_class (vbox, "card");

    /* Title */
    self->lbl_inspector_title = gtk_label_new (_("No track selected"));
    gtk_widget_add_css_class (self->lbl_inspector_title, "bold");
    gtk_widget_set_margin_top  (self->lbl_inspector_title, 6);
    gtk_widget_set_margin_start (self->lbl_inspector_title, 6);
    gtk_widget_set_halign (self->lbl_inspector_title, GTK_ALIGN_START);
    gtk_label_set_ellipsize (GTK_LABEL (self->lbl_inspector_title), PANGO_ELLIPSIZE_END);
    gtk_box_append (GTK_BOX (vbox), self->lbl_inspector_title);

    /* Routing toggles */
    GtkWidget *grp_route = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_widget_add_css_class (grp_route, "linked");
    gtk_widget_set_halign (grp_route, GTK_ALIGN_CENTER);
    const char *route_labels[] = { "Disk", "In", "Iso", "Lock" };
    for (int i = 0; i < 4; i++) {
        GtkWidget *b = gtk_button_new_with_label (route_labels[i]);
        gtk_widget_add_css_class (b, "daw-btn-compact");
        if (i == 0) gtk_widget_add_css_class (b, "suggested-action");
        gtk_box_append (GTK_BOX (grp_route), b);
    }
    gtk_box_append (GTK_BOX (vbox), grp_route);

    /* Plugin rack label */
    GtkWidget *lbl_inserts = gtk_label_new (_("Plugin Inserts:"));
    gtk_widget_add_css_class (lbl_inserts, "dim-label");
    gtk_widget_set_halign  (lbl_inserts, GTK_ALIGN_START);
    gtk_widget_set_margin_start (lbl_inserts, 8);
    gtk_box_append (GTK_BOX (vbox), lbl_inserts);

    /* Plugin rack placeholder */
    GtkWidget *rack = gtk_box_new (GTK_ORIENTATION_VERTICAL, 2);
    gtk_widget_add_css_class (rack, "card");
    gtk_widget_set_margin_start (rack, 6);
    gtk_widget_set_margin_end   (rack, 6);
    GtkWidget *rack_hint = gtk_label_new (_("(No plugins — add via Phase 4)"));
    gtk_widget_add_css_class (rack_hint, "dim-label");
    gtk_widget_set_margin_top    (rack_hint, 4);
    gtk_widget_set_margin_bottom (rack_hint, 4);
    gtk_box_append (GTK_BOX (rack), rack_hint);
    gtk_box_append (GTK_BOX (vbox), rack);

    /* Fader + Meter */
    GtkWidget *fader_meter = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_widget_set_margin_start (fader_meter, 8);
    gtk_widget_set_margin_end   (fader_meter, 8);
    gtk_widget_set_vexpand (fader_meter, TRUE);

    self->meter_bar = gtk_level_bar_new_for_interval (0.0, 1.0);
    gtk_level_bar_set_value    (GTK_LEVEL_BAR (self->meter_bar), 0.0);
    gtk_level_bar_set_inverted (GTK_LEVEL_BAR (self->meter_bar), TRUE);
    gtk_widget_set_size_request (self->meter_bar, 14, -1);
    gtk_widget_set_vexpand (self->meter_bar, TRUE);
    gtk_box_append (GTK_BOX (fader_meter), self->meter_bar);

    self->fader_scale = gtk_scale_new_with_range (GTK_ORIENTATION_VERTICAL, -60.0, 6.0, 0.5);
    gtk_scale_set_draw_value   (GTK_SCALE (self->fader_scale), FALSE);
    gtk_range_set_inverted     (GTK_RANGE  (self->fader_scale), TRUE);
    gtk_range_set_value        (GTK_RANGE  (self->fader_scale), 0.0);
    gtk_widget_set_vexpand (self->fader_scale, TRUE);
    g_signal_connect (self->fader_scale, "value-changed", G_CALLBACK (on_fader_changed), self);
    gtk_box_append (GTK_BOX (fader_meter), self->fader_scale);

    GtkWidget *lbl_db = gtk_label_new ("0\n-4\n-10\n-15\n-20\n-30\n-40\n-inf");
    gtk_widget_add_css_class (lbl_db, "dim-label");
    gtk_widget_add_css_class (lbl_db, "monospace");
    gtk_box_append (GTK_BOX (fader_meter), lbl_db);

    gtk_box_append (GTK_BOX (vbox), fader_meter);

    /* Fader readout */
    self->lbl_fader_readout = gtk_label_new ("Fader: 0.00 dB\nTrim:  0.00 dB");
    gtk_widget_add_css_class (self->lbl_fader_readout, "monospace");
    gtk_widget_set_margin_start (self->lbl_fader_readout, 6);
    gtk_widget_set_halign (self->lbl_fader_readout, GTK_ALIGN_START);
    gtk_box_append (GTK_BOX (vbox), self->lbl_fader_readout);

    /* Pan slider */
    GtkWidget *lbl_pan_hdr = gtk_label_new (_("Azimuth Pan  L ←→ R"));
    gtk_widget_add_css_class (lbl_pan_hdr, "dim-label");
    gtk_box_append (GTK_BOX (vbox), lbl_pan_hdr);

    self->pan_scale = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, -1.0, 1.0, 0.05);
    gtk_range_set_value    (GTK_RANGE (self->pan_scale), 0.0);
    gtk_widget_set_margin_bottom (self->pan_scale, 6);
    g_signal_connect (self->pan_scale, "value-changed", G_CALLBACK (on_pan_changed), self);
    gtk_box_append (GTK_BOX (vbox), self->pan_scale);

    return vbox;
}

/* ════════════════════════════════════════════════════════════════════════════
 * SECTION 5 — RIGHT SIDEBAR (Markers & Lists)
 * ════════════════════════════════════════════════════════════════════════════ */

static void
rebuild_sidebar_markers (ArdourEditorWorkspace *self)
{
    if (!self->sidebar_box) return;
    GtkWidget *parent = gtk_widget_get_parent (self->sidebar_box);
    if (!parent) return;
    gtk_box_remove (GTK_BOX (parent), self->sidebar_box);
    /* Rebuild below */

    GtkWidget *vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_size_request (vbox, 240, -1);
    gtk_widget_set_hexpand (vbox, FALSE);
    gtk_widget_add_css_class (vbox, "card");
    gtk_widget_set_margin_start (vbox, 2);

    /* Tab switcher for sidebar views */
    const char *sidebar_tabs[] = {
        _("Tracks"), _("Sources"), _("Regions"),
        _("Clips"), _("Snapshots"), _("Groups"), _("Ranges & Marks")
    };
    GtkWidget *tab_grid = gtk_grid_new ();
    gtk_grid_set_column_spacing (GTK_GRID (tab_grid), 2);
    gtk_grid_set_row_spacing (GTK_GRID (tab_grid), 2);
    gtk_widget_set_margin_start  (tab_grid, 4);
    gtk_widget_set_margin_end    (tab_grid, 4);
    gtk_widget_set_margin_top    (tab_grid, 4);
    for (int i = 0; i < 7; i++) {
        GtkWidget *tb = gtk_button_new_with_label (sidebar_tabs[i]);
        gtk_widget_add_css_class (tb, "daw-btn-compact");
        if (i == 6) gtk_widget_add_css_class (tb, "suggested-action");
        gtk_grid_attach (GTK_GRID (tab_grid), tb, i % 2, i / 2, 1, 1);
    }
    gtk_box_append (GTK_BOX (vbox), tab_grid);

    /* Loop / Punch Ranges card */
    GtkWidget *card1 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 4);
    gtk_widget_add_css_class (card1, "card");
    gtk_widget_set_margin_start  (card1, 4);
    gtk_widget_set_margin_end    (card1, 4);

    GtkWidget *lhdr1 = gtk_label_new (_("Loop / Punch Ranges"));
    gtk_widget_add_css_class (lhdr1, "bold");
    gtk_widget_set_halign (lhdr1, GTK_ALIGN_START);
    gtk_widget_set_margin_start (lhdr1, 4);
    gtk_box_append (GTK_BOX (card1), lhdr1);
    gtk_box_append (GTK_BOX (vbox), card1);

    /* Markers card */
    GtkWidget *card2 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 4);
    gtk_widget_add_css_class (card2, "card");
    gtk_widget_set_margin_start  (card2, 4);
    gtk_widget_set_margin_end    (card2, 4);

    GtkWidget *lhdr2 = gtk_label_new (_("Location Markers"));
    gtk_widget_add_css_class (lhdr2, "bold");
    gtk_widget_set_halign (lhdr2, GTK_ALIGN_START);
    gtk_widget_set_margin_start (lhdr2, 4);
    gtk_box_append (GTK_BOX (card2), lhdr2);

    for (const auto &m : self->markers) {
        GtkWidget *mrow = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 3);
        char btn_lbl[64];
        g_snprintf (btn_lbl, sizeof(btn_lbl), "→ %s", m.bbt_str.c_str ());
        GtkWidget *mb = gtk_button_new_with_label (btn_lbl);
        gtk_widget_add_css_class (mb, "daw-btn-compact");
        /* Closure: capture position */
        g_object_set_data_full (G_OBJECT (mb), "workspace", self, NULL);
        g_object_set_data (G_OBJECT (mb), "pos_x", GINT_TO_POINTER ((int)m.position_x));
        g_signal_connect (mb, "clicked", G_CALLBACK (+[](GtkButton *b, gpointer) {
            ArdourEditorWorkspace *ws = ARDOUR_EDITOR_WORKSPACE (
                g_object_get_data (G_OBJECT (b), "workspace"));
            double px = (double)GPOINTER_TO_INT (g_object_get_data (G_OBJECT (b), "pos_x"));
            ardour_editor_workspace_jump_to_position (ws, px);
        }), NULL);
        gtk_box_append (GTK_BOX (mrow), mb);

        GtkWidget *ml = gtk_label_new (m.name.c_str ());
        gtk_widget_add_css_class (ml, "dim-label");
        gtk_box_append (GTK_BOX (mrow), ml);

        gtk_box_append (GTK_BOX (card2), mrow);
    }

    if (self->markers.empty ()) {
        GtkWidget *empty_lbl = gtk_label_new (_("(No markers — use New Marker to add)"));
        gtk_widget_add_css_class (empty_lbl, "dim-label");
        gtk_widget_set_margin_start  (empty_lbl, 4);
        gtk_widget_set_margin_bottom (empty_lbl, 4);
        gtk_box_append (GTK_BOX (card2), empty_lbl);
    }

    gtk_box_append (GTK_BOX (vbox), card2);

    /* New Marker button */
    GtkWidget *btn_new_marker = gtk_button_new_with_label (_("+ New Marker…"));
    gtk_widget_add_css_class (btn_new_marker, "suggested-action");
    gtk_widget_set_margin_top   (btn_new_marker, 6);
    gtk_widget_set_margin_start (btn_new_marker, 4);
    gtk_widget_set_margin_end   (btn_new_marker, 4);
    g_signal_connect_swapped (btn_new_marker, "clicked",
        G_CALLBACK (ardour_editor_workspace_add_marker), self);
    gtk_box_append (GTK_BOX (vbox), btn_new_marker);

    self->sidebar_box = vbox;
    gtk_box_append (GTK_BOX (parent), self->sidebar_box);
}

/* ════════════════════════════════════════════════════════════════════════════
 * SECTION 6 — ADD TRACK DIALOG (Two-panel: GtkPaned)
 * ════════════════════════════════════════════════════════════════════════════ */

typedef struct {
    ArdourEditorWorkspace *workspace;
    GtkWidget *dialog;
    GtkListBox *type_list;
    GtkWidget *entry_name;
    GtkWidget *spin_count;
    GtkWidget *dd_channels;
    GtkWidget *dd_position;
    GtkWidget *dd_pin_mode;
    GtkWidget *lbl_desc;
} AddTrackDialogData;

static const struct {
    const char *name;
    const char *type;       /* internal type string */
    const char *color;
    const char *description;
} kTrackPresets[] = {
    { "Audio Tracks",   "audio", "#3a8a55",
      "Creates one or more audio tracks.\n\nYou can configure:\n"
      "• Number of tracks to add\n• Track name template\n"
      "• Mono, stereo, or multi-channel operation\n• Route group assignment\n"
      "• Pin connection mode (Flexible / Strict I/O)\n\n"
      "New tracks are inserted at the position you specify." },

    { "MIDI Tracks",    "midi",  "#4a6db5",
      "Creates one or more MIDI tracks.\n\nYou can configure:\n"
      "• Number of tracks to add\n• Track name template\n"
      "• Instrument plugin (or None to drive external hardware)\n"
      "• Route group assignment\n\n"
      "New tracks are inserted at the position you specify." },

    { "Audio Busses",   "bus",   "#5a7880",
      "Creates one or more audio mix buses.\n\nUse buses to:\n"
      "• Collect and process multiple track signals\n"
      "• Host shared send effects (reverb, delay)\n"
      "• Create stem groups for stems export\n\n"
      "You can configure the number of channels and group assignment." },

    { "MIDI Busses",    "bus",   "#7b5a90",
      "Creates one or more MIDI buses.\n\nMIDI buses combine the output "
      "of multiple tracks. They are commonly used to host a single heavy "
      "instrument plugin fed from multiple MIDI tracks." },

    { "VCA Masters",    "vca",   "#8a6030",
      "Creates one or more VCA master strips.\n\nVCA masters provide "
      "scalable gain control over multiple linked tracks without inserting "
      "a bus into the signal path. You can assign tracks to a VCA from "
      "their own strip." },

    { "Foldback Busses","bus",   "#6a5050",
      "Creates one or more foldback monitor buses.\n\nFoldback buses are "
      "used as master outputs for in-ear monitor mixes and are fed by "
      "hidden monitor sends from each track." },
};
static const int kNumPresets = 6;

static void
on_preset_row_selected (GtkListBox * /*lb*/, GtkListBoxRow *row, gpointer user_data)
{
    AddTrackDialogData *d = (AddTrackDialogData *) user_data;
    if (!row) return;
    int idx = gtk_list_box_row_get_index (row);
    if (idx < 0 || idx >= kNumPresets) return;
    gtk_label_set_text (GTK_LABEL (d->lbl_desc), kTrackPresets[idx].description);
}

void
ardour_editor_workspace_show_add_dialog (ArdourEditorWorkspace *self,
                                          GtkWindow             *parent,
                                          const char            *preset_type)
{
    g_return_if_fail (ARDOUR_IS_EDITOR_WORKSPACE (self));

    AddTrackDialogData *d = g_new0 (AddTrackDialogData, 1);
    d->workspace = self;

    GtkWidget *dialog = gtk_dialog_new_with_buttons (
        _("Add Track / Bus / VCA"),
        parent,
        (GtkDialogFlags)(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
        _("Cancel"),          GTK_RESPONSE_CANCEL,
        _("Add and Close"),   GTK_RESPONSE_ACCEPT,
        NULL);
    d->dialog = dialog;
    gtk_window_set_default_size (GTK_WINDOW (dialog), 720, 460);
    gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_ACCEPT);

    GtkWidget *content = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
    gtk_box_set_spacing (GTK_BOX (content), 0);

    /* ── Two-panel split using GtkPaned ── */
    GtkWidget *paned = gtk_paned_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_paned_set_position (GTK_PANED (paned), 200);
    gtk_widget_set_vexpand (paned, TRUE);
    gtk_box_append (GTK_BOX (content), paned);

    /* Left panel — preset list */
    GtkWidget *left_scroll = gtk_scrolled_window_new ();
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (left_scroll),
                                    GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request (left_scroll, 190, -1);

    d->type_list = GTK_LIST_BOX (gtk_list_box_new ());
    gtk_list_box_set_selection_mode (d->type_list, GTK_SELECTION_SINGLE);

    for (int i = 0; i < kNumPresets; i++) {
        GtkWidget *row_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6);
        gtk_widget_set_margin_start  (row_box, 6);
        gtk_widget_set_margin_end    (row_box, 6);
        gtk_widget_set_margin_top    (row_box, 4);
        gtk_widget_set_margin_bottom (row_box, 4);

        /* Small color dot */
        GtkWidget *dot = gtk_drawing_area_new ();
        gtk_widget_set_size_request (dot, 10, 10);
        gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (dot),
            +[](GtkDrawingArea *, cairo_t *c, int, int, gpointer ud) {
                double r, g, b;
                parse_color_hex ((const char *) ud, &r, &g, &b);
                cairo_set_source_rgb (c, r, g, b);
                cairo_arc (c, 5, 5, 4, 0, 2 * G_PI);
                cairo_fill (c);
            },
            g_strdup (kTrackPresets[i].color), g_free);
        gtk_widget_set_valign (dot, GTK_ALIGN_CENTER);
        gtk_box_append (GTK_BOX (row_box), dot);

        GtkWidget *lbl = gtk_label_new (kTrackPresets[i].name);
        gtk_widget_set_halign (lbl, GTK_ALIGN_START);
        gtk_box_append (GTK_BOX (row_box), lbl);

        gtk_list_box_append (d->type_list, row_box);
    }

    g_signal_connect (d->type_list, "row-selected",
                      G_CALLBACK (on_preset_row_selected), d);
    gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (left_scroll), GTK_WIDGET (d->type_list));
    gtk_paned_set_start_child (GTK_PANED (paned), left_scroll);
    gtk_paned_set_shrink_start_child (GTK_PANED (paned), FALSE);

    /* Right panel — settings form */
    GtkWidget *right_box = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);

    /* Description text area */
    GtkWidget *desc_scroll = gtk_scrolled_window_new ();
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (desc_scroll),
                                    GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request (desc_scroll, -1, 100);

    d->lbl_desc = gtk_label_new (kTrackPresets[0].description);
    gtk_label_set_wrap         (GTK_LABEL (d->lbl_desc), TRUE);
    gtk_label_set_xalign       (GTK_LABEL (d->lbl_desc), 0.0);
    gtk_widget_add_css_class   (d->lbl_desc, "dim-label");
    gtk_widget_set_margin_start  (d->lbl_desc, 10);
    gtk_widget_set_margin_end    (d->lbl_desc, 10);
    gtk_widget_set_margin_top    (d->lbl_desc, 8);
    gtk_widget_set_margin_bottom (d->lbl_desc, 8);
    gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (desc_scroll), d->lbl_desc);
    gtk_box_append (GTK_BOX (right_box), desc_scroll);

    gtk_box_append (GTK_BOX (right_box), gtk_separator_new (GTK_ORIENTATION_HORIZONTAL));

    /* Settings form grid */
    GtkWidget *form = gtk_grid_new ();
    gtk_grid_set_row_spacing    (GTK_GRID (form), 8);
    gtk_grid_set_column_spacing (GTK_GRID (form), 10);
    gtk_widget_set_margin_start  (form, 14);
    gtk_widget_set_margin_end    (form, 14);
    gtk_widget_set_margin_top    (form, 12);
    gtk_widget_set_margin_bottom (form, 12);

    int row = 0;

    /* Count */
    GtkWidget *lbl_count = gtk_label_new (_("Add:"));
    gtk_widget_set_halign (lbl_count, GTK_ALIGN_END);
    gtk_grid_attach (GTK_GRID (form), lbl_count, 0, row, 1, 1);
    GtkAdjustment *adj = gtk_adjustment_new (1, 1, 128, 1, 4, 0);
    d->spin_count = gtk_spin_button_new (adj, 1, 0);
    gtk_grid_attach (GTK_GRID (form), d->spin_count, 1, row, 1, 1);
    row++;

    /* Name */
    GtkWidget *lbl_name = gtk_label_new (_("Name:"));
    gtk_widget_set_halign (lbl_name, GTK_ALIGN_END);
    gtk_grid_attach (GTK_GRID (form), lbl_name, 0, row, 1, 1);
    d->entry_name = gtk_entry_new ();
    gtk_editable_set_text (GTK_EDITABLE (d->entry_name), _("Audio"));
    gtk_entry_set_activates_default (GTK_ENTRY (d->entry_name), TRUE);
    gtk_widget_set_hexpand (d->entry_name, TRUE);
    gtk_grid_attach (GTK_GRID (form), d->entry_name, 1, row, 2, 1);
    row++;

    /* Channels */
    GtkWidget *lbl_ch = gtk_label_new (_("Input Config:"));
    gtk_widget_set_halign (lbl_ch, GTK_ALIGN_END);
    gtk_grid_attach (GTK_GRID (form), lbl_ch, 0, row, 1, 1);
    const char *ch_opts[] = { _("Mono"), _("Stereo"), _("3 channels"), _("4 channels"), NULL };
    d->dd_channels = gtk_drop_down_new_from_strings (ch_opts);
    gtk_drop_down_set_selected (GTK_DROP_DOWN (d->dd_channels), 1); /* default: Stereo */
    gtk_grid_attach (GTK_GRID (form), d->dd_channels, 1, row, 1, 1);
    row++;

    /* Position */
    GtkWidget *lbl_pos = gtk_label_new (_("Position:"));
    gtk_widget_set_halign (lbl_pos, GTK_ALIGN_END);
    gtk_grid_attach (GTK_GRID (form), lbl_pos, 0, row, 1, 1);
    const char *pos_opts[] = {
        _("First"), _("Before Selection"), _("After Selection"), _("Last"), NULL
    };
    d->dd_position = gtk_drop_down_new_from_strings (pos_opts);
    gtk_drop_down_set_selected (GTK_DROP_DOWN (d->dd_position), 3); /* Last */
    gtk_grid_attach (GTK_GRID (form), d->dd_position, 1, row, 1, 1);
    row++;

    /* Pin mode */
    GtkWidget *lbl_pin = gtk_label_new (_("Pin Mode:"));
    gtk_widget_set_halign (lbl_pin, GTK_ALIGN_END);
    gtk_grid_attach (GTK_GRID (form), lbl_pin, 0, row, 1, 1);
    const char *pin_opts[] = { _("Flexible-I/O"), _("Strict-I/O"), NULL };
    d->dd_pin_mode = gtk_drop_down_new_from_strings (pin_opts);
    gtk_grid_attach (GTK_GRID (form), d->dd_pin_mode, 1, row, 1, 1);
    row++;

    gtk_box_append (GTK_BOX (right_box), form);
    gtk_paned_set_end_child (GTK_PANED (paned), right_box);
    gtk_paned_set_shrink_end_child (GTK_PANED (paned), FALSE);

    /* Select the first preset matching preset_type hint */
    int default_row = 0;
    if (preset_type) {
        for (int i = 0; i < kNumPresets; i++) {
            if (g_strcmp0 (kTrackPresets[i].name, preset_type) == 0) {
                default_row = i;
                break;
            }
            if (g_str_has_prefix (preset_type, "MIDI") && g_str_has_prefix (kTrackPresets[i].name, "MIDI Tracks")) {
                default_row = i;
                break;
            }
        }
    }
    gtk_list_box_select_row (d->type_list,
        gtk_list_box_get_row_at_index (d->type_list, default_row));
    gtk_label_set_text (GTK_LABEL (d->lbl_desc), kTrackPresets[default_row].description);

    gtk_widget_show (dialog);

    /* Response handler */
    g_signal_connect (dialog, "response", G_CALLBACK (+[](GtkDialog *dlg, int resp, gpointer ud) {
        AddTrackDialogData *data = (AddTrackDialogData *) ud;
        if (resp == GTK_RESPONSE_ACCEPT) {
            /* Determine selected type */
            GtkListBoxRow *sel = gtk_list_box_get_selected_row (data->type_list);
            int preset_idx = sel ? gtk_list_box_row_get_index (sel) : 0;
            const char *type  = kTrackPresets[preset_idx].type;
            const char *color = kTrackPresets[preset_idx].color;

            /* Count and name */
            int count = (int)gtk_spin_button_get_value (GTK_SPIN_BUTTON (data->spin_count));
            const char *base_name = gtk_editable_get_text (GTK_EDITABLE (data->entry_name));

            for (int i = 0; i < count; i++) {
                TrackData tr;
                if (count > 1) {
                    char name_buf[128];
                    g_snprintf (name_buf, sizeof(name_buf), "%s %d", base_name, i + 1);
                    tr.name = name_buf;
                } else {
                    tr.name = (base_name && base_name[0]) ? base_name : type;
                }
                tr.type        = type;
                tr.color_hex   = color;
                tr.is_active   = false;
                tr.is_muted    = false;
                tr.is_soloed   = false;
                tr.is_rec_armed = false;
                tr.fader_db    = 0.0;
                tr.trim_db     = 0.0;
                tr.pan_pos     = 0.0;
                tr.has_regions = false;
                tr.height_px   = (g_strcmp0 (type, "midi") == 0) ? 90
                               : (g_strcmp0 (type, "vca")  == 0) ? 40 : 70;

                data->workspace->tracks.push_back (tr);
            }

            rebuild_track_headers (data->workspace);
            gtk_widget_set_size_request (data->workspace->canvas_draw_area,
                                         -1, total_tracks_height (data->workspace));
            gtk_widget_queue_draw (data->workspace->canvas_draw_area);
        }
        gtk_window_destroy (GTK_WINDOW (dlg));
        g_free (data);
    }), d);
}

/* ════════════════════════════════════════════════════════════════════════════
 * SECTION 7 — PUBLIC API IMPLEMENTATIONS
 * ════════════════════════════════════════════════════════════════════════════ */

void
ardour_editor_workspace_add_track (ArdourEditorWorkspace *self,
                                    const char *name, const char *type)
{
    g_return_if_fail (ARDOUR_IS_EDITOR_WORKSPACE (self));

    TrackData tr;
    tr.name       = (name && name[0]) ? name : "Audio";
    tr.type       = (type && type[0]) ? type : "audio";
    tr.color_hex  = (tr.type == "midi") ? "#4a6db5"
                  : (tr.type == "bus")  ? "#5a7880"
                  : (tr.type == "vca")  ? "#8a6030" : "#3a8a55";
    tr.is_active  = false;
    tr.is_muted   = false;
    tr.is_soloed  = false;
    tr.is_rec_armed = false;
    tr.fader_db   = 0.0;
    tr.trim_db    = 0.0;
    tr.pan_pos    = 0.0;
    tr.has_regions = false;
    tr.height_px  = (tr.type == "midi") ? 90 : (tr.type == "vca") ? 40 : 70;

    self->tracks.push_back (tr);
    rebuild_track_headers (self);
    gtk_widget_set_size_request (self->canvas_draw_area, -1, total_tracks_height (self));
    gtk_widget_queue_draw (self->canvas_draw_area);
}

void
ardour_editor_workspace_select_track (ArdourEditorWorkspace *self, int index)
{
    g_return_if_fail (ARDOUR_IS_EDITOR_WORKSPACE (self));
    if (index < 0 || index >= (int)self->tracks.size ()) return;

    for (size_t i = 0; i < self->tracks.size (); ++i)
        self->tracks[i].is_active = ((int)i == index);
    self->selected_track_index = index;

    rebuild_track_headers (self);
    update_inspector_for_selected (self);
    gtk_widget_queue_draw (self->canvas_draw_area);
}

void
ardour_editor_workspace_remove_track (ArdourEditorWorkspace *self, int index)
{
    g_return_if_fail (ARDOUR_IS_EDITOR_WORKSPACE (self));
    if (index < 0 || index >= (int)self->tracks.size ()) return;

    self->tracks.erase (self->tracks.begin () + index);

    /* Adjust selected index */
    if (self->selected_track_index >= (int)self->tracks.size ())
        self->selected_track_index = (int)self->tracks.size () - 1;

    rebuild_track_headers (self);
    update_inspector_for_selected (self);
    gtk_widget_set_size_request (self->canvas_draw_area, -1, total_tracks_height (self));
    gtk_widget_queue_draw (self->canvas_draw_area);
}

void
ardour_editor_workspace_rename_track (ArdourEditorWorkspace *self, int index, const char *new_name)
{
    g_return_if_fail (ARDOUR_IS_EDITOR_WORKSPACE (self));
    if (index < 0 || index >= (int)self->tracks.size ()) return;
    if (!new_name || new_name[0] == '\0') return;
    self->tracks[index].name = new_name;
    rebuild_track_headers (self);
    update_inspector_for_selected (self);
}

void
ardour_editor_workspace_add_marker (ArdourEditorWorkspace *self, const char *name_hint)
{
    g_return_if_fail (ARDOUR_IS_EDITOR_WORKSPACE (self));

    /* Generate default name */
    char default_name[64];
    g_snprintf (default_name, sizeof(default_name), "Marker %zu", self->markers.size () + 1);

    /* Show name dialog */
    GtkWindow *parent = GTK_WINDOW (gtk_widget_get_root (GTK_WIDGET (self)));
    GtkWidget *dialog = gtk_dialog_new_with_buttons (
        _("New Marker"), parent,
        (GtkDialogFlags)(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
        _("Cancel"), GTK_RESPONSE_CANCEL,
        _("Add"),    GTK_RESPONSE_ACCEPT,
        NULL);
    gtk_dialog_set_default_response (GTK_DIALOG (dialog), GTK_RESPONSE_ACCEPT);

    GtkWidget *content = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
    gtk_box_set_spacing (GTK_BOX (content), 8);
    gtk_widget_set_margin_start  (content, 12);
    gtk_widget_set_margin_end    (content, 12);
    gtk_widget_set_margin_top    (content, 8);
    gtk_widget_set_margin_bottom (content, 8);

    GtkWidget *lbl = gtk_label_new (_("Marker name:"));
    gtk_widget_set_halign (lbl, GTK_ALIGN_START);
    gtk_box_append (GTK_BOX (content), lbl);

    GtkWidget *entry = gtk_entry_new ();
    gtk_editable_set_text (GTK_EDITABLE (entry), name_hint ? name_hint : default_name);
    gtk_entry_set_activates_default (GTK_ENTRY (entry), TRUE);
    gtk_box_append (GTK_BOX (content), entry);

    /* Capture needed values by heap-allocating a small struct */
    struct MarkerDialogCtx {
        ArdourEditorWorkspace *ws;
        double pos_x;
    };
    auto *ctx = g_new (MarkerDialogCtx, 1);
    ctx->ws    = self;
    ctx->pos_x = self->playhead_x;

    gtk_widget_show (dialog);

    g_signal_connect (dialog, "response", G_CALLBACK (+[](GtkDialog *d, int resp, gpointer ud) {
        auto *c = (MarkerDialogCtx *) ud;
        if (resp == GTK_RESPONSE_ACCEPT) {
            GtkWidget *content = gtk_dialog_get_content_area (d);
            GtkWidget *entry = gtk_widget_get_last_child (content);
            const char *text = gtk_editable_get_text (GTK_EDITABLE (entry));

            MarkerData m;
            m.name       = (text && text[0]) ? text : "Marker";
            m.position_x = c->pos_x;
            guint bar = (guint)(c->pos_x / 40) + 1;
            char bbt[32];
            g_snprintf (bbt, sizeof(bbt), "%03u|01|0000", bar);
            m.bbt_str = bbt;

            c->ws->markers.push_back (m);
            gtk_widget_queue_draw (c->ws->ruler_area);
            rebuild_sidebar_markers (c->ws);
        }
        gtk_window_destroy (GTK_WINDOW (d));
        g_free (c);
    }), ctx);
}

void
ardour_editor_workspace_jump_to_position (ArdourEditorWorkspace *self, double x)
{
    g_return_if_fail (ARDOUR_IS_EDITOR_WORKSPACE (self));
    self->playhead_x = x;
    gtk_widget_queue_draw (self->canvas_draw_area);
    gtk_widget_queue_draw (self->ruler_area);
}

void
ardour_editor_workspace_set_zoom (ArdourEditorWorkspace *self, double zoom_factor)
{
    g_return_if_fail (ARDOUR_IS_EDITOR_WORKSPACE (self));
    self->zoom_factor = CLAMP (zoom_factor, 0.1, 8.0);
    gtk_widget_queue_draw (self->canvas_draw_area);
    gtk_widget_queue_draw (self->ruler_area);
}

double
ardour_editor_workspace_get_current_zoom (ArdourEditorWorkspace *self)
{
    g_return_val_if_fail (ARDOUR_IS_EDITOR_WORKSPACE (self), 1.0);
    return self->zoom_factor;
}

int
ardour_editor_workspace_get_selected_track (ArdourEditorWorkspace *self)
{
    g_return_val_if_fail (ARDOUR_IS_EDITOR_WORKSPACE (self), -1);
    return self->selected_track_index;
}

/* ════════════════════════════════════════════════════════════════════════════
 * SECTION 8 — WIDGET LIFECYCLE
 * ════════════════════════════════════════════════════════════════════════════ */

static void
ardour_editor_workspace_dispose (GObject *object)
{
    ArdourEditorWorkspace *self = ARDOUR_EDITOR_WORKSPACE (object);

    GtkWidget *child = gtk_widget_get_first_child (GTK_WIDGET (self));
    while (child) {
        GtkWidget *next = gtk_widget_get_next_sibling (child);
        gtk_widget_unparent (child);
        child = next;
    }

    G_OBJECT_CLASS (ardour_editor_workspace_parent_class)->dispose (object);
}

static void
ardour_editor_workspace_class_init (ArdourEditorWorkspaceClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    object_class->dispose = ardour_editor_workspace_dispose;
    gtk_widget_class_set_layout_manager_type (GTK_WIDGET_CLASS (klass), GTK_TYPE_BIN_LAYOUT);
}

static void
ardour_editor_workspace_init (ArdourEditorWorkspace *self)
{
    /* ── Phase 3.5: Start with a completely empty session ── */
    self->playhead_x          = 0.0;
    self->zoom_factor         = 1.0;
    self->selected_track_index = -1;
    self->sidebar_box          = NULL;

    /* tracks and markers default-constructed as empty vectors */

    /* CSS for compact DAW buttons */
    GtkCssProvider *css = gtk_css_provider_new ();
    gtk_css_provider_load_from_data (css,
        ".daw-btn-compact { min-height:22px; min-width:22px; padding:0 5px; font-size:11px; }\n"
        ".bold { font-weight: bold; }\n"
        ".monospace { font-family: monospace; font-size: 11px; }\n", -1);
    gtk_style_context_add_provider_for_display (
        gdk_display_get_default (),
        GTK_STYLE_PROVIDER (css),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref (css);

    /* ── Main horizontal layout ── */
    self->main_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_hexpand (self->main_hbox, TRUE);
    gtk_widget_set_vexpand (self->main_hbox, TRUE);
    gtk_widget_set_parent (self->main_hbox, GTK_WIDGET (self));

    /* 1. Left: Track Inspector Panel (220 px, fixed) */
    self->inspector_box = create_track_inspector_panel (self);
    gtk_box_append (GTK_BOX (self->main_hbox), self->inspector_box);
    gtk_box_append (GTK_BOX (self->main_hbox), gtk_separator_new (GTK_ORIENTATION_VERTICAL));

    /* 2. Center: Ruler + (Track Headers | Canvas) with synchronized scroll */
    self->canvas_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_hexpand (self->canvas_vbox, TRUE);
    gtk_widget_set_vexpand (self->canvas_vbox, TRUE);

    /* 2a. Timeline ruler (96 px tall, horizontal scroll only) */
    self->ruler_area = gtk_drawing_area_new ();
    gtk_widget_set_size_request (self->ruler_area, -1, 96);
    gtk_widget_set_hexpand (self->ruler_area, TRUE);
    gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (self->ruler_area),
                                     on_ruler_draw, self, NULL);
    GtkGesture *ruler_click = gtk_gesture_click_new ();
    g_signal_connect (ruler_click, "pressed", G_CALLBACK (on_ruler_click), self);
    gtk_widget_add_controller (self->ruler_area, GTK_EVENT_CONTROLLER (ruler_click));
    gtk_box_append (GTK_BOX (self->canvas_vbox), self->ruler_area);

    /* 2b. Horizontal box: track-header-scroll | timeline-canvas-scroll */
    GtkWidget *tracks_canvas_hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_hexpand (tracks_canvas_hbox, TRUE);
    gtk_widget_set_vexpand (tracks_canvas_hbox, TRUE);

    /* ── Timeline canvas scroll (RIGHT, hscroll + vscroll) ── */
    self->timeline_scroll = gtk_scrolled_window_new ();
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (self->timeline_scroll),
                                     GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_hexpand (self->timeline_scroll, TRUE);
    gtk_widget_set_vexpand (self->timeline_scroll, TRUE);

    self->canvas_draw_area = gtk_drawing_area_new ();
    gtk_widget_set_hexpand (self->canvas_draw_area, TRUE);
    gtk_widget_set_size_request (self->canvas_draw_area, 1800, MAX (total_tracks_height (self), 300));
    gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (self->canvas_draw_area),
                                     on_canvas_draw, self, NULL);
    GtkGesture *canvas_click = gtk_gesture_click_new ();
    g_signal_connect (canvas_click, "pressed", G_CALLBACK (on_canvas_click), self);
    gtk_widget_add_controller (self->canvas_draw_area, GTK_EVENT_CONTROLLER (canvas_click));

    gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (self->timeline_scroll),
                                    self->canvas_draw_area);

    /* ── Track headers scroll (LEFT, vscroll only, SHARES vadjustment) ── */
    self->headers_scroll = gtk_scrolled_window_new ();
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (self->headers_scroll),
                                     GTK_POLICY_NEVER, GTK_POLICY_EXTERNAL);
    gtk_widget_set_size_request (self->headers_scroll, 200, -1);
    gtk_widget_set_vexpand (self->headers_scroll, TRUE);

    /* Share the timeline's vertical adjustment → lockstep 60 FPS vertical sync */
    GtkAdjustment *shared_vadj =
        gtk_scrolled_window_get_vadjustment (GTK_SCROLLED_WINDOW (self->timeline_scroll));
    gtk_scrolled_window_set_vadjustment (GTK_SCROLLED_WINDOW (self->headers_scroll),
                                          shared_vadj);

    self->tracks_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request (self->tracks_vbox, 200, -1);
    gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (self->headers_scroll),
                                    self->tracks_vbox);

    rebuild_track_headers (self);

    gtk_box_append (GTK_BOX (tracks_canvas_hbox), self->headers_scroll);
    gtk_box_append (GTK_BOX (tracks_canvas_hbox), self->timeline_scroll);

    gtk_box_append (GTK_BOX (self->canvas_vbox), tracks_canvas_hbox);
    gtk_box_append (GTK_BOX (self->main_hbox), self->canvas_vbox);
    gtk_box_append (GTK_BOX (self->main_hbox), gtk_separator_new (GTK_ORIENTATION_VERTICAL));

    /* 3. Right: Sidebar (240 px, fixed) */
    GtkWidget *sidebar_outer = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append (GTK_BOX (self->main_hbox), sidebar_outer);
    self->sidebar_box = NULL;

    /* Build sidebar inside sidebar_outer */
    GtkWidget *sidebar = gtk_box_new (GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_size_request (sidebar, 240, -1);
    gtk_widget_set_hexpand (sidebar, FALSE);
    gtk_widget_add_css_class (sidebar, "card");
    gtk_widget_set_margin_start (sidebar, 2);
    gtk_box_append (GTK_BOX (sidebar_outer), sidebar);
    self->sidebar_box = sidebar;

    /* Do a full sidebar build */
    rebuild_sidebar_markers (self);
}

GtkWidget *
ardour_editor_workspace_new (void)
{
    return GTK_WIDGET (g_object_new (ARDOUR_TYPE_EDITOR_WORKSPACE, NULL));
}
