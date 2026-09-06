import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import ".."

Rectangle {
    id: preferencesViewRoot
    color: Theme.bgWindow

    property int activeTab: 0

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 10

        // Header Title Bar
        Rectangle {
            Layout.fillWidth: true; height: 44; color: Theme.bgCard; radius: 4
            RowLayout {
                anchors.fill: parent; anchors.leftMargin: 12; anchors.rightMargin: 12
                Text { text: "Ardour Preferences & Engine Configuration (RCOptionEditor)"; color: Theme.textMain; font.pixelSize: 14; font.bold: true }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 8

            // Left Navigation Category List
            Rectangle {
                width: 180; Layout.fillHeight: true; color: Theme.bgPanel; border.color: Theme.borderDark; radius: 4
                ColumnLayout {
                    anchors.fill: parent; anchors.margins: 4; spacing: 2
                    Repeater {
                        model: ["Audio & MIDI Engine", "Performance & DSP", "Monitoring & Metering", "Control Surfaces", "Keyboard Shortcuts"]
                        Rectangle {
                            Layout.fillWidth: true; height: 28; radius: 3
                            color: preferencesViewRoot.activeTab === index ? Theme.btnActive : Theme.btnNormal
                            Text {
                                anchors.left: parent.left; anchors.leftMargin: 8; anchors.verticalCenter: parent.verticalCenter
                                text: modelData; color: preferencesViewRoot.activeTab === index ? Theme.textMain : Theme.textMuted; font.pixelSize: 10; font.bold: true
                            }
                            MouseArea { anchors.fill: parent; onClicked: preferencesViewRoot.activeTab = index }
                        }
                    }
                    Item { Layout.fillHeight: true }
                }
            }

            // Right Preferences Content View Stack
            Rectangle {
                Layout.fillWidth: true; Layout.fillHeight: true; color: Theme.bgPanel; border.color: Theme.borderDark; radius: 4

                StackLayout {
                    anchors.fill: parent; anchors.margins: 16
                    currentIndex: preferencesViewRoot.activeTab

                    // Category 0: Audio Engine Settings
                    ColumnLayout {
                        spacing: 12
                        Text { text: "Audio Subsystem & Device Settings"; color: Theme.textMain; font.pixelSize: 12; font.bold: true }

                        RowLayout {
                            Text { text: "Sample Rate:"; color: Theme.textMuted; Layout.preferredWidth: 120 }
                            ComboBox { model: ["44100 Hz", "48000 Hz", "88200 Hz", "96000 Hz"]; currentIndex: 1 }
                        }
                        RowLayout {
                            Text { text: "Buffer Size:"; color: Theme.textMuted; Layout.preferredWidth: 120 }
                            ComboBox { model: ["128 samples", "256 samples", "512 samples", "1024 samples"]; currentIndex: 1 }
                        }
                        RowLayout {
                            Text { text: "Driver Backend:"; color: Theme.textMuted; Layout.preferredWidth: 120 }
                            ComboBox { model: ["ALSA", "JACK", "PulseAudio / PipeWire", "Dummy"] }
                        }
                        Item { Layout.fillHeight: true }
                    }

                    // Category 1: Performance
                    ColumnLayout {
                        spacing: 12
                        Text { text: "Performance & Threading Settings"; color: Theme.textMain; font.pixelSize: 12; font.bold: true }
                        CheckBox { text: "Use Realtime Scheduling Priority"; checked: true }
                        CheckBox { text: "Enable Process Buffering / Disk Thread Pool"; checked: true }
                        Item { Layout.fillHeight: true }
                    }

                    // Category 2: Monitoring
                    ColumnLayout {
                        spacing: 12
                        Text { text: "Hardware & Software Monitoring Model"; color: Theme.textMain; font.pixelSize: 12; font.bold: true }
                        ComboBox { model: ["Ardour Hardware Monitoring", "External Hardware Monitoring", "Audio Engine Software Monitoring"] }
                        Item { Layout.fillHeight: true }
                    }

                    // Category 3: Control Surfaces
                    ColumnLayout {
                        spacing: 12
                        Text { text: "Control Surface Protocol Plugins"; color: Theme.textMain; font.pixelSize: 12; font.bold: true }
                        CheckBox { text: "Open Sound Control (OSC)"; checked: true }
                        CheckBox { text: "Mackie Control Universal (MCU)"; checked: false }
                        Item { Layout.fillHeight: true }
                    }

                    // Category 4: Shortcuts
                    ColumnLayout {
                        spacing: 12
                        Text { text: "Keyboard Shortcuts & Keymap Bindings"; color: Theme.textMain; font.pixelSize: 12; font.bold: true }
                        Text { text: "Default Preset: Ardour Standard Keymap (8.6)"; color: Theme.clockBlue; font.pixelSize: 10 }
                        Item { Layout.fillHeight: true }
                    }
                }
            }
        }
    }
}
