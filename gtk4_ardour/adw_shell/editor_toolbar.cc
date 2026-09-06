/*
 * ardour4-shell: Secondary Editor Control Strip Implementation
 * Phase 3.5 — GTK4/Libadwaita migration of Ardour Plus
 *
 * Recreates Row 2 of legacy GTK2 Ardour editor control strip:
 * - Edit Mode Selector (Slide, Lock, Ripple)
 * - Mouse Tool Palette (Smart, Select, Range, Cut, Stretch, Draw) — radio-group, wired
 * - Snap Toggle & Grid Unit selector
 * - Timeline Cursor Locator display
 * - Zoom Controls — wired to workspace via ardour_editor_toolbar_set_workspace()
 */

#include "editor_toolbar.h"
#include "editor_workspace.h"
#include <glib/gi18n.h>

typedef enum {
    TOOL_SMART = 0,
    TOOL_SELECT,
    TOOL_RANGE,
    TOOL_CUT,
    TOOL_STRETCH,
    TOOL_DRAW
} EditorTool;

struct _ArdourEditorToolbar {
    GtkWidget parent_instance;

    /* Workspace reference injected by shell_window.cc */
    GtkWidget *workspace;

    GtkWidget *edit_mode_combo;
    GtkWidget *btn_smart;
    GtkWidget *btn_select;
    GtkWidget *btn_range;
    GtkWidget *btn_cut;
    GtkWidget *btn_stretch;
    GtkWidget *btn_draw;
    GtkWidget *btn_snap_toggle;
    GtkWidget *grid_unit_combo;
    GtkWidget *chk_adjust_markers;
    GtkWidget *lbl_cursor_pos;
    GtkWidget *btn_zoom_in;
    GtkWidget *btn_zoom_out;
    GtkWidget *btn_zoom_full;
    GtkWidget *scrubber_bar;

    EditorTool current_tool;
    gboolean   snap_enabled;
};

G_DEFINE_TYPE (ArdourEditorToolbar, ardour_editor_toolbar, GTK_TYPE_WIDGET)

/* ── Tool radio-group helpers ─────────────────────────────────────────────── */

static void
set_active_tool (ArdourEditorToolbar *self, EditorTool tool)
{
    GtkWidget *btns[6] = {
        self->btn_smart, self->btn_select, self->btn_range,
        self->btn_cut, self->btn_stretch, self->btn_draw
    };
    for (int i = 0; i < 6; i++) {
        if (i == (int)tool) {
            gtk_widget_add_css_class    (btns[i], "suggested-action");
        } else {
            gtk_widget_remove_css_class (btns[i], "suggested-action");
        }
    }
    self->current_tool = tool;
}

static void on_tool_smart   (GtkButton *, gpointer ud) { set_active_tool (ARDOUR_EDITOR_TOOLBAR (ud), TOOL_SMART);   }
static void on_tool_select  (GtkButton *, gpointer ud) { set_active_tool (ARDOUR_EDITOR_TOOLBAR (ud), TOOL_SELECT);  }
static void on_tool_range   (GtkButton *, gpointer ud) { set_active_tool (ARDOUR_EDITOR_TOOLBAR (ud), TOOL_RANGE);   }
static void on_tool_cut     (GtkButton *, gpointer ud) { set_active_tool (ARDOUR_EDITOR_TOOLBAR (ud), TOOL_CUT);     }
static void on_tool_stretch (GtkButton *, gpointer ud) { set_active_tool (ARDOUR_EDITOR_TOOLBAR (ud), TOOL_STRETCH); }
static void on_tool_draw    (GtkButton *, gpointer ud) { set_active_tool (ARDOUR_EDITOR_TOOLBAR (ud), TOOL_DRAW);    }

/* ── Zoom handlers ─────────────────────────────────────────────────────────── */

static void
on_zoom_in_clicked (GtkButton * /*btn*/, gpointer user_data)
{
    ArdourEditorToolbar *self = ARDOUR_EDITOR_TOOLBAR (user_data);
    if (!self->workspace) return;
    double z = ardour_editor_workspace_get_current_zoom (ARDOUR_EDITOR_WORKSPACE (self->workspace));
    ardour_editor_workspace_set_zoom (ARDOUR_EDITOR_WORKSPACE (self->workspace),
                                      MIN (z * 1.5, 8.0));
}

