cmake_minimum_required(VERSION 3.22)
project(ardour4-qt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2 Svg)
qt_standard_project_setup(REQUIRES 6.5)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIBMM REQUIRED glibmm-2.4)
pkg_check_modules(SIGCPP REQUIRED sigc++-2.0)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(LIBXML REQUIRED libxml-2.0)

# ─── Ardour C++ Core Includes & Linkage ──────────────────────────────────────
set(ARDOUR_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(ARDOUR_BUILD_LIBS "${ARDOUR_ROOT}/build/libs")

set(ARDOUR_INCLUDES
    "${ARDOUR_ROOT}/libs/ardour"
    "${ARDOUR_ROOT}/libs/pbd"
    "${ARDOUR_ROOT}/libs/temporal"
    "${ARDOUR_ROOT}/libs/evoral"
    "${ARDOUR_ROOT}/libs/midi++2"
    "${ARDOUR_ROOT}/libs/audiographer"
    "${ARDOUR_ROOT}/libs/waveview"
    "${ARDOUR_ROOT}/libs/ardour/ardour"
    "${ARDOUR_ROOT}/libs/zita-resampler"
    "${ARDOUR_ROOT}/libs/libltc"
    "${ARDOUR_ROOT}/libs/libltc/ltc"
    "${ARDOUR_ROOT}/libs/lua"
    "${ARDOUR_ROOT}/libs/ptformat"
    "${ARDOUR_ROOT}/libs/aaf"
    "${ARDOUR_ROOT}/libs/appleutility"
    "${ARDOUR_ROOT}/libs/vamp-sdk"
    "${ARDOUR_ROOT}/libs/qm-dsp"
    "${ARDOUR_ROOT}/build"
    ${GLIBMM_INCLUDE_DIRS}
    ${SIGCPP_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
    ${LIBXML_INCLUDE_DIRS}
)




# ─── C++ Sources ──────────────────────────────────────────────────────────────
set(CPP_SOURCES
    qml_shell/main.cpp
    qml_shell/transport_bar.cpp
    qml_shell/editor_workspace.cpp
    qml_shell/session_bridge.cpp
    qml_shell/models/qt_route.cpp
    qml_shell/theme.h
)

# ─── QML Sources ──────────────────────────────────────────────────────────────
set(QML_FILES
    qml_shell/qml/MainShell.qml
    qml_shell/qml/components/TopMenuBar.qml
    qml_shell/qml/components/TransportBar.qml
    qml_shell/qml/components/EditorToolbar.qml
    qml_shell/qml/components/TrackInspector.qml
    qml_shell/qml/components/TrackList.qml
    qml_shell/qml/components/TimelineCanvas.qml
    qml_shell/qml/components/RightSidebar.qml
    qml_shell/qml/components/StatusBar.qml
    qml_shell/qml/components/AddTrackDialog.qml
    qml_shell/qml/components/SplashScreen.qml
    qml_shell/qml/components/SessionSetupDialog.qml
    qml_shell/qml/components/MixerConsole.qml
    qml_shell/qml/components/AudioMatrix.qml
    qml_shell/qml/components/RecorderView.qml
    qml_shell/qml/components/TriggerPage.qml
    qml_shell/qml/components/PreferencesView.qml
    qml_shell/qml/components/MixerStrip.qml
)

set(ICON_FILES
    qml_shell/qml/icons/grabber.png
    qml_shell/qml/icons/scissors.png
    qml_shell/qml/icons/time_exp.png
    qml_shell/qml/icons/midi_tool_pencil.png
    qml_shell/qml/icons/zoom_in_cursor.png
    qml_shell/qml/icons/midi_tool_select.png
    qml_shell/qml/icons/padlock_closed.png
    qml_shell/qml/icons/padlock_open.png
    qml_shell/qml/icons/rec-enabled.png
    qml_shell/qml/icons/record_normal_red.png
    qml_shell/qml/icons/mute-enabled.png
    qml_shell/qml/icons/solo-enabled.png
    qml_shell/qml/icons/transport_play.svg
    qml_shell/qml/icons/transport_stop.svg
    qml_shell/qml/icons/transport_record.svg
    qml_shell/qml/icons/transport_start.svg
    qml_shell/qml/icons/transport_end.svg
    qml_shell/qml/icons/transport_range.svg
    qml_shell/qml/icons/metronome.svg
    qml_shell/qml/icons/midi_panic.svg
)

qt_add_executable(ardour4-qt ${CPP_SOURCES})

qt_add_qml_module(ardour4-qt
    URI ardour4qt
    VERSION 1.0
    RESOURCE_PREFIX "/qt/qml"
    QML_FILES ${QML_FILES}
    RESOURCES ${ICON_FILES}
)

target_include_directories(ardour4-qt PRIVATE qml_shell ${ARDOUR_INCLUDES})

target_compile_definitions(ardour4-qt PRIVATE HAVE_CONFIG_H)

target_link_directories(ardour4-qt PRIVATE
    "${ARDOUR_BUILD_LIBS}/ardour"
    "${ARDOUR_BUILD_LIBS}/pbd"
    "${ARDOUR_BUILD_LIBS}/temporal"
    "${ARDOUR_BUILD_LIBS}/evoral"
    "${ARDOUR_BUILD_LIBS}/audiographer"
    "${ARDOUR_BUILD_LIBS}/midi++2"
    "${ARDOUR_BUILD_LIBS}/ptformat"
    "${ARDOUR_BUILD_LIBS}/tk/suil"
)

set(RPATH_LIST
    "${ARDOUR_BUILD_LIBS}/ardour"
    "${ARDOUR_BUILD_LIBS}/pbd"
    "${ARDOUR_BUILD_LIBS}/temporal"
    "${ARDOUR_BUILD_LIBS}/evoral"
    "${ARDOUR_BUILD_LIBS}/audiographer"
    "${ARDOUR_BUILD_LIBS}/midi++2"
    "${ARDOUR_BUILD_LIBS}/ptformat"
    "${ARDOUR_BUILD_LIBS}/tk/suil"
)
string(JOIN ":" RPATH_STR ${RPATH_LIST})
set_target_properties(ardour4-qt PROPERTIES BUILD_RPATH "${RPATH_STR}" INSTALL_RPATH "${RPATH_STR}")

target_link_libraries(ardour4-qt PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Svg
    ${GLIBMM_LIBRARIES}
    ${SIGCPP_LIBRARIES}
    ${GLIB_LIBRARIES}
    ${LIBXML_LIBRARIES}
    ardour
    pbd
    temporal
    evoral
    audiographer
    midipp
    ptformat
    suil
)

# Optional: Install
install(TARGETS ardour4-qt DESTINATION bin)


