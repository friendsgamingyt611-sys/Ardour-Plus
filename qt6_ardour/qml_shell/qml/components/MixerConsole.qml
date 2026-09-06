import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import ".." // Theme

Rectangle {
    id: mixerConsoleRoot
    color: Theme.bgWindow

    Flickable {
        anchors.fill: parent
        anchors.margins: 4
        contentWidth: mixerRow.width + 120
        contentHeight: height
        clip: true
        boundsBehavior: Flickable.StopAtBounds

        Row {
            id: mixerRow
            height: parent.height
            spacing: 4

            // Channel Strips bound to real C++ routes
            Repeater {
                model: globalSession ? globalSession.routes : []

                MixerStrip {
                    height: mixerRow.height
                    routeData: model.modelData
                }
            }

            // Master Bus Placeholder
            Rectangle {
                width: 105
                height: mixerRow.height
                color: Theme.bgCard
                border.color: Theme.borderDark
                border.width: 1
                radius: 3

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 3
                    spacing: 2
                    
                    Rectangle {
                        Layout.fillWidth: true
                        height: 22
                        radius: 2
                        color: "#aa3333"
                        Text {
                            text: "Master"
                            color: "#ffffff"; font.pixelSize: 11; font.bold: true
                            anchors.centerIn: parent
                        }
                    }
                }
            }
        }
    }
}
