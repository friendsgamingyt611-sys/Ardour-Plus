project(
    'ardour-gtk4',
    ['c', 'cpp'],
    version: '0.1.0',
    meson_version: '>=1.0',
    default_options: [
        'c_std=c17',
        'cpp_std=c++17',
        'warning_level=2',
        'buildtype=debugoptimized',
    ],
)

# Each phase lives in its own subdirectory with its own meson.build.
# Phases are independent — build only what you need with:
#   meson setup builddir && ninja -C builddir ardour4-prefs

subdir('adw_prefs')
subdir('adw_dialogs')   # Phase 1
subdir('adw_shell')     # Phase 2
# subdir('adw_clocks')    # Phase 3
# subdir('adw_mixer')     # Phase 4
# subdir('adw_trigger')   # Phase 5
# subdir('adw_pianoroll') # Phase 6
# subdir('adw_editor')    # Phase 7
