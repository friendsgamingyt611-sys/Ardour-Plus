import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import "components"

ApplicationWindow {
    id: mainWindow
    visible: true
    width: 1360
    height: 840
    title: "Ardour Plus DAW (Qt6 / QML)"

    property int startupState: 2 // 0: Splash Screen, 1: Session Setup Dialog, 2: Main Workspace

    background: Rectangle {
        color: Theme.bgWindow
    }

    // ──────────────── Startup Flow Overlay 1: Ardour Splash Screen ────────────────
    Rectangle {
        anchors.fill: parent
        color: Theme.bgHeader
        z: 999
        visible: mainWindow.startupState === 0

        SplashScreen {
            anchors.centerIn: parent
            onInitFinished: mainWindow.startupState = 2
            onOpenSetupRequested: mainWindow.startupState = 1
        }
    }

    // ──────────────── Startup Flow Overlay 2: Session Setup & Engine Manager Dialog ────────────────
    Rectangle {
        anchors.fill: parent
        color: Theme.bgHeader
        z: 998
        visible: mainWindow.startupState === 1

        SessionSetupDialog {
            anchors.centerIn: parent
            onLaunchWorkspaceRequested: {
                mainWindow.startupState = 2
            }
        }
    }

    // ──────────────── Main DAW Shell Workspace Container ────────────────
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        visible: mainWindow.startupState === 2

        // 1. Top Menu Bar (Session, Transport, Edit, Region, Track, View, Window, Help)
        TopMenuBar {
            id: topMenuBar
            Layout.fillWidth: true
        }

        // 2. Transport Header Control Panel (Row 1)
        TransportBar {
            id: transportBar
            Layout.fillWidth: true
        }

        // 3. Editor Toolbar (Row 2 Tools, Edit Modes, Snap, Clock Readout)
        EditorToolbar {
            id: editorToolbar
            Layout.fillWidth: true
            visible: globalSession.workspaceMode === 0 || globalSession.workspaceMode === 3
        }

        // 4. Central Workspace StackLayout (Switching between GTK2 Ardour Tabbable Modes)
        StackLayout {
            id: mainWorkspaceStack
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: globalSession.workspaceMode

            // ──────── Mode 0: Editor Timeline Workspace ────────
            ColumnLayout {
                spacing: 0

                SplitView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    orientation: Qt.Horizontal

                    // Column 1: Left Track Inspector (Active Channel Strip)
                    TrackInspector {
                        id: trackInspector
                        SplitView.minimumWidth: 150
                        SplitView.preferredWidth: 170
                        SplitView.maximumWidth: 245
                    }

                    // Column 2: Center Editor Workspace (Track Headers + Timeline Canvas)
                    SplitView {
                        SplitView.fillWidth: true
                        orientation: Qt.Horizontal

                        // Track Header List
                        TrackList {
                            id: trackList
                            SplitView.minimumWidth: 200
                            SplitView.preferredWidth: 240
                            SplitView.maximumWidth: 320
                        }

                        // Timeline Canvas (Ruler, Track Lanes, Audio Regions, Playhead)
                        TimelineCanvas {
                            id: timelineCanvas
                            SplitView.fillWidth: true
                        }
                    }

                    // Column 3: Right Sidebar (Inspector, Audio/MIDI Regions, Locations, Metering)
                    RightSidebar {
                        id: rightSidebar
                        SplitView.minimumWidth: 220
                        SplitView.preferredWidth: 260
                        SplitView.maximumWidth: 340
                    }
                }

                // Bottom Dock notebook (Mixer Strips / Audio Connections / MIDI Editor)
                Rectangle {
                    Layout.fillWidth: true
                    height: 160
                    color: Theme.bgWindow
                    border.color: Theme.borderDark

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 0

                        // Lower notebook Tab Bar
                        Rectangle {
                            Layout.fillWidth: true
                            height: 24
                            color: Theme.bgHeader

                            RowLayout {
                                anchors.fill: parent
                                anchors.leftMargin: 8
                                spacing: 2

                                Repeater {
                                    model: ["Mixer Strips", "Audio Connections", "MIDI Event List", "DSP & Plugins"]
                                    Rectangle {
                                        implicitWidth: 120
                                        implicitHeight: 22
                                        radius: 2
                                        color: lowerDockStack.currentIndex === index ? Theme.btnActive : Theme.btnNormal
                                        Text {
                                            anchors.centerIn: parent
                                            text: modelData
                                            color: lowerDockStack.currentIndex === index ? Theme.textMain : Theme.textMuted
                                            font.pixelSize: 10
                                            font.bold: true
                                        }
                                        MouseArea {
                                            anchors.fill: parent
                                            onClicked: lowerDockStack.currentIndex = index
                                        }
                                    }
                                }
                            }
                        }

                        // Lower Stack Content Pages
                        StackLayout {
                            id: lowerDockStack
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            currentIndex: 0

                            // Tab 0: Mixer Strips
                            MixerConsole {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }

                            // Tab 1: Audio Connections
                            AudioMatrix {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }

                            // Tab 2: MIDI Event List
                            Rectangle {
                                color: Theme.bgWindow
                                RowLayout {
                                    anchors.fill: parent; anchors.margins: 10
                                    Text { text: "MIDI Note Event Inspector & Controller Curves"; color: Theme.textMuted; font.pixelSize: 11 }
                                }
                            }

                            // Tab 3: Plugins & DSP
                            Rectangle {
                                color: Theme.bgWindow
                                RowLayout {
                                    anchors.fill: parent; anchors.margins: 10
                                    Text { text: "Plugin Insert Rack & Signal Flow Matrix"; color: Theme.textMuted; font.pixelSize: 11 }
                                }
                            }
                        }
                    }
                }
            }

            // ──────── Mode 1: Dedicated Full Mixer View (Mixer_UI) ────────
            MixerConsole {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            // ──────── Mode 2: Dedicated Recorder View (RecorderUI) ────────
            RecorderView {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            // ──────── Mode 3: Cues / Trigger Page View (TriggerPage) ────────
            TriggerPage {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            // ──────── Mode 4: Setup & Preferences View (RCOptionEditor) ────────
            PreferencesView {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

        // 6. Bottom Status Bar
        StatusBar {
            id: statusBar
            Layout.fillWidth: true
        }
    }

    // Modal Add Track Dialog
    AddTrackDialog {
        id: addTrackDialog
    }

    Connections {
        target: globalSession
        function onRequestAddTrackDialog() {
            addTrackDialog.open()
        }
    }
}
