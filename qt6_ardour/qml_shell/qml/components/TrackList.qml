import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import ".."

Rectangle {
    id: trackListRoot
    implicitWidth: 240
    color: Theme.bgWindow

    signal requestAddTrack()

    Rectangle {
        anchors.right: parent.right
        width: 1
        height: parent.height
        color: Theme.borderDark
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Header Title Bar
        Rectangle {
            Layout.fillWidth: true
            implicitHeight: 24
            color: Theme.bgHeader

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 8
                anchors.rightMargin: 8

                Text {
                    text: "TRACKS & BUSES"
                    color: Theme.textMuted
                    font.pixelSize: 10
                    font.bold: true
                }

                Item { Layout.fillWidth: true }

                Text {
                    text: globalTrackModel.rowCount() + " Tracks"
                    color: Theme.textMuted
                    font.pixelSize: 9
                }
            }

            Rectangle {
                anchors.bottom: parent.bottom
                width: parent.width
                height: 1
                color: Theme.borderDark
            }
        }

        // Track ListView
        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: globalTrackModel
            spacing: 2

            delegate: Rectangle {
                width: listView.width
                height: model.track_height || 56
                color: model.is_selected ? Theme.bgCard : Theme.bgPanel
                border.color: model.is_selected ? Theme.btnActive : Theme.borderDark

                MouseArea {
                    anchors.fill: parent
                    onClicked: globalTrackModel.select_track(model.index)
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 2
                    spacing: 4

                    // Track Color Strip
                    Rectangle {
                        width: 5
                        Layout.fillHeight: true
                        radius: 2
                        color: model.color_hex || Theme.btnActive
                    }

                    // Main Controls Stack Column
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 2

                        // Row 1: Track Name & Type Badge
                        RowLayout {
                            spacing: 4

                            Text {
                                text: model.name
                                color: Theme.textMain
                                font.pixelSize: 11
                                font.bold: true
                                Layout.fillWidth: true
                                elide: Text.ElideRight
                            }

                            Rectangle {
                                width: 34; height: 14; radius: 2
                                color: model.type === "midi" ? "#6b21a8" : (model.type === "bus" ? "#1e3a8a" : (model.type === "vca" ? "#92400e" : "#065f46"))
                                Text {
                                    anchors.centerIn: parent
                                    text: model.type ? model.type.toUpperCase() : "AUDIO"
                                    color: "#ffffff"
                                    font.pixelSize: 7
                                    font.bold: true
                                }
                            }

                            // Delete Track Button
                            Rectangle {
                                width: 14; height: 14; radius: 7
                                color: Theme.bgHeader
                                Text { anchors.centerIn: parent; text: "×"; color: Theme.textMuted; font.pixelSize: 10 }
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: globalTrackModel.remove_track(model.index)
                                }
                            }
                        }

                        // Row 2: Mute (M), Solo (S), Rec Arm (R) + Volume Slider
                        RowLayout {
                            spacing: 2

                            // Mute (M)
                            Rectangle {
                                width: 18; height: 18; radius: 2
                                color: model.is_muted ? Theme.muteRed : Theme.btnNormal
                                border.color: model.is_muted ? Theme.muteRed : Theme.borderDark
                                Text { anchors.centerIn: parent; text: "M"; color: model.is_muted ? Theme.textMain : Theme.textMuted; font.pixelSize: 9; font.bold: true }
                                MouseArea { anchors.fill: parent; onClicked: globalTrackModel.toggle_mute(model.index) }
                            }

                            // Solo (S)
                            Rectangle {
                                width: 18; height: 18; radius: 2
                                color: model.is_soloed ? Theme.soloOrange : Theme.btnNormal
                                border.color: model.is_soloed ? Theme.soloOrange : Theme.borderDark
                                Text { anchors.centerIn: parent; text: "S"; color: model.is_soloed ? Theme.textMain : Theme.textMuted; font.pixelSize: 9; font.bold: true }
                                MouseArea { anchors.fill: parent; onClicked: globalTrackModel.toggle_solo(model.index) }
                            }

                            // Rec Arm (R)
                            Rectangle {
                                width: 18; height: 18; radius: 2
                                color: model.is_rec_armed ? Theme.recRed : Theme.btnNormal
                                border.color: model.is_rec_armed ? Theme.recRed : Theme.borderDark
                                Text { anchors.centerIn: parent; text: "R"; color: model.is_rec_armed ? Theme.textMain : Theme.textMuted; font.pixelSize: 9; font.bold: true }
                                MouseArea { anchors.fill: parent; onClicked: globalTrackModel.toggle_rec_arm(model.index) }
                            }

                            // Volume Slider
                            Slider {
                                Layout.fillWidth: true
                                implicitHeight: 16
                                from: -60.0; to: 6.0; value: model.volume_db || 0.0
                                onValueChanged: globalTrackModel.set_volume(model.index, value)
                            }
                        }

                        // Row 3: Pan Slider & Automation Mode
                        RowLayout {
                            spacing: 4

                            Rectangle {
                                width: 50; height: 14; color: Theme.bgHeader; radius: 2
                                Text { anchors.centerIn: parent; text: "L66R34"; color: Theme.clockBlue; font.pixelSize: 8; font.family: "Monospace" }
                            }

                            ComboBox {
                                implicitWidth: 65
                                implicitHeight: 16
                                model: ["Manual", "Play", "Write"]
                                currentIndex: 0
                                background: Rectangle { color: Theme.btnNormal; border.color: Theme.borderDark; radius: 2 }
                                contentItem: Text { text: parent.displayText; color: Theme.textMuted; font.pixelSize: 8; verticalAlignment: Text.AlignVCenter; horizontalAlignment: Text.AlignHCenter }
                            }
                        }
                    }
                }
            }
        }

        // Add Track Footer Button
        Rectangle {
            Layout.fillWidth: true
            implicitHeight: 30
            color: Theme.bgHeader

            Rectangle {
                anchors.top: parent.top
                width: parent.width
                height: 1
                color: Theme.borderDark
            }

            Rectangle {
                anchors.centerIn: parent
                width: parent.width - 12
                height: 22
                radius: 2
                color: Theme.btnActive

                RowLayout {
                    anchors.centerIn: parent
                    spacing: 4
                    Text { text: "+"; color: Theme.textMain; font.pixelSize: 13; font.bold: true }
                    Text { text: "Add Track / Bus"; color: Theme.textMain; font.pixelSize: 10; font.bold: true }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: trackListRoot.requestAddTrack()
                }
            }
        }
    }
}
