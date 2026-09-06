import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import ".."

Rectangle {
    id: editorToolbarRoot
    implicitHeight: 30
    color: Theme.bgPanel

    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 1
        color: Theme.borderDark
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 6
        anchors.rightMargin: 6
        spacing: 4

        // 1. Edit Mode Buttons (Slide, Ripple, Lock)
        RowLayout {
            spacing: 2

            // Slide Mode (0)
            Rectangle {
                width: 50; height: 22; radius: 2
                color: globalSession?.editMode === 0 ? Theme.btnActive : Theme.btnNormal
                border.color: globalSession?.editMode === 0 ? Theme.btnActive : Theme.borderDark
                RowLayout {
                    anchors.centerIn: parent
                    spacing: 3
                    Text { text: "↔"; color: globalSession?.editMode === 0 ? Theme.textMain : Theme.textMuted; font.pixelSize: 10 }
                    Text { text: "Slide"; color: globalSession?.editMode === 0 ? Theme.textMain : Theme.textMuted; font.pixelSize: 10; font.bold: true }
                }
                MouseArea { anchors.fill: parent; onClicked: globalSession?.set_edit_mode(0) }
            }

            // Ripple Mode (1)
            Rectangle {
                width: 52; height: 22; radius: 2
                color: globalSession?.editMode === 1 ? Theme.btnActive : Theme.btnNormal
                border.color: globalSession?.editMode === 1 ? Theme.btnActive : Theme.borderDark
                RowLayout {
                    anchors.centerIn: parent
                    spacing: 3
                    Text { text: "➔"; color: globalSession?.editMode === 1 ? Theme.textMain : Theme.textMuted; font.pixelSize: 10 }
                    Text { text: "Ripple"; color: globalSession?.editMode === 1 ? Theme.textMain : Theme.textMuted; font.pixelSize: 10; font.bold: true }
                }
                MouseArea { anchors.fill: parent; onClicked: globalSession?.set_edit_mode(1) }
            }

            // Lock Toggle (2)
            Rectangle {
                width: 48; height: 22; radius: 2
                color: globalSession?.editMode === 2 ? Theme.recRed : Theme.btnNormal
                border.color: globalSession?.editMode === 2 ? Theme.recRed : Theme.borderDark
                RowLayout {
                    anchors.centerIn: parent
                    spacing: 3
                    Image {
                        width: 11; height: 11
                        source: globalSession?.editMode === 2 ? "../icons/padlock_closed.png" : "../icons/padlock_open.png"
                        fillMode: Image.PreserveAspectFit
                    }
                    Text { text: "Lock"; color: globalSession?.editMode === 2 ? Theme.textMain : Theme.textMuted; font.pixelSize: 10; font.bold: true }
                }
                MouseArea { anchors.fill: parent; onClicked: globalSession?.set_edit_mode(globalSession?.editMode === 2 ? 0 : 2) }
            }
        }

        Rectangle { width: 1; height: 18; color: Theme.borderDark }

        // 2. Tool Palette Buttons with EXACT Ardour Icons
        RowLayout {
            spacing: 2

            Repeater {
                model: [
                    { name: "Select (Object)", icon: "../icons/grabber.png" },
                    { name: "Cut (Range)",     icon: "../icons/scissors.png" },
                    { name: "Stretch (Time)",  icon: "../icons/time_exp.png" },
                    { name: "Draw (Notes)",    icon: "../icons/midi_tool_pencil.png" },
                    { name: "Zoom",            icon: "../icons/zoom_in_cursor.png" },
                    { name: "Edit Content",    icon: "../icons/midi_tool_select.png" }
                ]

                Rectangle {
                    width: 24
                    height: 22
                    radius: 2
                    color: globalSession?.mouseMode === index ? Theme.btnActive : Theme.btnNormal
                    border.color: globalSession?.mouseMode === index ? Theme.btnActive : Theme.borderDark
                    ToolTip.visible: toolMouseArea.containsMouse
                    ToolTip.text: modelData.name

                    Image {
                        anchors.centerIn: parent
                        width: 14; height: 14
                        source: modelData.icon
                        fillMode: Image.PreserveAspectFit
                    }

                    MouseArea {
                        id: toolMouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: globalSession?.set_mouse_mode(index)
                    }
                }
            }
        }

        Rectangle { width: 1; height: 18; color: Theme.borderDark }

        // 3. Snap & Grid Settings
        RowLayout {
            spacing: 3

            // Snap Switch Button
            Rectangle {
                width: 44; height: 22; radius: 2
                color: globalSession?.snapEnabled ? Theme.clockGreen : Theme.btnNormal
                border.color: globalSession?.snapEnabled ? Theme.clockGreen : Theme.borderDark
                Text {
                    anchors.centerIn: parent
                    text: "Snap"
                    color: globalSession?.snapEnabled ? Theme.textMain : Theme.textMuted
                    font.pixelSize: 10
                    font.bold: true
                }
                MouseArea { anchors.fill: parent; onClicked: globalSession?.toggle_snap() }
            }

            // Grid Unit Dropdown
            ComboBox {
                implicitWidth: 78
                implicitHeight: 22
                model: ["1/4 Note", "1/8 Note", "1/16 Note", "1/32 Note", "Bar", "Beat"]
                currentIndex: (globalSession && model.indexOf(globalSession.gridUnit) >= 0) ? model.indexOf(globalSession.gridUnit) : 2
                onActivated: globalSession?.set_grid_unit(currentText)

                background: Rectangle {
                    color: Theme.btnNormal
                    border.color: Theme.borderDark
                    radius: 2
                }
                contentItem: Text {
                    text: parent.displayText
                    color: Theme.clockGreen
                    font.pixelSize: 9
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }

        Rectangle { width: 1; height: 18; color: Theme.borderDark }

        // 4. Position Readout Clock (< > 00:00:04:17)
        RowLayout {
            spacing: 2

            Rectangle {
                width: 18; height: 20; radius: 2
                color: Theme.btnNormal; border.color: Theme.borderDark
                Text { anchors.centerIn: parent; text: "‹"; color: Theme.textMuted; font.pixelSize: 12; font.bold: true }
                MouseArea { anchors.fill: parent; onClicked: globalTransport?.rewind() }
            }

            Rectangle {
                width: 18; height: 20; radius: 2
                color: Theme.btnNormal; border.color: Theme.borderDark
                Text { anchors.centerIn: parent; text: "›"; color: Theme.textMuted; font.pixelSize: 12; font.bold: true }
                MouseArea { anchors.fill: parent; onClicked: globalTransport?.fast_forward() }
            }

            Rectangle {
                width: 90; height: 22; radius: 2
                color: Theme.bgHeader; border.color: Theme.borderDark
                Text {
                    anchors.centerIn: parent
                    text: globalTransport?.timecodeString || "00:00:00:00"
                    color: Theme.clockBlue
                    font.pixelSize: 11
                    font.family: "Monospace"
                    font.bold: true
                }
            }
        }

        Item { Layout.fillWidth: true }

        // 5. Zoom Controls
        RowLayout {
            spacing: 3

            Rectangle {
                width: 22; height: 22; radius: 2
                color: Theme.btnNormal; border.color: Theme.borderDark
                Text { anchors.centerIn: parent; text: "−"; color: Theme.textMain; font.pixelSize: 13; font.bold: true }
                MouseArea { anchors.fill: parent; onClicked: globalTrackModel?.zoom_out() }
            }

            Text { text: "Zoom"; color: Theme.textMuted; font.pixelSize: 10 }

            Rectangle {
                width: 22; height: 22; radius: 2
                color: Theme.btnNormal; border.color: Theme.borderDark
                Text { anchors.centerIn: parent; text: "+"; color: Theme.textMain; font.pixelSize: 13; font.bold: true }
                MouseArea { anchors.fill: parent; onClicked: globalTrackModel?.zoom_in() }
            }
        }
    }
}