static void
on_zoom_out_clicked (GtkButton * /*btn*/, gpointer user_data)
{
    ArdourEditorToolbar *self = ARDOUR_EDITOR_TOOLBAR (user_data);
    if (!self->workspace) return;
    double z = ardour_editor_workspace_get_current_zoom (ARDOUR_EDITOR_WORKSPACE (self->workspace));
    ardour_editor_workspace_set_zoom (ARDOUR_EDITOR_WORKSPACE (self->workspace),
                                      MAX (z / 1.5, 0.1));
}

static void
on_zoom_full_clicked (GtkButton * /*btn*/, gpointer user_data)
{
    ArdourEditorToolbar *self = ARDOUR_EDITOR_TOOLBAR (user_data);
    if (!self->workspace) return;
    ardour_editor_workspace_set_zoom (ARDOUR_EDITOR_WORKSPACE (self->workspace), 1.0);
}

/* ── Snap toggle ────────────────────────────────────────────────────────────── */

static void
on_snap_toggled (GtkButton *button, gpointer user_data)
{
    ArdourEditorToolbar *self = ARDOUR_EDITOR_TOOLBAR (user_data);
    self->snap_enabled = !self->snap_enabled;
    if (self->snap_enabled) {
        gtk_widget_add_css_class    (GTK_WIDGET (button), "suggested-action");
    } else {
        gtk_widget_remove_css_class (GTK_WIDGET (button), "suggested-action");
    }
}

/* ── Lifecycle ──────────────────────────────────────────────────────────────── */

static void
ardour_editor_toolbar_dispose (GObject *object)
{
    ArdourEditorToolbar *self = ARDOUR_EDITOR_TOOLBAR (object);

    GtkWidget *child = gtk_widget_get_first_child (GTK_WIDGET (self));
    while (child) {
        GtkWidget *next = gtk_widget_get_next_sibling (child);
        gtk_widget_unparent (child);
        child = next;
    }

    G_OBJECT_CLASS (ardour_editor_toolbar_parent_class)->dispose (object);
}

static void
ardour_editor_toolbar_class_init (ArdourEditorToolbarClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    object_class->dispose = ardour_editor_toolbar_dispose;

    gtk_widget_class_set_layout_manager_type (GTK_WIDGET_CLASS (klass), GTK_TYPE_BIN_LAYOUT);
}

