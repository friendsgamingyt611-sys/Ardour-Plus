import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import ".." // Theme

Rectangle {
    id: root
    width: 90
    height: parent ? parent.height : 600
    color: "#2B2B2B" // GTK2 Dark bg
    border.color: "#1A1A1A"
    border.width: 1

    property var routeData: null // Maps to QtRoute QObject

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 4
        spacing: 4

        // Top Section: Routing (Input)
        Button {
            Layout.fillWidth: true
            Layout.preferredHeight: 24
            text: "Input"
            background: Rectangle {
                color: "#3D3D3D"
                border.color: "#1A1A1A"
                radius: 2
            }
            contentItem: Text {
                text: parent.text
                color: "#CCCCCC"
                font.pixelSize: 10
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        // Processor Box (Empty for now)
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 120
            color: "#1E1E1E"
            border.color: "#111111"
            
            Text {
                text: "Plugins..."
                color: "#555555"
                font.pixelSize: 10
                anchors.centerIn: parent
            }
        }

        // Panner (Placeholder)
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 30
            color: "transparent"
            border.color: "#333333"
            
            Rectangle {
                width: 4
                height: 15
                color: "#00FF00"
                anchors.centerIn: parent
            }
        }

        // Mute & Solo
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 24
            spacing: 2
            
            Button {
                id: muteBtn
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: "M"
                checkable: true
                checked: root.routeData ? root.routeData.isMuted : false
                onClicked: {
                    if (root.routeData) root.routeData.isMuted = checked
                }
                background: Rectangle {
                    color: parent.checked ? "#FF3333" : "#333333"
                    radius: 2
                    border.color: "#111"
                }
                contentItem: Text {
                    text: parent.text
                    color: parent.checked ? "#FFFFFF" : "#AAAAAA"
                    font.pixelSize: 11
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
            Button {
                id: soloBtn
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: "S"
                checkable: true
                checked: root.routeData ? root.routeData.isSoloed : false
                onClicked: {
                    if (root.routeData) root.routeData.isSoloed = checked
                }
                background: Rectangle {
                    color: parent.checked ? "#FFFF00" : "#333333"
                    radius: 2
                    border.color: "#111"
                }
                contentItem: Text {
                    text: parent.text
                    color: parent.checked ? "#000000" : "#AAAAAA"
                    font.pixelSize: 11
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        // Fader & Meter Area
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            
            RowLayout {
                anchors.fill: parent
                spacing: 4
                
                // Peak Meter
                Rectangle {
                    Layout.preferredWidth: 10
                    Layout.fillHeight: true
                    color: "#111111"
                    border.color: "#000000"
                    
                    Rectangle {
                        id: meterFill
                        width: parent.width - 2
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 1
                        anchors.horizontalCenter: parent.horizontalCenter
                        
                        // Map 0..1 to height
                        property real level: root.routeData ? root.routeData.peakMeterLevel : 0.0
                        height: Math.max(0, Math.min(1, level)) * (parent.height - 2)
                        
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: "#FF0000" }
                            GradientStop { position: 0.2; color: "#FFFF00" }
                            GradientStop { position: 1.0; color: "#00FF00" }
                        }
                    }
                }
                
                // Fader Slot
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: "#222222"
                    border.color: "#111111"
                    
                    // Fader Handle (Simple visualization for now)
                    Rectangle {
                        id: faderCap
                        width: parent.width
                        height: 20
                        color: "#555555"
                        border.color: "#777777"
                        
                        property real gain: root.routeData ? root.routeData.gain : 1.0
                        // Naive mapping: 1.0 gain is roughly 75% up
                        y: parent.height - 20 - (parent.height - 20) * Math.min(1.0, gain * 0.75)
                        
                        Rectangle {
                            width: parent.width - 6
                            height: 2
                            color: "#FFFFFF"
                            anchors.centerIn: parent
                        }
                        
                        MouseArea {
                            anchors.fill: parent
                            drag.target: parent
                            drag.axis: Drag.YAxis
                            drag.minimumY: 0
                            drag.maximumY: parent.parent.height - parent.height
                            onPositionChanged: {
                                if (drag.active && root.routeData) {
                                    // Inverse mapping
                                    let norm = 1.0 - (parent.y / drag.maximumY)
                                    root.routeData.gain = norm / 0.75
                                }
                            }
                        }
                    }
                }
            }
        }
        
        // Output / Record Arm
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 20
            
            Button {
                Layout.preferredWidth: 20
                Layout.fillHeight: true
                background: Rectangle {
                    color: "#333333"
                    radius: 10
                    border.color: "#111"
                }
            }
            Button {
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: "Out"
                background: Rectangle {
                    color: "#3D3D3D"
                    radius: 2
                    border.color: "#111"
                }
                contentItem: Text {
                    text: parent.text
                    color: "#CCCCCC"
                    font.pixelSize: 10
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        // Track Name
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 24
            color: "#111111"
            
            Text {
                text: root.routeData ? root.routeData.name : "Track"
                color: "#FFFFFF"
                font.pixelSize: 11
                anchors.centerIn: parent
                elide: Text.ElideRight
                width: parent.width - 4
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }
}
