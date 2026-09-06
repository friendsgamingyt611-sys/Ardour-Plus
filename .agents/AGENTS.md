# Ardour GTK2 to Qt6 Porting Protocol & Strict Rules

## Core Rule: Empirical GTK2 Measurement First
Before writing a single line of QML or C++ ViewModel for any screen, panel, strip, or component:
1. **GTK2 Reference Capture**: Open the target screen in real GTK2 Ardour with a populated multi-track test session loaded.
2. **Reference Artifact Standard**:
   - High-resolution screenshot of the GTK2 UI in dark theme with live audio/MIDI session data.
   - Exact widget hierarchy extracted from GTK2 source (`gtk2_ardour/*.cc`), including class names, pixel coordinates, dimensions, exact string labels, and mouse/drag event handlers.
   - Comprehensive inventory of keyboard shortcuts, modifier key bindings (Shift/Ctrl/Alt click/drag), and right-click context menus.
3. **Fidelity Goal**: Match pixel geometry, spacing, density, labels, and interaction patterns 1:1. Modernization means GPU-accelerated Qt6/QML rendering with crisp typography and modern dark colors—NOT redesigning or altering the layout.

## Verification & Quality Discipline
1. **Empirical Proof over Self-Grading**: Never claim "100% ported" or "fully live" without supplying the exact executable command, test action sequence, and loaded session file.
2. **Real Multi-Track Session Testing**: All tests must use a populated `.ardour` session file containing multiple audio/MIDI tracks, fader automation, and plugin inserts. Zero testing on empty sessions.
3. **Comparison Check-in Standard**: Each milestone submission must present:
   `Real GTK2 Screenshot | Qt6 QML Screenshot | Explicit List of Interaction & Geometry Audits`
4. **Dynamic Data Validation**: Any track, bus, plugin, or send list must be dynamically generated from `libardour` engine objects at runtime and proven by live addition/removal events.
5. **Phase-Gated Execution**: Complete and verify Phase N before commencing Phase N+1. No moving forward with unresolved layout or behavioral gaps.