static void
ardour_editor_toolbar_init (ArdourEditorToolbar *self)
{
    self->workspace    = NULL;
    self->current_tool = TOOL_SMART;
    self->snap_enabled = TRUE;

    /* Custom CSS for editor toolbar */
    GtkCssProvider *css = gtk_css_provider_new ();
    gtk_css_provider_load_from_data (css,
        ".editor-strip { background-color: #16181b; border-bottom: 1px solid #24282e; padding: 2px 6px; }\n"
        ".loc-box { background-color: #0d0f12; border: 1px solid #262a30; border-radius: 3px; padding: 1px 6px; }\n"
        ".loc-text { font-family: 'DejaVu Sans Mono', 'Liberation Mono', monospace; font-size: 13px; font-weight: bold; color: #00ffcc; }\n"
        ".strip-btn { min-height: 24px; padding: 1px 6px; font-size: 11px; }\n", -1);
    gtk_style_context_add_provider_for_display (
        gdk_display_get_default (),
        GTK_STYLE_PROVIDER (css),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref (css);

    GtkWidget *hbox = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_widget_add_css_class (hbox, "editor-strip");
    gtk_widget_set_parent (hbox, GTK_WIDGET (self));

    /* 1. Edit Mode Selector Dropdown */
    const char *edit_modes[] = { _("Slide Mode"), _("Lock Mode"), _("Ripple Mode"), NULL };
    self->edit_mode_combo = gtk_drop_down_new_from_strings (edit_modes);
    gtk_widget_set_tooltip_text (self->edit_mode_combo, _("Edit Mode (Slide / Lock / Ripple)"));
    gtk_box_append (GTK_BOX (hbox), self->edit_mode_combo);

    gtk_box_append (GTK_BOX (hbox), gtk_separator_new (GTK_ORIENTATION_VERTICAL));

    /* 2. Mouse Tool Palette Buttons — radio group, each wired */
    GtkWidget *grp_tools = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_widget_add_css_class (grp_tools, "linked");

    self->btn_smart = gtk_button_new_with_label (_("Smart"));
    gtk_widget_add_css_class (self->btn_smart, "strip-btn");
    gtk_widget_add_css_class (self->btn_smart, "suggested-action"); /* default active */
    gtk_widget_set_tooltip_text (self->btn_smart, _("Smart Tool (Context-Sensitive Mouse Actions)"));
    g_signal_connect (self->btn_smart, "clicked", G_CALLBACK (on_tool_smart), self);
    gtk_box_append (GTK_BOX (grp_tools), self->btn_smart);

    self->btn_select = gtk_button_new_with_label (_("Select"));
    gtk_widget_add_css_class (self->btn_select, "strip-btn");
    gtk_widget_set_tooltip_text (self->btn_select, _("Object Selection Pointer Tool"));
    g_signal_connect (self->btn_select, "clicked", G_CALLBACK (on_tool_select), self);
    gtk_box_append (GTK_BOX (grp_tools), self->btn_select);

    self->btn_range = gtk_button_new_with_label (_("Range"));
    gtk_widget_add_css_class (self->btn_range, "strip-btn");
    gtk_widget_set_tooltip_text (self->btn_range, _("Range Selection Tool"));
    g_signal_connect (self->btn_range, "clicked", G_CALLBACK (on_tool_range), self);
    gtk_box_append (GTK_BOX (grp_tools), self->btn_range);

    self->btn_cut = gtk_button_new_with_label (_("Cut"));
    gtk_widget_add_css_class (self->btn_cut, "strip-btn");
    gtk_widget_set_tooltip_text (self->btn_cut, _("Cut / Split Region Tool"));
    g_signal_connect (self->btn_cut, "clicked", G_CALLBACK (on_tool_cut), self);
    gtk_box_append (GTK_BOX (grp_tools), self->btn_cut);

    self->btn_stretch = gtk_button_new_with_label (_("Stretch"));
    gtk_widget_add_css_class (self->btn_stretch, "strip-btn");
    gtk_widget_set_tooltip_text (self->btn_stretch, _("Time-Stretch / Pitch Tool"));
    g_signal_connect (self->btn_stretch, "clicked", G_CALLBACK (on_tool_stretch), self);
    gtk_box_append (GTK_BOX (grp_tools), self->btn_stretch);

    self->btn_draw = gtk_button_new_with_label (_("Draw"));
    gtk_widget_add_css_class (self->btn_draw, "strip-btn");
    gtk_widget_set_tooltip_text (self->btn_draw, _("Draw MIDI / Automation Pencil Tool"));
    g_signal_connect (self->btn_draw, "clicked", G_CALLBACK (on_tool_draw), self);
    gtk_box_append (GTK_BOX (grp_tools), self->btn_draw);

    gtk_box_append (GTK_BOX (hbox), grp_tools);

    gtk_box_append (GTK_BOX (hbox), gtk_separator_new (GTK_ORIENTATION_VERTICAL));

    /* 3. Snap & Grid Section */
    GtkWidget *grp_snap = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);

    self->btn_snap_toggle = gtk_button_new_with_label (_("Snap"));
    gtk_widget_add_css_class (self->btn_snap_toggle, "strip-btn");
    gtk_widget_add_css_class (self->btn_snap_toggle, "suggested-action");
    gtk_widget_set_tooltip_text (self->btn_snap_toggle, _("Toggle Grid Snap Alignment"));
    g_signal_connect (self->btn_snap_toggle, "clicked", G_CALLBACK (on_snap_toggled), self);
    gtk_box_append (GTK_BOX (grp_snap), self->btn_snap_toggle);

    const char *grid_units[] = { _("1/4 Note"), _("Bar"), _("1/8 Note"), _("1/16 Note"), _("Frame"), NULL };
    self->grid_unit_combo = gtk_drop_down_new_from_strings (grid_units);
    gtk_widget_set_tooltip_text (self->grid_unit_combo, _("Grid Unit Division"));
    gtk_box_append (GTK_BOX (grp_snap), self->grid_unit_combo);

    self->chk_adjust_markers = gtk_check_button_new_with_label (_("Adjust Markers"));
    gtk_check_button_set_active (GTK_CHECK_BUTTON (self->chk_adjust_markers), TRUE);
    gtk_box_append (GTK_BOX (grp_snap), self->chk_adjust_markers);

    gtk_box_append (GTK_BOX (hbox), grp_snap);

    gtk_box_append (GTK_BOX (hbox), gtk_separator_new (GTK_ORIENTATION_VERTICAL));

    /* 4. Timeline Cursor Position Locator */
    GtkWidget *loc_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_widget_add_css_class (loc_box, "loc-box");

    GtkWidget *btn_prev = gtk_button_new_from_icon_name ("go-previous-symbolic");
    gtk_widget_add_css_class (btn_prev, "flat");
    gtk_widget_set_tooltip_text (btn_prev, _("Jump to Previous Marker"));
    gtk_box_append (GTK_BOX (loc_box), btn_prev);

    self->lbl_cursor_pos = gtk_label_new ("00:00:00:00");
    gtk_widget_add_css_class (self->lbl_cursor_pos, "loc-text");
    gtk_box_append (GTK_BOX (loc_box), self->lbl_cursor_pos);

    GtkWidget *btn_next = gtk_button_new_from_icon_name ("go-next-symbolic");
    gtk_widget_add_css_class (btn_next, "flat");
    gtk_widget_set_tooltip_text (btn_next, _("Jump to Next Marker"));
    gtk_box_append (GTK_BOX (loc_box), btn_next);

    gtk_box_append (GTK_BOX (hbox), loc_box);

    gtk_box_append (GTK_BOX (hbox), gtk_separator_new (GTK_ORIENTATION_VERTICAL));

    /* 5. Zoom Level Controls — wired to workspace via set_workspace() injection */
    GtkWidget *zoom_box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 1);
    gtk_widget_add_css_class (zoom_box, "linked");

    self->btn_zoom_out = gtk_button_new_from_icon_name ("zoom-out-symbolic");
    gtk_widget_set_tooltip_text (self->btn_zoom_out, _("Zoom Out Timeline Horizontal Scale"));
    g_signal_connect (self->btn_zoom_out, "clicked", G_CALLBACK (on_zoom_out_clicked), self);
    gtk_box_append (GTK_BOX (zoom_box), self->btn_zoom_out);

    self->btn_zoom_in = gtk_button_new_from_icon_name ("zoom-in-symbolic");
    gtk_widget_set_tooltip_text (self->btn_zoom_in, _("Zoom In Timeline Horizontal Scale"));
    g_signal_connect (self->btn_zoom_in, "clicked", G_CALLBACK (on_zoom_in_clicked), self);
    gtk_box_append (GTK_BOX (zoom_box), self->btn_zoom_in);

    self->btn_zoom_full = gtk_button_new_from_icon_name ("zoom-fit-best-symbolic");
    gtk_widget_set_tooltip_text (self->btn_zoom_full, _("Reset Zoom to 1:1"));
    g_signal_connect (self->btn_zoom_full, "clicked", G_CALLBACK (on_zoom_full_clicked), self);
    gtk_box_append (GTK_BOX (zoom_box), self->btn_zoom_full);

    gtk_box_append (GTK_BOX (hbox), zoom_box);
}

GtkWidget *
ardour_editor_toolbar_new (void)
{
    return GTK_WIDGET (g_object_new (ARDOUR_TYPE_EDITOR_TOOLBAR, NULL));
}

void
ardour_editor_toolbar_set_workspace (ArdourEditorToolbar *self, GtkWidget *workspace)
{
    g_return_if_fail (ARDOUR_IS_EDITOR_TOOLBAR (self));
    g_return_if_fail (workspace != NULL);
    self->workspace = workspace;
}
