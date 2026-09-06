import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import ".."

Rectangle {
    id: transportBarRoot
    implicitHeight: 46
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
        spacing: 6

        // 1. InL Toggle
        Rectangle {
            width: 26; height: 26; radius: 2
            color: Theme.btnNormal; border.color: Theme.borderDark
            Text { anchors.centerIn: parent; text: "InL"; color: Theme.textMuted; font.pixelSize: 10; font.bold: true }
        }

        // 2. Main Transport Buttons (Start, End, Stop, Play, Rec)
        RowLayout {
            spacing: 2

            // Start
            Rectangle {
                width: 26; height: 26; radius: 2
                color: Theme.btnNormal; border.color: Theme.borderDark
                Image { anchors.centerIn: parent; width: 14; height: 14; source: "../icons/transport_start.svg"; fillMode: Image.PreserveAspectFit }
                MouseArea { anchors.fill: parent; onClicked: globalTransport.rewind() }
            }

            // End
            Rectangle {
                width: 26; height: 26; radius: 2
                color: Theme.btnNormal; border.color: Theme.borderDark
                Image { anchors.centerIn: parent; width: 14; height: 14; source: "../icons/transport_end.svg"; fillMode: Image.PreserveAspectFit }
                MouseArea { anchors.fill: parent; onClicked: globalTransport?.fast_forward() }
            }

            // Stop
            Rectangle {
                width: 32; height: 26; radius: 2
                color: !globalTransport?.isPlaying ? Theme.bgCard : Theme.btnNormal
                border.color: !globalTransport?.isPlaying ? Theme.btnActive : Theme.borderDark
                Image { anchors.centerIn: parent; width: 14; height: 14; source: "../icons/transport_stop.svg"; fillMode: Image.PreserveAspectFit }
                MouseArea { anchors.fill: parent; onClicked: globalTransport?.stop() }
            }

            // Play
            Rectangle {
                width: 34; height: 26; radius: 2
                color: globalTransport?.isPlaying ? Theme.clockGreen : Theme.btnNormal
                border.color: globalTransport?.isPlaying ? Theme.clockGreen : Theme.borderDark
                Image { anchors.centerIn: parent; width: 16; height: 16; source: "../icons/transport_play.svg"; fillMode: Image.PreserveAspectFit }
                MouseArea { anchors.fill: parent; onClicked: globalTransport?.play() }
            }

            // Record Arm
            Rectangle {
                width: 32; height: 26; radius: 2
                color: globalTransport?.isRecording ? Theme.recRed : Theme.btnNormal
                border.color: globalTransport?.isRecording ? Theme.recRed : Theme.borderDark
                Image { anchors.centerIn: parent; width: 14; height: 14; source: "../icons/transport_record.svg"; fillMode: Image.PreserveAspectFit }
                MouseArea { anchors.fill: parent; onClicked: globalTransport?.toggle_rec() }
            }
        }

        // 3. Shuttle & VS Control
        RowLayout {
            spacing: 2
            Rectangle {
                width: 24; height: 24; radius: 2
                color: Theme.btnNormal; border.color: Theme.borderDark
                Text { anchors.centerIn: parent; text: "VS"; color: Theme.textMuted; font.pixelSize: 10 }
            }
            Slider {
                implicitWidth: 50; implicitHeight: 14
                from: -4.0; to: 4.0; value: globalTransport?.shuttleSpeed || 1.0
                onValueChanged: if (globalTransport) globalTransport.shuttleSpeed = value
            }
            Text { text: (globalTransport?.shuttleSpeed || 1.0).toFixed(1) + "x"; color: Theme.soloOrange; font.pixelSize: 9; font.family: "Monospace" }
        }

        Rectangle { width: 1; height: 24; color: Theme.borderDark }

        // 4. Punch & PDC Toggles
        RowLayout {
            spacing: 2
            Rectangle {
                width: 50; height: 24; radius: 2
                color: globalTransport?.punchEnabled ? Theme.recRed : Theme.btnNormal
                border.color: globalTransport?.punchEnabled ? Theme.recRed : Theme.borderDark
                Text { anchors.centerIn: parent; text: "Punch: in"; color: globalTransport?.punchEnabled ? Theme.textMain : Theme.textMuted; font.pixelSize: 9 }
                MouseArea { anchors.fill: parent; onClicked: globalTransport?.toggle_punch() }
            }
            Rectangle {
                width: 65; height: 24; radius: 2
                color: Theme.btnNormal; border.color: Theme.borderDark
                Text { anchors.centerIn: parent; text: "Disable PDC"; color: Theme.textMuted; font.pixelSize: 9 }
            }
        }

        // 5. Follow Range & Auto Return
        RowLayout {
            spacing: 2
            Rectangle {
                width: 66; height: 24; radius: 2
                color: globalTransport?.followRangeEnabled ? Theme.btnActive : Theme.btnNormal
                border.color: globalTransport?.followRangeEnabled ? Theme.btnActive : Theme.borderDark
                Text { anchors.centerIn: parent; text: "Follow Range"; color: globalTransport?.followRangeEnabled ? Theme.textMain : Theme.textMuted; font.pixelSize: 9 }
                MouseArea { anchors.fill: parent; onClicked: globalTransport?.toggle_follow_range() }
            }
            Rectangle {
                width: 62; height: 24; radius: 2
                color: globalTransport?.autoReturnEnabled ? Theme.clockGreen : Theme.btnNormal
                border.color: globalTransport?.autoReturnEnabled ? Theme.clockGreen : Theme.borderDark
                Text { anchors.centerIn: parent; text: "Auto Return"; color: globalTransport?.autoReturnEnabled ? Theme.textMain : Theme.textMuted; font.pixelSize: 9 }
                MouseArea { anchors.fill: parent; onClicked: globalTransport?.toggle_auto_return() }
            }
        }

        Rectangle { width: 1; height: 24; color: Theme.borderDark }

        // 6. BBT & Timecode LED Displays (Matching official Ardour slate LED style)
        RowLayout {
            spacing: 4

            // BBT LED Box
            Rectangle {
                width: 110; height: 32; radius: 3
                color: Theme.bgHeader; border.color: Theme.borderDark
                ColumnLayout {
                    anchors.centerIn: parent; spacing: 0
                    Text { text: globalTransport?.bbtString || "001|01|000"; color: Theme.clockGreen; font.pixelSize: 13; font.family: "Monospace"; font.bold: true; Layout.alignment: Qt.AlignHCenter }
                    Text { text: "♩=120.000  TS: 4/4"; color: Theme.textMuted; font.pixelSize: 8; Layout.alignment: Qt.AlignHCenter }
                }
            }

            // Timecode LED Box
            Rectangle {
                width: 115; height: 32; radius: 3
                color: Theme.bgHeader; border.color: Theme.borderDark
                ColumnLayout {
                    anchors.centerIn: parent; spacing: 0
                    Text { text: globalTransport?.timecodeString || "00:00:00:00"; color: Theme.clockBlue; font.pixelSize: 13; font.family: "Monospace"; font.bold: true; Layout.alignment: Qt.AlignHCenter }
                    Text { text: "INT / M-Clk"; color: Theme.textMuted; font.pixelSize: 8; Layout.alignment: Qt.AlignHCenter }
                }
            }
        }

        // 7. Loop / Punch Bounds Readout Table
        ColumnLayout {
            spacing: 1
            RowLayout {
                spacing: 4
                Text { text: "Start:"; color: Theme.textMuted; font.pixelSize: 8 }
                Text { text: "001|01|000"; color: Theme.clockGreen; font.pixelSize: 8; font.family: "Monospace" }
            }
            RowLayout {
                spacing: 4
                Text { text: "End:"; color: Theme.textMuted; font.pixelSize: 8 }
                Text { text: "005|01|000"; color: Theme.clockGreen; font.pixelSize: 8; font.family: "Monospace" }
            }
            RowLayout {
                spacing: 4
                Text { text: "Length:"; color: Theme.textMuted; font.pixelSize: 8 }
                Text { text: "004|00|000"; color: Theme.clockBlue; font.pixelSize: 8; font.family: "Monospace" }
            }
        }

        Item { Layout.fillWidth: true }

        // 8. Main Mode Switcher Tabs (Editor, Mixer, Rec, Cues, Setup)
        RowLayout {
            spacing: 2
            Repeater {
                model: ["Editor", "Mixer", "Rec", "Cues", "Setup"]
                Rectangle {
                    width: 44; height: 26; radius: 2
                    color: globalSession?.workspaceMode === index ? Theme.btnActive : Theme.btnNormal
                    border.color: globalSession?.workspaceMode === index ? Theme.btnActive : Theme.borderDark
                    Text { anchors.centerIn: parent; text: modelData; color: globalSession?.workspaceMode === index ? Theme.textMain : Theme.textMuted; font.pixelSize: 10; font.bold: globalSession?.workspaceMode === index }
                    MouseArea { anchors.fill: parent; onClicked: globalSession?.set_workspace_mode(index) }
                }
            }
        }
    }
}
