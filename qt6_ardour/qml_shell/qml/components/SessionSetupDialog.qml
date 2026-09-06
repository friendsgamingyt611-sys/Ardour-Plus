import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Rectangle {
    id: sessionSetupRoot
    width: 680
    height: 460
    color: "#181a20"
    border.color: "#2a2f3a"
    border.width: 1
    radius: 6

    signal launchWorkspaceRequested(string sessionName, string sessionPath)

    property int activeTab: 0 // 0: Recent, 1: New Session, 2: Audio Engine

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        // Header Title Bar
        Rectangle {
            Layout.fillWidth: true
            height: 50
            color: "#23272e"
            radius: 4

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 12; anchors.rightMargin: 12
                spacing: 12

                Rectangle {
                    width: 32; height: 32; radius: 4; color: "#1976d2"
                    Text { anchors.centerIn: parent; text: "A+"; color: "#ffffff"; font.bold: true; font.pixelSize: 14 }
                }

                ColumnLayout {
                    spacing: 2
                    Text { text: "Ardour Plus — Session & Engine Setup"; color: "#e2e8f0"; font.pixelSize: 14; font.bold: true }
                    Text { text: "Select an existing session, create a new project, or configure audio I/O interfaces"; color: "#8f99ad"; font.pixelSize: 9 }
                }
            }
        }

        // Tab Selection Buttons
        RowLayout {
            Layout.fillWidth: true
            spacing: 2

            Repeater {
                model: ["Recent Sessions", "Create New Session", "Audio & MIDI Engine Setup"]
                Rectangle {
                    Layout.fillWidth: true
                    height: 28
                    radius: 3
                    color: sessionSetupRoot.activeTab === index ? "#1976d2" : "#282e38"
                    Text {
                        anchors.centerIn: parent
                        text: modelData
                        color: sessionSetupRoot.activeTab === index ? "#ffffff" : "#9aa4b8"
                        font.pixelSize: 10
                        font.bold: true
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: sessionSetupRoot.activeTab = index
                    }
                }
            }
        }

        // Tab Stack Content Container
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#1b1e24"
            border.color: "#282d38"
            radius: 4

            StackLayout {
                anchors.fill: parent
                anchors.margins: 12
                currentIndex: sessionSetupRoot.activeTab

                // ──────── Tab 0: Recent Sessions ────────
                ColumnLayout {
                    spacing: 8

                    Text { text: "Recent Ardour Sessions:"; color: "#9aa4b8"; font.pixelSize: 10; font.bold: true }

                    ListView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        model: [
                            { name: "My First MultiTrack Session", path: "/home/devansh/ArdourSessions/MyFirstMultiTrack", rate: "48.0 kHz", date: "2026-09-06" },
                            { name: "Acoustic Guitar Mix", path: "/home/devansh/ArdourSessions/AcousticGuitarMix", rate: "44.1 kHz", date: "2026-09-05" },
                            { name: "Mastering Demo Project", path: "/home/devansh/ArdourSessions/MasteringDemo", rate: "96.0 kHz", date: "2026-09-04" }
                        ]
                        delegate: Rectangle {
                            width: ListView.view.width
                            height: 40
                            radius: 3
                            color: "#23272e"
                            border.color: "#2d323e"

                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 8
                                Text { text: modelData.name; color: "#e2e8f0"; font.pixelSize: 11; font.bold: true; Layout.fillWidth: true }
                                Text { text: modelData.rate; color: "#60a5fa"; font.pixelSize: 9; font.family: "Monospace" }
                                Text { text: modelData.date; color: "#64748b"; font.pixelSize: 9 }
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: sessionSetupRoot.launchWorkspaceRequested(modelData.name, modelData.path)
                            }
                        }
                    }

                    Button {
                        text: "Open Selected Session"
                        Layout.alignment: Qt.AlignRight
                        onClicked: sessionSetupRoot.launchWorkspaceRequested("My First MultiTrack Session", "/home/devansh/ArdourSessions/MyFirstMultiTrack")
                    }
                }

                // ──────── Tab 1: Create New Session ────────
                ColumnLayout {
                    spacing: 10

                    RowLayout {
                        spacing: 8
                        Text { text: "Session Name:"; color: "#9aa4b8"; font.pixelSize: 10; width: 100 }
                        TextField {
                            id: newSessionNameField
                            text: "New Session 1"
                            Layout.fillWidth: true
                        }
                    }

                    RowLayout {
                        spacing: 8
                        Text { text: "Folder Location:"; color: "#9aa4b8"; font.pixelSize: 10; width: 100 }
                        TextField {
                            id: newSessionPathField
                            text: "/home/devansh/ArdourSessions"
                            Layout.fillWidth: true
                        }
                    }

                    RowLayout {
                        spacing: 8
                        Text { text: "Project Template:"; color: "#9aa4b8"; font.pixelSize: 10; width: 100 }
                        ComboBox {
                            Layout.fillWidth: true
                            model: ["Empty Session", "Recording Template (8 Tracks)", "Mixing Template (16 Tracks + Busses)", "Mastering Suite Template"]
                        }
                    }

                    RowLayout {
                        spacing: 8
                        Text { text: "Sample Rate:"; color: "#9aa4b8"; font.pixelSize: 10; width: 100 }
                        ComboBox {
                            Layout.fillWidth: true
                            model: ["44.1 kHz", "48.0 kHz (Standard)", "88.2 kHz", "96.0 kHz"]
                            currentIndex: 1
                        }
                    }

                    Item { Layout.fillHeight: true }

                    Button {
                        text: "Create & Open Session"
                        Layout.alignment: Qt.AlignRight
                        onClicked: sessionSetupRoot.launchWorkspaceRequested(newSessionNameField.text, newSessionPathField.text + "/" + newSessionNameField.text)
                    }
                }

                // ──────── Tab 2: Audio & MIDI Engine Setup ────────
                ColumnLayout {
                    spacing: 10

                    RowLayout {
                        spacing: 8
                        Text { text: "Audio Subsystem:"; color: "#9aa4b8"; font.pixelSize: 10; width: 110 }
                        ComboBox {
                            Layout.fillWidth: true
                            model: ["ALSA (Direct Hardware)", "JACK Audio Connection Kit", "PulseAudio / PipeWire Bridge", "Dummy (Offline Testing)"]
                        }
                    }

                    RowLayout {
                        spacing: 8
                        Text { text: "Input Device:"; color: "#9aa4b8"; font.pixelSize: 10; width: 110 }
                        ComboBox {
                            Layout.fillWidth: true
                            model: ["default (system:capture_1/2)", "HDA Intel PCH", "USB Audio Interface"]
                        }
                    }

                    RowLayout {
                        spacing: 8
                        Text { text: "Output Device:"; color: "#9aa4b8"; font.pixelSize: 10; width: 110 }
                        ComboBox {
                            Layout.fillWidth: true
                            model: ["default (system:playback_1/2)", "HDA Intel PCH", "USB Audio Interface"]
                        }
                    }

                    RowLayout {
                        spacing: 8
                        Text { text: "Buffer Size:"; color: "#9aa4b8"; font.pixelSize: 10; width: 110 }
                        ComboBox {
                            Layout.fillWidth: true
                            model: ["128 samples (2.7 ms latency)", "256 samples (5.3 ms latency)", "512 samples (10.7 ms latency)", "1024 samples (21.3 ms latency)"]
                            currentIndex: 2
                        }
                    }

                    Item { Layout.fillHeight: true }

                    RowLayout {
                        Layout.fillWidth: true
                        Text { text: "Engine Status: Stopped"; color: "#f59e0b"; font.pixelSize: 9; font.bold: true }
                        Item { Layout.fillWidth: true }
                        Button {
                            text: "Apply & Connect Engine"
                            onClicked: sessionSetupRoot.launchWorkspaceRequested("Default Session", "/home/devansh/ArdourSessions/Default")
                        }
                    }
                }
            }
        }
    }
}
