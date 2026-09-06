# ── adw_shell — Phase 2 GTK4 Main DAW Shell ───────────────
# Included via subdir() from gtk4_ardour/meson.build.

# ── System dependencies ──────────────────────────────────────
gtk4_dep      = dependency('gtk4',         required: true)
adw_dep       = dependency('libadwaita-1', required: true, version: '>=1.4')
glib_dep      = dependency('glib-2.0',     required: true)
glibmm_dep    = dependency('glibmm-2.4',   required: true)
sigcpp_dep    = dependency('sigc++-2.0',   required: true)
suil_dep      = dependency('suil-0',       required: false)

# ── Ardour source tree root ───────────────────────────────────
ardour_root = meson.project_source_root() / '..'
waf_build = ardour_root / 'build'

ardour_inc_flags = [
    '-I' + ardour_root / 'libs',
    '-I' + ardour_root / 'libs/ardour',
    '-I' + ardour_root / 'libs/pbd',
    '-I' + ardour_root / 'libs/temporal',
    '-I' + ardour_root / 'libs/evoral',
    '-I' + ardour_root / 'libs/midi++2',
    '-I' + ardour_root / 'libs/waveview',
    '-I' + ardour_root / 'libs/ardour/ardour',
    '-I' + ardour_root / 'libs/zita-resampler',
    '-I' + ardour_root / 'libs/libltc',
    '-I' + ardour_root / 'libs/libltc/ltc',
    '-I' + ardour_root / 'libs/lua',
]

ardour_lib_dir = waf_build / 'libs'

libardour = declare_dependency(
    link_args: [
        '-L' + ardour_lib_dir / 'ardour',
        '-lardour',
        '-Wl,-rpath,' + ardour_lib_dir / 'ardour',
    ],
)

libpbd = declare_dependency(
    link_args: [
        '-L' + ardour_lib_dir / 'pbd',
        '-lpbd',
        '-Wl,-rpath,' + ardour_lib_dir / 'pbd',
    ],
)

libtemporal = declare_dependency(
    link_args: [
        '-L' + ardour_lib_dir / 'temporal',
        '-ltemporal',
        '-Wl,-rpath,' + ardour_lib_dir / 'temporal',
    ],
)

libevoral = declare_dependency(
    link_args: [
        '-L' + ardour_lib_dir / 'evoral',
        '-levoral',
        '-Wl,-rpath,' + ardour_lib_dir / 'evoral',
    ],
)

libaudiographer = declare_dependency(
    link_args: [
        '-L' + ardour_lib_dir / 'audiographer',
        '-laudiographer',
        '-Wl,-rpath,' + ardour_lib_dir / 'audiographer',
    ],
)

libmidipp = declare_dependency(
    link_args: [
        '-L' + ardour_lib_dir / 'midi++2',
        '-lmidipp',
        '-Wl,-rpath,' + ardour_lib_dir / 'midi++2',
    ],
)

libptformat = declare_dependency(
    link_args: [
        '-L' + ardour_lib_dir / 'ptformat',
        '-lptformat',
        '-Wl,-rpath,' + ardour_lib_dir / 'ptformat',
    ],
)

# ── Build-time definitions ───────────────────────────────────
css_dir  = meson.current_source_dir() / '..' / 'shared'
locale_dir = '/usr/share/locale'

conf = configuration_data()
conf.set_quoted('ARDOUR4_SHARED_CSS_DIR', css_dir)
conf.set_quoted('ARDOUR4_LOCALE_DIR',     locale_dir)
conf.set_quoted('GETTEXT_PACKAGE',        'ardour')
conf.set('ENABLE_NLS', 1)
configure_file(output: 'ardour4_config.h', configuration: conf)

ardour4_config_inc = include_directories('.')

# ── Source files ─────────────────────────────────────────────
shell_sources = files(
    'main.cc',
    'shell_window.cc',
    'transport_bar.cc',
    'editor_toolbar.cc',
    'editor_workspace.cc',
    'status_bar.cc',
)

# ── Executable ───────────────────────────────────────────────
executable(
    'ardour4-shell',
    shell_sources,
    dependencies: [
        gtk4_dep,
        adw_dep,
        glib_dep,
        glibmm_dep,
        sigcpp_dep,
        suil_dep,
        libardour,
        libpbd,
        libtemporal,
        libevoral,
        libaudiographer,
        libmidipp,
        libptformat,
    ],
    include_directories: [
        ardour4_config_inc,
    ],
    cpp_args: [
        '-DHAVE_CONFIG_H',
        '-std=c++17',
        '-Wno-deprecated-declarations',
    ] + ardour_inc_flags,
    install: false,
)
