import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import ".."

Rectangle {
    id: recorderViewRoot
    color: Theme.bgWindow

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 6

        // 1. Header Toolbar (Global Arm All, Arm None, Peak Hold Reset, Disk Status)
        Rectangle {
            Layout.fillWidth: true
            height: 44
            color: Theme.bgCard
            border.color: Theme.borderDark
            radius: 4

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 10; anchors.rightMargin: 10
                spacing: 8

                // Arm All Button
                Rectangle {
                    width: 110; height: 26; radius: 3
                    color: Theme.recRed; border.color: Theme.recRed
                    RowLayout {
                        anchors.centerIn: parent; spacing: 4
                        Text { text: "●"; color: Theme.textMain; font.pixelSize: 12 }
                        Text { text: "Arm All Tracks"; color: Theme.textMain; font.pixelSize: 10; font.bold: true }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            for (var i = 0; i < globalTrackModel.rowCount(); i++) {
                                if (!globalTrackModel.data(globalTrackModel.index(i, 0), 263)) {
                                    globalTrackModel.toggle_rec_arm(i);
                                }
                            }
                        }
                    }
                }

                // Arm None Button
                Rectangle {
                    width: 100; height: 26; radius: 3
                    color: Theme.btnNormal; border.color: Theme.borderDark
                    Text { anchors.centerIn: parent; text: "Arm None"; color: Theme.textMain; font.pixelSize: 10; font.bold: true }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            for (var i = 0; i < globalTrackModel.rowCount(); i++) {
                                if (globalTrackModel.data(globalTrackModel.index(i, 0), 263)) {
                                    globalTrackModel.toggle_rec_arm(i);
                                }
                            }
                        }
                    }
                }

                // Reset Peak Hold
                Rectangle {
                    width: 130; height: 26; radius: 3
                    color: Theme.btnNormal; border.color: Theme.borderDark
                    Text { anchors.centerIn: parent; text: "Reset Peak Hold"; color: Theme.clockBlue; font.pixelSize: 10; font.bold: true }
                }

                Item { Layout.fillWidth: true }

                // Disk Recording Status Gauge Box
                Rectangle {
                    width: 240; height: 32; radius: 3
                    color: Theme.bgHeader; border.color: Theme.borderDark

                    RowLayout {
                        anchors.fill: parent; anchors.margins: 6; spacing: 6
                        Text { text: "Free Space:"; color: Theme.textMuted; font.pixelSize: 9 }
                        Text { text: globalSession.diskSpaceFree; color: Theme.clockGreen; font.pixelSize: 10; font.bold: true; font.family: "Monospace" }
                    }
                }
            }
        }

        // 2. Armed Tracks Multi-Channel Meter Grid View
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: Theme.bgPanel
            border.color: Theme.borderDark
            radius: 4

            Flickable {
                anchors.fill: parent
                anchors.margins: 8
                contentWidth: recorderGrid.width + 20
                contentHeight: height
                clip: true

                Row {
                    id: recorderGrid
                    height: parent.height
                    spacing: 8

                    Repeater {
                        model: globalTrackModel

                        Rectangle {
                            width: 140
                            height: recorderGrid.height
                            color: model.is_rec_armed ? Theme.bgCard : Theme.bgPanel
                            border.color: model.is_rec_armed ? Theme.recRed : Theme.borderDark
                            border.width: model.is_rec_armed ? 2 : 1
                            radius: 4

                            ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 4
                                spacing: 4

                                // Track Header & Arm Switch
                                Rectangle {
                                    Layout.fillWidth: true; height: 26; radius: 3
                                    color: model.is_rec_armed ? Theme.recRed : Theme.btnNormal
                                    RowLayout {
                                        anchors.fill: parent; anchors.margins: 4
                                        Text { text: "#" + (model.track_number || (index + 1)); color: Theme.textMain; font.pixelSize: 10; font.bold: true }
                                        Text { text: model.name; color: Theme.textMain; font.pixelSize: 10; font.bold: true; Layout.fillWidth: true; elide: Text.ElideRight }
                                        Text { text: "REC"; color: Theme.textMain; font.pixelSize: 9; font.bold: true; visible: model.is_rec_armed }
                                    }
                                    MouseArea { anchors.fill: parent; onClicked: globalTrackModel.toggle_rec_arm(index) }
                                }

                                // Monitoring Selector (In / Disk)
                                RowLayout {
                                    Layout.fillWidth: true; spacing: 2
                                    Rectangle {
                                        Layout.fillWidth: true; height: 16; radius: 2; color: Theme.btnNormal
                                        Text { anchors.centerIn: parent; text: "In: " + (model.input_name || "capture_1"); color: Theme.textMuted; font.pixelSize: 8; elide: Text.ElideRight }
                                    }
                                }

                                // Large Armed Level Meter Bar Stack
                                RowLayout {
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    spacing: 4

                                    Rectangle {
                                        Layout.fillWidth: true; Layout.fillHeight: true
                                        color: Theme.bgHeader; radius: 3; border.color: Theme.borderDark

                                        Row {
                                            anchors.fill: parent; anchors.margins: 2; spacing: 2
                                            // Left Channel Large Meter
                                            Rectangle {
                                                width: parent.width / 2 - 1; height: parent.height; color: Theme.bgHeader
                                                Rectangle {
                                                    anchors.bottom: parent.bottom; width: parent.width
                                                    height: parent.height * (globalTransport.isPlaying || model.is_rec_armed ? Math.min(1.0, Math.max(0.06, ((model.volume_db || 0) + 60.0) / 66.0)) : 0.04)
                                                    color: height > parent.height * 0.88 ? Theme.muteRed : (height > parent.height * 0.7 ? Theme.soloOrange : Theme.clockGreen)
                                                }
                                            }
                                            // Right Channel Large Meter
                                            Rectangle {
                                                width: parent.width / 2 - 1; height: parent.height; color: Theme.bgHeader
                                                Rectangle {
                                                    anchors.bottom: parent.bottom; width: parent.width
                                                    height: parent.height * (globalTransport.isPlaying || model.is_rec_armed ? Math.min(1.0, Math.max(0.05, ((model.volume_db || 0) + 58.0) / 66.0)) : 0.04)
                                                    color: height > parent.height * 0.88 ? Theme.muteRed : (height > parent.height * 0.7 ? Theme.soloOrange : Theme.clockGreen)
                                                }
                                            }
                                        }
                                    }
                                }

                                // Peak Readout
                                Rectangle {
                                    Layout.fillWidth: true; height: 18; color: Theme.bgHeader; radius: 2
                                    Text { anchors.centerIn: parent; text: "-5.8 dB pk"; color: Theme.clockGreen; font.pixelSize: 9; font.bold: true; font.family: "Monospace" }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
