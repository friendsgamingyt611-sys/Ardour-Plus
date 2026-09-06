# ── adw_prefs — Phase 0 GTK4 preferences window ─────────────
# Included via subdir() from gtk4_ardour/meson.build.
# Do NOT add a project() call here — only the parent meson.build may do that.

# ── System dependencies ──────────────────────────────────────
gtk4_dep      = dependency('gtk4',         required: true)
adw_dep       = dependency('libadwaita-1', required: true, version: '>=1.4')
glib_dep      = dependency('glib-2.0',     required: true)
glibmm_dep    = dependency('glibmm-2.4',   required: true)  # needed by libardour/pbd headers
sigcpp_dep    = dependency('sigc++-2.0',   required: true)
suil_dep      = dependency('suil-0',       required: false)

# ── Ardour source tree root ───────────────────────────────────
# meson.project_source_root() now points to gtk4_ardour/ (the parent project).
# One level up from gtk4_ardour/ is the Arduor-Plus repository root.
ardour_root = meson.project_source_root() / '..'

# ── Paths into the existing WAF build output ─────────────────
# The WAF build puts .so files in build/libs/<name>/
waf_build = ardour_root / 'build'

# ── Include directories (mirrors WAF CPPPATH) ────────────────
# Meson 1.x does not allow include_directories() with absolute paths outside
# the project source root. External Ardour headers are passed as raw -I flags
# in cpp_args below instead.
ardour_inc_flags = [
    '-I' + ardour_root / 'libs/ardour',
    '-I' + ardour_root / 'libs/pbd',
    '-I' + ardour_root / 'libs/temporal',
    '-I' + ardour_root / 'libs/evoral',
    '-I' + ardour_root / 'libs/midi++2',
    '-I' + ardour_root / 'libs/waveview',
    # Needed for SessionEvent, TriggerBox etc included transitively
    '-I' + ardour_root / 'libs/ardour/ardour',
    # zita-resampler is bundled in-tree; audioengine.h pulls it in transitively
    '-I' + ardour_root / 'libs/zita-resampler',
]

# ── Link against the existing WAF-built shared libraries ─────
# We find them via the known output directory, not pkg-config,
# because they are not installed system-wide.
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
# meson.current_source_dir() = gtk4_ardour/adw_prefs/; one level up = gtk4_ardour/shared/
css_dir  = meson.current_source_dir() / '..' / 'shared'
locale_dir = '/usr/share/locale'   # fallback; override at install time

conf = configuration_data()
conf.set_quoted('ARDOUR4_SHARED_CSS_DIR', css_dir)
conf.set_quoted('ARDOUR4_LOCALE_DIR',     locale_dir)
conf.set_quoted('GETTEXT_PACKAGE',        'ardour')
conf.set('ENABLE_NLS', 1)
configure_file(output: 'ardour4_config.h', configuration: conf)

ardour4_config_inc = include_directories('.')  # picks up ardour4_config.h from builddir

# ── Source files ─────────────────────────────────────────────
prefs_sources = files(
    'main.cc',
    'prefs_window.cc',
)

# ── Executable ───────────────────────────────────────────────
executable(
    'ardour4-prefs',
    prefs_sources,
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
        # Silence the flood of gtkmm2/sigc++ warnings from libardour headers
        '-Wno-deprecated-declarations',
    ] + ardour_inc_flags,
    install: false,   # standalone dev binary, not installed
)
