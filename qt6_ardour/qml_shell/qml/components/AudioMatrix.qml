import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import ".."

Rectangle {
    id: audioMatrixRoot
    color: Theme.bgWindow

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 6
        spacing: 4

        // Header Title
        RowLayout {
            Layout.fillWidth: true
            Text {
                text: "Audio & MIDI Routing Patchbay Matrix (ALSA / JACK Connections)"
                color: Theme.textMain; font.pixelSize: 11; font.bold: true
            }
            Item { Layout.fillWidth: true }
            Text {
                text: "Click intersection point to toggle route connection"
                color: Theme.textMuted; font.pixelSize: 9
            }
        }

        // Patchbay Matrix Table Container
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: Theme.bgPanel
            border.color: Theme.borderDark
            radius: 3

            Flickable {
                anchors.fill: parent
                anchors.margins: 4
                contentWidth: matrixGrid.width + 120
                contentHeight: matrixGrid.height + 40
                clip: true

                Column {
                    id: matrixGrid
                    spacing: 2

                    // Top Destination Headers Row
                    Row {
                        spacing: 2
                        Rectangle { width: 140; height: 24; color: Theme.bgHeader }

                        Repeater {
                            model: ["Master In L", "Master In R", "Track 1 In", "Track 2 In", "System Out 1", "System Out 2"]
                            Rectangle {
                                width: 85; height: 24; color: Theme.bgHeader; border.color: Theme.borderDark
                                Text { anchors.centerIn: parent; text: modelData; color: Theme.clockBlue; font.pixelSize: 9; font.bold: true; elide: Text.ElideRight }
                            }
                        }
                    }

                    // Matrix Source Rows Repeater
                    Repeater {
                        model: [
                            "system:capture_1",
                            "system:capture_2",
                            "Track 1 Out L",
                            "Track 1 Out R",
                            "Track 2 Out L",
                            "Track 2 Out R",
                            "Master Out L",
                            "Master Out R"
                        ]

                        Row {
                            spacing: 2
                            property int sourceIdx: index

                            // Left Source Header Label
                            Rectangle {
                                width: 140; height: 20; color: Theme.bgCard; border.color: Theme.borderDark
                                Text {
                                    anchors.left: parent.left; anchors.leftMargin: 6; anchors.verticalCenter: parent.verticalCenter
                                    text: modelData; color: Theme.textMain; font.pixelSize: 9; font.bold: true; elide: Text.ElideRight
                                }
                            }

                            // Connection Grid Cells
                            Repeater {
                                model: 6
                                Rectangle {
                                    width: 85; height: 20
                                    color: (sourceIdx === index) ? Theme.btnActive : Theme.bgPanel
                                    border.color: Theme.borderDark

                                    // Active Connection Dot
                                    Rectangle {
                                        visible: (sourceIdx === index) || (sourceIdx === 6 && index === 4) || (sourceIdx === 7 && index === 5)
                                        width: 8; height: 8; radius: 4
                                        anchors.centerIn: parent
                                        color: Theme.clockGreen
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            // Toggle route connection point
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
}
