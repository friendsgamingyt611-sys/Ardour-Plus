import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import ".."

Rectangle {
    id: rightSidebarRoot
    implicitWidth: 260
    color: Theme.bgPanel

    property bool isCollapsed: false
    property int activeTab: 6 /* Default 'M' (Markers) tab */

    Rectangle {
        anchors.left: parent.left
        width: 1
        height: parent.height
        color: Theme.borderDark
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        // Main Sidebar Content Area
        StackLayout {
            id: sidebarStack
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: rightSidebarRoot.activeTab
            visible: !rightSidebarRoot.isCollapsed

            // Tab 0: Tracks ('T')
            Rectangle {
                color: Theme.bgPanel
                ColumnLayout {
                    anchors.fill: parent; anchors.margins: 6
                    Text { text: "TRACKS SUMMARY"; color: Theme.textMuted; font.pixelSize: 10; font.bold: true }
                    Text { text: globalTrackModel.rowCount() + " Active Tracks"; color: Theme.textMain; font.pixelSize: 10 }
                    Item { Layout.fillHeight: true }
                }
            }

            // Tab 1: Sources ('S')
            Rectangle {
                color: Theme.bgPanel
                ColumnLayout {
                    anchors.fill: parent; anchors.margins: 6
                    Text { text: "AUDIO SOURCES"; color: Theme.textMuted; font.pixelSize: 10; font.bold: true }
                    Text { text: "0 Source Files Loaded"; color: Theme.textMuted; font.pixelSize: 10 }
                    Item { Layout.fillHeight: true }
                }
            }

            // Tab 2: Regions ('R')
            Rectangle {
                color: Theme.bgPanel
                ColumnLayout {
                    anchors.fill: parent; anchors.margins: 6
                    Text { text: "SESSION REGIONS"; color: Theme.textMuted; font.pixelSize: 10; font.bold: true }
                    Text { text: "0 Regions in Playlist"; color: Theme.textMuted; font.pixelSize: 10 }
                    Item { Layout.fillHeight: true }
                }
            }

            // Tab 3: Collections ('C')
            Rectangle {
                color: Theme.bgPanel
                ColumnLayout {
                    anchors.fill: parent; anchors.margins: 6
                    Text { text: "COLLECTIONS & SAMPLES"; color: Theme.textMuted; font.pixelSize: 10; font.bold: true }
                    Text { text: "No Sample Libraries Attached"; color: Theme.textMuted; font.pixelSize: 10 }
                    Item { Layout.fillHeight: true }
                }
            }

            // Tab 4: Plugins ('P')
            Rectangle {
                color: Theme.bgPanel
                ColumnLayout {
                    anchors.fill: parent; anchors.margins: 6
                    Text { text: "PLUGIN RACK"; color: Theme.textMuted; font.pixelSize: 10; font.bold: true }
                    Text { text: "ACE Compressor, ACE EQ"; color: Theme.textMain; font.pixelSize: 10 }
                    Item { Layout.fillHeight: true }
                }
            }

            // Tab 5: Groups ('G')
            Rectangle {
                color: Theme.bgPanel
                ColumnLayout {
                    anchors.fill: parent; anchors.margins: 6
                    Text { text: "ROUTE GROUPS"; color: Theme.textMuted; font.pixelSize: 10; font.bold: true }
                    Text { text: "All Routes Ungrouped"; color: Theme.textMuted; font.pixelSize: 10 }
                    Item { Layout.fillHeight: true }
                }
            }

            // Tab 6: Markers ('M')
            ColumnLayout {
                spacing: 0

                // 1. Loop / Punch Ranges Box
                Rectangle {
                    Layout.fillWidth: true
                    implicitHeight: 110
                    color: Theme.bgPanel

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 4
                        spacing: 3

                        Rectangle {
                            Layout.fillWidth: true
                            height: 18
                            color: Theme.bgCard
                            Text {
                                anchors.left: parent.left; anchors.leftMargin: 6; anchors.verticalCenter: parent.verticalCenter
                                text: "Loop / Punch Ranges"
                                color: Theme.textMain; font.pixelSize: 9; font.bold: true
                            }
                        }

                        // Range Row 1 (Loop)
                        RowLayout {
                            spacing: 3
                            Button {
                                implicitWidth: 34; implicitHeight: 18
                                text: "Goto"; font.pixelSize: 8
                                onClicked: globalTransport.rewind()
                            }
                            Rectangle {
                                width: 65; height: 18; color: Theme.bgHeader
                                Text { anchors.centerIn: parent; text: "001|01|0000"; color: Theme.clockGreen; font.pixelSize: 8; font.family: "Monospace" }
                            }
                            Button { implicitWidth: 44; implicitHeight: 18; text: "Use PH"; font.pixelSize: 8 }
                            Rectangle {
                                Layout.fillWidth: true; height: 18; color: Theme.btnActive; radius: 2
                                Text { anchors.centerIn: parent; text: "Loop"; color: Theme.textMain; font.pixelSize: 8; font.bold: true }
                            }
                        }

                        // Range Row 2 (Punch)
                        RowLayout {
                            spacing: 3
                            Button {
                                implicitWidth: 34; implicitHeight: 18
                                text: "Goto"; font.pixelSize: 8
                                onClicked: globalTransport.fast_forward()
                            }
                            Rectangle {
                                width: 65; height: 18; color: Theme.bgHeader
                                Text { anchors.centerIn: parent; text: "007|01|0000"; color: Theme.clockGreen; font.pixelSize: 8; font.family: "Monospace" }
                            }
                            Button { implicitWidth: 44; implicitHeight: 18; text: "Use PH"; font.pixelSize: 8 }
                            Rectangle {
                                Layout.fillWidth: true; height: 18; color: Theme.recRed; radius: 2
                                Text { anchors.centerIn: parent; text: "Punch"; color: Theme.textMain; font.pixelSize: 8; font.bold: true }
                            }
                        }
                    }
                }

                Rectangle { Layout.fillWidth: true; height: 1; color: Theme.borderDark }

                // 2. Markers List Box
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: Theme.bgPanel

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 4
                        spacing: 3

                        Rectangle {
                            Layout.fillWidth: true
                            height: 18
                            color: Theme.bgCard
                            Text {
                                anchors.left: parent.left; anchors.leftMargin: 6; anchors.verticalCenter: parent.verticalCenter
                                text: "Markers (including CD Index)"
                                color: Theme.textMain; font.pixelSize: 9; font.bold: true
                            }
                        }

                        // Markers ListView (Dynamically bound to globalTrackModel markers)
                        ListView {
                            id: markerList
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            clip: true
                            spacing: 2
                            model: globalTrackModel.markers()

                            delegate: Rectangle {
                                width: markerList.width
                                height: 20
                                color: Theme.bgCard
                                border.color: Theme.borderDark

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.margins: 2
                                    spacing: 3

                                    Button {
                                        implicitWidth: 32; implicitHeight: 16
                                        text: "Goto"; font.pixelSize: 8
                                        onClicked: globalTrackModel.jumpToPosition(modelData.positionX)
                                    }
                                    Rectangle {
                                        width: 60; height: 16; color: Theme.bgHeader
                                        Text { anchors.centerIn: parent; text: modelData.bbtStr; color: Theme.clockGreen; font.pixelSize: 8; font.family: "Monospace" }
                                    }
                                    Button {
                                        implicitWidth: 40; implicitHeight: 16
                                        text: "Use PH"; font.pixelSize: 8
                                    }
                                    Text {
                                        text: modelData.name
                                        color: Theme.textMain
                                        font.pixelSize: 9
                                        Layout.fillWidth: true
                                        elide: Text.ElideRight
                                    }
                                }
                            }
                        }

                        // Add Marker Button
                        Rectangle {
                            Layout.fillWidth: true
                            height: 22
                            radius: 2
                            color: Theme.btnNormal
                            border.color: Theme.borderDark

                            Text {
                                anchors.centerIn: parent
                                text: "+ New Marker"
                                color: Theme.textMain
                                font.pixelSize: 9
                                font.bold: true
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: globalTrackModel.addMarker("")
                            }
                        }
                    }
                }
            }
        }

        // Vertical Tab Strip (Far Right Edge matching image2 exact letters: T, S, R, C, P, G, M)
        Rectangle {
            width: 22
            Layout.fillHeight: true
            color: Theme.bgHeader

            Rectangle {
                anchors.left: parent.left
                width: 1
                height: parent.height
                color: Theme.borderDark
            }

            Column {
                anchors.fill: parent
                spacing: 6
                padding: 3

                Repeater {
                    model: ["T", "S", "R", "C", "P", "G", "M"]

                    Rectangle {
                        width: 16
                        height: 22
                        radius: 2
                        color: rightSidebarRoot.activeTab === index ? Theme.btnActive : Theme.btnNormal

                        Text {
                            anchors.centerIn: parent
                            text: modelData
                            color: rightSidebarRoot.activeTab === index ? Theme.textMain : Theme.textMuted
                            font.pixelSize: 10
                            font.bold: true
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (rightSidebarRoot.activeTab === index) {
                                    rightSidebarRoot.isCollapsed = !rightSidebarRoot.isCollapsed;
                                } else {
                                    rightSidebarRoot.activeTab = index;
                                    rightSidebarRoot.isCollapsed = false;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

