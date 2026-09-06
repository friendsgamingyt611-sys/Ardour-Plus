import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Rectangle {
    id: splashRoot
    width: 600
    height: 380
    color: "#181a20"
    border.color: "#2a2f3a"
    border.width: 1
    radius: 6

    signal initFinished()
    signal openSetupRequested()

    property real progress: 0.0
    property string statusText: "Initializing Core Engine..."

    Timer {
        id: initTimer
        interval: 150
        running: true
        repeat: true
        onTriggered: {
            splashRoot.progress += 0.08
            if (splashRoot.progress < 0.25) {
                splashRoot.statusText = "Loading Core Engine & PBD subsystems..."
            } else if (splashRoot.progress < 0.50) {
                splashRoot.statusText = "Discovering Audio/MIDI Interfaces (ALSA/JACK)..."
            } else if (splashRoot.progress < 0.75) {
                splashRoot.statusText = "Loading Session Templates & Preferences..."
            } else if (splashRoot.progress < 1.0) {
                splashRoot.statusText = "Engine Ready. Preparing Startup Flow..."
            } else {
                initTimer.stop()
                splashRoot.progress = 1.0
                splashRoot.statusText = "Ready."
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: 16

        // Ardour Graphic Header Banner
        Rectangle {
            Layout.fillWidth: true
            height: 120
            color: "#23272e"
            radius: 4
            border.color: "#2d323e"

            RowLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 16

                // Ardour Red Accent Icon Badge
                Rectangle {
                    width: 64; height: 64; radius: 8
                    color: "#b91c1c"
                    Text {
                        anchors.centerIn: parent
                        text: "A+"
                        color: "#ffffff"
                        font.pixelSize: 28
                        font.bold: true
                    }
                }

                ColumnLayout {
                    spacing: 4
                    Text {
                        text: "ARDOUR PLUS"
                        color: "#e2e8f0"
                        font.pixelSize: 24
                        font.bold: true
                        font.letterSpacing: 2
                    }
                    Text {
                        text: "Professional Digital Audio Workstation — Version 8.6.0"
                        color: "#8f99ad"
                        font.pixelSize: 11
                    }
                    Text {
                        text: "Qt6 Engine Shell — High-Performance Core DAW"
                        color: "#64748b"
                        font.pixelSize: 10
                    }
                }
            }
        }

        // Subsystem Initialization Box
        Rectangle {
            Layout.fillWidth: true
            height: 90
            color: "#1b1e24"
            radius: 4
            border.color: "#282d38"

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 8

                RowLayout {
                    Layout.fillWidth: true
                    Text {
                        text: splashRoot.statusText
                        color: "#9aa4b8"
                        font.pixelSize: 11
                        Layout.fillWidth: true
                    }
                    Text {
                        text: Math.round(splashRoot.progress * 100) + "%"
                        color: "#00e676"
                        font.pixelSize: 11
                        font.bold: true
                        font.family: "Monospace"
                    }
                }

                // Progress Bar
                Rectangle {
                    Layout.fillWidth: true
                    height: 8
                    radius: 4
                    color: "#121418"

                    Rectangle {
                        height: parent.height
                        width: parent.width * Math.min(1.0, splashRoot.progress)
                        radius: 4
                        color: "#1976d2"
                    }
                }
            }
        }

        // Action Buttons Row
        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Button {
                text: "Open Session & Engine Setup"
                Layout.fillWidth: true
                enabled: splashRoot.progress >= 1.0
                onClicked: splashRoot.openSetupRequested()
            }

            Button {
                text: "Direct to DAW Workspace"
                Layout.fillWidth: true
                enabled: splashRoot.progress >= 1.0
                onClicked: splashRoot.initFinished()
            }
        }
    }
}
