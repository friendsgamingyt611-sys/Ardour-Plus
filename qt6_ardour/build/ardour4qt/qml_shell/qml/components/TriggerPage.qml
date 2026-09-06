import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import ".."

Rectangle {
    id: triggerPageRoot
    color: Theme.bgWindow

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 6

        // Header Control Panel
        Rectangle {
            Layout.fillWidth: true; height: 36; color: Theme.bgCard; radius: 4
            RowLayout {
                anchors.fill: parent; anchors.leftMargin: 10; anchors.rightMargin: 10
                Text { text: "Cue / Clip Matrix Launcher (Trigger Page)"; color: Theme.textMain; font.pixelSize: 12; font.bold: true }
                Item { Layout.fillWidth: true }
                Text { text: "Quantize: 1 Bar"; color: Theme.clockBlue; font.pixelSize: 10; font.bold: true }
            }
        }

        // Clip Launcher Grid
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: Theme.bgPanel
            border.color: Theme.borderDark
            radius: 4

            Flickable {
                anchors.fill: parent
                anchors.margins: 8
                contentWidth: triggerGrid.width + 20
                contentHeight: triggerGrid.height + 20
                clip: true

                Row {
                    id: triggerGrid
                    spacing: 6

                    Repeater {
                        model: globalTrackModel

                        Column {
                            spacing: 4
                            // Track Title Bar
                            Rectangle {
                                width: 110; height: 24; color: model.color_hex || Theme.btnActive; radius: 3
                                Text { anchors.centerIn: parent; text: model.name; color: Theme.textMain; font.pixelSize: 9; font.bold: true; elide: Text.ElideRight }
                            }

                            // 8 Clip Slots per Track
                            Repeater {
                                model: 6
                                Rectangle {
                                    width: 110; height: 38; color: Theme.bgCard; border.color: Theme.borderDark; radius: 3
                                    RowLayout {
                                        anchors.fill: parent; anchors.margins: 4
                                        Rectangle {
                                            width: 20; height: 20; radius: 10
                                            color: globalTransport.isPlaying ? Theme.clockGreen : Theme.btnActive
                                            Text { anchors.centerIn: parent; text: "▶"; color: Theme.textMain; font.pixelSize: 8 }
                                        }
                                        Text { text: "Clip_" + (index + 1); color: Theme.textMain; font.pixelSize: 9; Layout.fillWidth: true }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
