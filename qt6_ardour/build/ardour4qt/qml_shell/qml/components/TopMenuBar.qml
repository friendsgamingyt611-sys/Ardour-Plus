import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import ".."

Rectangle {
    id: topMenuBarRoot
    implicitHeight: 24
    color: Theme.bgHeader

    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 1
        color: Theme.borderDark
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 6
        anchors.rightMargin: 8
        spacing: 1

        // Main Menu Items with Dropdown Popup Menus
        Repeater {
            model: [
                { title: "Session",   items: ["New Session", "Open Session", "Save", "Save As...", "Export"] },
                { title: "Transport", items: ["Play / Stop", "Record Arm", "Rewind to Start", "Fast Forward"] },
                { title: "Edit",      items: ["Undo", "Redo", "Cut", "Copy", "Paste", "Select All"] },
                { title: "Region",    items: ["Split at Playhead", "Duplicate", "Nudge Right", "Nudge Left"] },
                { title: "Track",     items: ["Add Track/Bus", "Duplicate Selected", "Remove Selected"] },
                { title: "View",      items: ["Toggle Mixer Panel", "Toggle Region Detail", "Zoom In", "Zoom Out"] },
                { title: "Window",    items: ["Mixer Console", "Audio Region Editor", "MIDI Piano Roll", "Plugins"] },
                { title: "Help",      items: ["About Ardour Plus", "Keyboard Shortcuts", "Documentation"] }
            ]

            Rectangle {
                id: menuItemRoot
                implicitWidth: menuText.contentWidth + 14
                implicitHeight: 20
                radius: 2
                color: menuMouse.containsMouse || menuPopup.opened ? Theme.bgCard : "transparent"

                Text {
                    id: menuText
                    anchors.centerIn: parent
                    text: modelData.title
                    color: Theme.textMain
                    font.pixelSize: 11
                }

                Menu {
                    id: menuPopup
                    y: menuItemRoot.height

                    Repeater {
                        model: modelData.items
                        MenuItem {
                            text: modelData
                            onTriggered: globalSession.triggerMenuAction(modelData.title, modelData)
                        }
                    }
                }

                MouseArea {
                    id: menuMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: menuPopup.open()
                }
            }
        }


        Item { Layout.fillWidth: true }

        // Audio Engine & DSP Telemetry (Bound to C++ globalSession properties)
        RowLayout {
            spacing: 10

            RowLayout {
                spacing: 4
                Rectangle {
                    width: 6; height: 6; radius: 3
                    color: globalSession.engineRunning ? Theme.clockGreen : Theme.muteRed
                }
                Text {
                    text: "Audio: " + (globalSession.sampleRate / 1000.0).toFixed(1) + " kHz / " + globalSession.bufferLatencyMs.toFixed(1) + " ms"
                    color: Theme.textMuted
                    font.pixelSize: 10
                    font.family: "Monospace"
                }
            }

            Text {
                text: "Rec: >24h"
                color: Theme.soloOrange
                font.pixelSize: 10
                font.family: "Monospace"
                font.bold: true
            }

            RowLayout {
                spacing: 4
                Text { text: "DSP:"; color: Theme.textMuted; font.pixelSize: 10 }
                Rectangle {
                    width: 40; height: 6; radius: 2; color: Theme.bgHeader
                    Rectangle {
                        width: parent.width * (globalSession.dspLoad / 100.0)
                        height: parent.height; radius: 2
                        color: globalSession.dspLoad > 85 ? Theme.muteRed : Theme.clockBlue
                    }
                }
                Text {
                    text: globalSession.dspLoad.toFixed(0) + "% (1)"
                    color: Theme.textMain
                    font.pixelSize: 10
                    font.family: "Monospace"
                }
            }
        }
    }
}
