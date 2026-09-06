import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Dialog {
    id: addTrackDialogRoot
    title: "Add Track / Bus"
    modal: true
    focus: true
    anchors.centerIn: parent
    width: 480
    height: 360

    background: Rectangle {
        color: "#181a20"
        border.color: "#303443"
        border.width: 1
        radius: 6
    }

    header: Rectangle {
        implicitHeight: 40
        color: "#1f222b"
        radius: 6

        Text {
            anchors.left: parent.left
            anchors.leftMargin: 16
            anchors.verticalCenter: parent.verticalCenter
            text: "Add Track, Bus or VCA"
            color: "#ffffff"
            font.pixelSize: 13
            font.bold: true
        }

        Rectangle {
            anchors.bottom: parent.bottom
            width: parent.width
            height: 1
            color: "#2e3240"
        }
    }

    contentItem: ColumnLayout {
        spacing: 16

        RowLayout {
            spacing: 16

            // Track Type Selector
            ColumnLayout {
                spacing: 6
                Text { text: "Track/Bus Type:"; color: "#9094a6"; font.pixelSize: 11 }
                ComboBox {
                    id: typeCombo
                    implicitWidth: 180
                    model: ["Audio Track", "MIDI Track", "Audio Bus", "VCA Control"]
                    currentIndex: 0
                }
            }

            // Configuration Preset Selector
            ColumnLayout {
                spacing: 6
                Text { text: "Channel Configuration:"; color: "#9094a6"; font.pixelSize: 11 }
                ComboBox {
                    id: channelCombo
                    implicitWidth: 180
                    model: ["Mono (1 Channel)", "Stereo (2 Channels)", "5.1 Surround", "8-Channel Bus"]
                    currentIndex: 1
                }
            }
        }

        // Track Name Input
        ColumnLayout {
            spacing: 6
            Text { text: "Track Name:"; color: "#9094a6"; font.pixelSize: 11 }
            TextField {
                id: nameField
                Layout.fillWidth: true
                placeholderText: "Audio 1"
                text: typeCombo.currentText.split(" ")[0] + " " + (globalTrackModel.rowCount() + 1)
                color: "#ffffff"
                background: Rectangle {
                    color: "#121317"
                    border.color: "#2e3240"
                    radius: 3
                }
            }
        }

        // Track Count & Color selection
        RowLayout {
            spacing: 16

            ColumnLayout {
                spacing: 6
                Text { text: "Count / Quantity:"; color: "#9094a6"; font.pixelSize: 11 }
                SpinBox {
                    id: countSpin
                    from: 1; to: 16; value: 1
                }
            }

            ColumnLayout {
                spacing: 6
                Text { text: "Track Color:"; color: "#9094a6"; font.pixelSize: 11 }
                Row {
                    spacing: 6
                    Repeater {
                        model: ["#3b82f6", "#10b981", "#f59e0b", "#ef4444", "#8b5cf6", "#ec4899"]
                        Rectangle {
                            width: 22; height: 22; radius: 11
                            color: modelData
                            border.color: selectedColor === modelData ? "#ffffff" : "transparent"
                            border.width: 2
                            property string selectedColor: "#3b82f6"
                            MouseArea {
                                anchors.fill: parent
                                onClicked: parent.selectedColor = modelData
                            }
                        }
                    }
                }
            }
        }
    }

    footer: DialogButtonBox {
        background: Rectangle {
            color: "#1f222b"
            radius: 6
        }

        Button {
            text: "Add Track"
            DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
            onClicked: {
                var tName = nameField.text !== "" ? nameField.text : "Audio 1";
                var tType = "audio";
                if (typeCombo.currentIndex === 1) tType = "midi";
                else if (typeCombo.currentIndex === 2) tType = "bus";
                else if (typeCombo.currentIndex === 3) tType = "vca";

                for (var i = 0; i < countSpin.value; i++) {
                    var finalName = countSpin.value > 1 ? tName + " " + (i + 1) : tName;
                    globalTrackModel.add_track(finalName, tType);
                }
                addTrackDialogRoot.close();
            }
        }

        Button {
            text: "Cancel"
            DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
            onClicked: addTrackDialogRoot.close()
        }
    }
}
