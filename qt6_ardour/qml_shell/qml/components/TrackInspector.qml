import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import ".."

Rectangle {
    id: trackInspectorRoot
    implicitWidth: currentWidth
    color: Theme.bgPanel

    property int currentWidth: widthMode === 0 ? 95 : (widthMode === 1 ? 175 : 245)
    property int widthMode: 1 // 0: Narrow, 1: Normal, 2: Wide

    property int selIdx: globalTrackModel.selectedIndex >= 0 ? globalTrackModel.selectedIndex : 0
    property bool hasTrack: globalTrackModel.rowCount() > 0 && selIdx < globalTrackModel.rowCount()

    // Model bindings with safe defaults
    property int trackNumber: hasTrack ? (globalTrackModel.data(globalTrackModel.index(selIdx, 0), 270) || (selIdx + 1)) : 1
    property string trackName: hasTrack ? (globalTrackModel.data(globalTrackModel.index(selIdx, 0), 257) || "Track 1") : "No Track"
    property string trackType: hasTrack ? (globalTrackModel.data(globalTrackModel.index(selIdx, 0), 258) || "audio") : "audio"
    property string trackColor: hasTrack ? (globalTrackModel.data(globalTrackModel.index(selIdx, 0), 259) || "#3a8a55") : "#3a8a55"

    property bool isMuted: hasTrack ? (globalTrackModel.data(globalTrackModel.index(selIdx, 0), 261) || false) : false
    property bool isSoloed: hasTrack ? (globalTrackModel.data(globalTrackModel.index(selIdx, 0), 262) || false) : false
    property bool isSoloIso: hasTrack ? (globalTrackModel.data(globalTrackModel.index(selIdx, 0), 274) || false) : false
    property bool isRecArmed: hasTrack ? (globalTrackModel.data(globalTrackModel.index(selIdx, 0), 263) || false) : false
    property bool isRecSafe: hasTrack ? (globalTrackModel.data(globalTrackModel.index(selIdx, 0), 275) || false) : false
    property bool isPhaseInverted: hasTrack ? (globalTrackModel.data(globalTrackModel.index(selIdx, 0), 273) || false) : false

    property double faderDb: hasTrack ? (globalTrackModel.data(globalTrackModel.index(selIdx, 0), 264) || 0.0) : 0.0
    property double trimDb: hasTrack ? (globalTrackModel.data(globalTrackModel.index(selIdx, 0), 265) || 0.0) : 0.0
    property double panPos: hasTrack ? (globalTrackModel.data(globalTrackModel.index(selIdx, 0), 266) || 0.0) : 0.0
    property double panWidth: hasTrack ? (globalTrackModel.data(globalTrackModel.index(selIdx, 0), 276) || 100.0) : 100.0
    property bool panBypass: hasTrack ? (globalTrackModel.data(globalTrackModel.index(selIdx, 0), 277) || false) : false

    property string inputName: hasTrack ? (globalTrackModel.data(globalTrackModel.index(selIdx, 0), 271) || "system:capture_1") : "In 1"
    property string outputName: hasTrack ? (globalTrackModel.data(globalTrackModel.index(selIdx, 0), 272) || "master/in_1") : "Out Master"
    property string autoMode: hasTrack ? (globalTrackModel.data(globalTrackModel.index(selIdx, 0), 278) || "Manual") : "Manual"
    property int vcaGroup: hasTrack ? (globalTrackModel.data(globalTrackModel.index(selIdx, 0), 279) || 0) : 0

    property double peakLeftDb: hasTrack ? (globalTrackModel.data(globalTrackModel.index(selIdx, 0), 280) || -60.0) : -60.0
    property double peakRightDb: hasTrack ? (globalTrackModel.data(globalTrackModel.index(selIdx, 0), 281) || -60.0) : -60.0
    property bool peakClipped: hasTrack ? (globalTrackModel.data(globalTrackModel.index(selIdx, 0), 282) || false) : false

    // Border line on right edge
    Rectangle {
        anchors.right: parent.right
        width: 1; height: parent.height
        color: Theme.borderDark
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 3
        spacing: 2

        // ──────────────── 1. Header & Width Switcher Block ────────────────
        Rectangle {
            Layout.fillWidth: true
            implicitHeight: 44
            color: Theme.bgCard
            radius: 3
            border.color: Theme.borderDark

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 2
                spacing: 2

                // Width Switcher Buttons (N / M / W)
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 1

                    Repeater {
                        model: ["N", "Norm", "Wide"]
                        Rectangle {
                            Layout.fillWidth: true
                            height: 14
                            radius: 2
                            color: widthMode === index ? Theme.btnActive : Theme.btnNormal
                            Text {
                                anchors.centerIn: parent
                                text: modelData
                                color: widthMode === index ? Theme.textMain : Theme.textMuted
                                font.pixelSize: 8
                                font.bold: true
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: trackInspectorRoot.widthMode = index
                            }
                        }
                    }
                }

                // Track Number + Name + Color Tag
                Rectangle {
                    Layout.fillWidth: true
                    height: 20
                    radius: 2
                    color: trackInspectorRoot.trackColor

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 4
                        anchors.rightMargin: 4

                        Text {
                            text: "#" + trackInspectorRoot.trackNumber
                            color: "#ffffff"
                            font.pixelSize: 10
                            font.bold: true
                        }

                        Text {
                            text: trackInspectorRoot.trackName
                            color: "#ffffff"
                            font.pixelSize: 10
                            font.bold: true
                            Layout.fillWidth: true
                            elide: Text.ElideRight
                        }

                        Rectangle {
                            width: widthMode === 0 ? 18 : 32
                            height: 13; radius: 2; color: "#000000"; opacity: 0.35
                            Text {
                                anchors.centerIn: parent
                                text: trackInspectorRoot.trackType.substring(0, widthMode === 0 ? 1 : 4).toUpperCase()
                                color: "#ffffff"
                                font.pixelSize: 8
                                font.bold: true
                            }
                        }
                    }
                }
            }
        }

        // ──────────────── 2. I/O Routing Header ────────────────
        RowLayout {
            Layout.fillWidth: true
            spacing: 2

            Rectangle {
                Layout.fillWidth: true; height: 18; radius: 2
                color: Theme.btnNormal; border.color: Theme.borderDark
                Text {
                    anchors.centerIn: parent
                    text: widthMode === 0 ? "IN" : ("In: " + trackInspectorRoot.inputName)
                    color: Theme.textMuted; font.pixelSize: 8; elide: Text.ElideRight
                }
            }
            Rectangle {
                Layout.fillWidth: true; height: 18; radius: 2
                color: Theme.btnNormal; border.color: Theme.borderDark
                Text {
                    anchors.centerIn: parent
                    text: widthMode === 0 ? "OUT" : ("Out: " + trackInspectorRoot.outputName)
                    color: Theme.textMuted; font.pixelSize: 8; elide: Text.ElideRight
                }
            }
        }

        // ──────────────── 3. Phase Invert Ø & Trim Slider ────────────────
        RowLayout {
            Layout.fillWidth: true
            spacing: 2

            // Phase Invert Button
            Rectangle {
                width: 24; height: 20; radius: 2
                color: trackInspectorRoot.isPhaseInverted ? Theme.soloOrange : Theme.btnNormal
                border.color: trackInspectorRoot.isPhaseInverted ? Theme.soloOrange : Theme.borderDark
                Text {
                    anchors.centerIn: parent
                    text: "Ø"
                    color: trackInspectorRoot.isPhaseInverted ? Theme.textMain : Theme.textMuted
                    font.pixelSize: 11; font.bold: true
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: globalTrackModel.toggle_phase_invert(selIdx)
                }
            }

            // Pre-Trim Slider Box
            Rectangle {
                Layout.fillWidth: true
                height: 20; radius: 2
                color: Theme.bgHeader; border.color: Theme.borderDark

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 4; anchors.rightMargin: 4
                    spacing: 2

                    Text {
                        text: "Trim:"
                        color: Theme.textMuted; font.pixelSize: 8
                        visible: widthMode > 0
                    }

                    Text {
                        text: (trackInspectorRoot.trimDb >= 0 ? "+" : "") + trackInspectorRoot.trimDb.toFixed(1) + " dB"
                        color: Theme.clockBlue; font.pixelSize: 8; font.bold: true; font.family: "Monospace"
                        Layout.fillWidth: true
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onWheel: {
                        var step = wheel.angleDelta.y > 0 ? 0.5 : -0.5;
                        var newVal = Math.min(20.0, Math.max(-20.0, trackInspectorRoot.trimDb + step));
                        globalTrackModel.set_trim(selIdx, newVal);
                    }
                }
            }
        }

        // ──────────────── 4. Processor / Plugin Insert Slots ────────────────
        Rectangle {
            Layout.fillWidth: true
            implicitHeight: widthMode === 0 ? 36 : 54
            color: Theme.bgHeader
            border.color: Theme.borderDark
            radius: 2

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 2
                spacing: 1

                Text {
                    text: "Processors:"
                    color: Theme.textMuted; font.pixelSize: 8; font.bold: true
                    visible: widthMode > 0
                }

                Rectangle {
                    Layout.fillWidth: true; height: 14; color: Theme.bgCard; radius: 1
                    Text { anchors.centerIn: parent; text: "1. ACE Compressor"; color: Theme.textMain; font.pixelSize: 8 }
                }

                Rectangle {
                    Layout.fillWidth: true; height: 14; color: Theme.bgCard; radius: 1
                    Text { anchors.centerIn: parent; text: "2. ACE EQ"; color: Theme.textMain; font.pixelSize: 8 }
                }

                Rectangle {
                    Layout.fillWidth: true; height: 13; color: Theme.btnNormal; border.color: Theme.borderDark; radius: 1
                    Text { anchors.centerIn: parent; text: "+ Add Plugin"; color: Theme.btnActive; font.pixelSize: 7 }
                }
            }
        }

        // ──────────────── 5. Track Controls (Mute / Solo / Iso / Rec / Safe) ────────────────
        RowLayout {
            Layout.fillWidth: true
            spacing: 2

            // Mute (M)
            Rectangle {
                Layout.fillWidth: true; height: 22; radius: 2
                color: trackInspectorRoot.isMuted ? Theme.muteRed : Theme.btnNormal
                border.color: trackInspectorRoot.isMuted ? Theme.muteRed : Theme.borderDark
                Text {
                    anchors.centerIn: parent
                    text: "M"
                    color: trackInspectorRoot.isMuted ? Theme.textMain : Theme.textMuted
                    font.pixelSize: 10; font.bold: true
                }
                MouseArea { anchors.fill: parent; onClicked: globalTrackModel.toggle_mute(selIdx) }
            }

            // Solo (S)
            Rectangle {
                Layout.fillWidth: true; height: 22; radius: 2
                color: trackInspectorRoot.isSoloed ? Theme.soloOrange : Theme.btnNormal
                border.color: trackInspectorRoot.isSoloed ? Theme.soloOrange : Theme.borderDark
                Text {
                    anchors.centerIn: parent
                    text: "S"
                    color: trackInspectorRoot.isSoloed ? Theme.textMain : Theme.textMuted
                    font.pixelSize: 10; font.bold: true
                }
                MouseArea { anchors.fill: parent; onClicked: globalTrackModel.toggle_solo(selIdx) }
            }

            // Solo Iso (Iso)
            Rectangle {
                width: widthMode === 0 ? 18 : 26; height: 22; radius: 2
                color: trackInspectorRoot.isSoloIso ? Theme.soloOrange : Theme.btnNormal
                border.color: Theme.borderDark
                Text {
                    anchors.centerIn: parent
                    text: "Iso"
                    color: trackInspectorRoot.isSoloIso ? Theme.textMain : Theme.textMuted
                    font.pixelSize: 8; font.bold: true
                }
                MouseArea { anchors.fill: parent; onClicked: globalTrackModel.toggle_solo_iso(selIdx) }
            }

            // Rec Arm (R)
            Rectangle {
                Layout.fillWidth: true; height: 22; radius: 2
                color: trackInspectorRoot.isRecArmed ? Theme.recRed : Theme.btnNormal
                border.color: Theme.borderDark
                Text {
                    anchors.centerIn: parent
                    text: "R"
                    color: trackInspectorRoot.isRecArmed ? Theme.textMain : Theme.textMuted
                    font.pixelSize: 10; font.bold: true
                }
                MouseArea { anchors.fill: parent; onClicked: globalTrackModel.toggle_rec_arm(selIdx) }
            }

            // Rec Safe (Safe)
            Rectangle {
                width: widthMode === 0 ? 18 : 32; height: 22; radius: 2
                color: trackInspectorRoot.isRecSafe ? Theme.clockGreen : Theme.btnNormal
                border.color: Theme.borderDark
                Text {
                    anchors.centerIn: parent
                    text: "Safe"
                    color: trackInspectorRoot.isRecSafe ? Theme.textMain : Theme.textMuted
                    font.pixelSize: 8; font.bold: true
                }
                MouseArea { anchors.fill: parent; onClicked: globalTrackModel.toggle_rec_safe(selIdx) }
            }
        }

        // ──────────────── 6. Automation Mode & VCA Selector ────────────────
        RowLayout {
            Layout.fillWidth: true
            spacing: 2

            // Auto Mode Switcher
            Rectangle {
                Layout.fillWidth: true; height: 18; radius: 2
                color: Theme.btnNormal; border.color: Theme.borderDark
                Text {
                    anchors.centerIn: parent
                    text: "Auto: " + trackInspectorRoot.autoMode
                    color: Theme.textMain; font.pixelSize: 8; font.bold: true
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        var modes = ["Manual", "Play", "Write", "Touch"];
                        var curIdx = modes.indexOf(trackInspectorRoot.autoMode);
                        var nxtMode = modes[(curIdx + 1) % modes.length];
                        globalTrackModel.set_automation_mode(selIdx, nxtMode);
                    }
                }
            }

            // VCA Group Switcher
            Rectangle {
                width: widthMode === 0 ? 30 : 52; height: 18; radius: 2
                color: Theme.btnNormal; border.color: Theme.borderDark
                Text {
                    anchors.centerIn: parent
                    text: trackInspectorRoot.vcaGroup > 0 ? ("VCA " + trackInspectorRoot.vcaGroup) : "VCA: --"
                    color: trackInspectorRoot.vcaGroup > 0 ? Theme.soloOrange : Theme.textMuted
                    font.pixelSize: 8; font.bold: true
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        var nxtVca = (trackInspectorRoot.vcaGroup + 1) % 9;
                        globalTrackModel.set_vca_group(selIdx, nxtVca);
                    }
                }
            }
        }

        // ──────────────── 7. Fader & Dual Stereo Peak Level Meter Block ────────────────
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 3

            // Peak Meter Column
            ColumnLayout {
                Layout.fillHeight: true
                spacing: 1

                // Overload / Clip Readout Box
                Rectangle {
                    width: 18; height: 14; radius: 2
                    color: (globalTransport.isPlaying && trackInspectorRoot.faderDb > 3.0) ? Theme.muteRed : Theme.bgHeader
                    border.color: Theme.borderDark
                    Text {
                        anchors.centerIn: parent
                        text: (globalTransport.isPlaying && trackInspectorRoot.faderDb > 3.0) ? "CLIP" : "-inf"
                        color: (globalTransport.isPlaying && trackInspectorRoot.faderDb > 3.0) ? Theme.textMain : Theme.textMuted
                        font.pixelSize: 7; font.bold: true
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: globalTrackModel.reset_peak_clip(selIdx)
                    }
                }

                // Dual Peak Meters (L & R)
                Rectangle {
                    width: 18
                    Layout.fillHeight: true
                    radius: 2
                    color: Theme.bgHeader
                    border.color: Theme.borderDark

                    Row {
                        anchors.fill: parent
                        anchors.margins: 1
                        spacing: 1

                        // Left Channel Peak Meter
                        Rectangle {
                            width: 7; height: parent.height; color: Theme.bgHeader
                            Rectangle {
                                anchors.bottom: parent.bottom; width: parent.width
                                height: parent.height * (globalTransport.isPlaying ? Math.min(1.0, Math.max(0.05, (trackInspectorRoot.faderDb + 60.0) / 66.0)) : 0.03)
                                color: height > parent.height * 0.88 ? Theme.muteRed : (height > parent.height * 0.7 ? Theme.soloOrange : Theme.clockGreen)
                            }
                        }

                        // Right Channel Peak Meter
                        Rectangle {
                            width: 7; height: parent.height; color: Theme.bgHeader
                            Rectangle {
                                anchors.bottom: parent.bottom; width: parent.width
                                height: parent.height * (globalTransport.isPlaying ? Math.min(1.0, Math.max(0.04, (trackInspectorRoot.faderDb + 58.0) / 66.0)) : 0.03)
                                color: height > parent.height * 0.88 ? Theme.muteRed : (height > parent.height * 0.7 ? Theme.soloOrange : Theme.clockGreen)
                            }
                        }
                    }
                }
            }

            // Fader & dB Readouts Column
            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 2

                // Fader dB Numeric Readout Box
                Rectangle {
                    Layout.fillWidth: true
                    height: 18
                    color: Theme.bgHeader
                    border.color: Theme.borderDark
                    radius: 2

                    Text {
                        anchors.centerIn: parent
                        text: (trackInspectorRoot.faderDb >= 0 ? "+" : "") + trackInspectorRoot.faderDb.toFixed(2) + " dB"
                        color: Theme.clockGreen
                        font.pixelSize: 9
                        font.bold: true
                        font.family: "Monospace"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onDoubleClicked: globalTrackModel.set_volume(selIdx, 0.0)
                    }
                }

                // Vertical Fader Slider + dB Scale Markings
                RowLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: 1

                    // dB Markings Column (visible in Normal & Wide mode)
                    Column {
                        Layout.fillHeight: true
                        spacing: 12
                        visible: widthMode > 0
                        Text { text: "+6"; color: Theme.textMuted; font.pixelSize: 7 }
                        Text { text: " 0"; color: Theme.textMuted; font.pixelSize: 7 }
                        Text { text: "-6"; color: Theme.textMuted; font.pixelSize: 7 }
                        Text { text: "-18"; color: Theme.textMuted; font.pixelSize: 7 }
                        Text { text: "-36"; color: Theme.textMuted; font.pixelSize: 7 }
                        Text { text: "-inf"; color: Theme.textMuted; font.pixelSize: 7 }
                    }

                    // Vertical Slider
                    Slider {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        orientation: Qt.Vertical
                        from: -60.0
                        to: 6.0
                        value: trackInspectorRoot.faderDb
                        onValueChanged: {
                            if (Math.abs(value - trackInspectorRoot.faderDb) > 0.01) {
                                globalTrackModel.set_volume(selIdx, value);
                            }
                        }
                    }
                }
            }
        }

        // ──────────────── 8. Stereo Pan Azimuth & Width Block ────────────────
        Rectangle {
            Layout.fillWidth: true
            implicitHeight: 40
            color: Theme.bgCard
            border.color: Theme.borderDark
            radius: 2

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 2
                spacing: 1

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 2

                    // Pan Azimuth Readout Button / Knob
                    Rectangle {
                        Layout.fillWidth: true; height: 16; radius: 2
                        color: Theme.btnNormal; border.color: Theme.borderDark
                        Text {
                            anchors.centerIn: parent
                            text: trackInspectorRoot.panPos < 0 ? ("L" + Math.round(Math.abs(trackInspectorRoot.panPos * 100)))
                                : (trackInspectorRoot.panPos > 0 ? ("R" + Math.round(trackInspectorRoot.panPos * 100)) : "C")
                            color: Theme.clockBlue; font.pixelSize: 8; font.bold: true; font.family: "Monospace"
                        }
                    }

                    // Pan Width Readout
                    Rectangle {
                        width: widthMode === 0 ? 25 : 45; height: 16; radius: 2
                        color: Theme.btnNormal; border.color: Theme.borderDark
                        Text {
                            anchors.centerIn: parent
                            text: Math.round(trackInspectorRoot.panWidth) + "%"
                            color: Theme.clockGreen; font.pixelSize: 8; font.bold: true; font.family: "Monospace"
                        }
                    }

                    // Pan Bypass Button
                    Rectangle {
                        width: 22; height: 16; radius: 2
                        color: trackInspectorRoot.panBypass ? Theme.muteRed : Theme.btnNormal
                        border.color: Theme.borderDark
                        Text {
                            anchors.centerIn: parent
                            text: "Byp"
                            color: trackInspectorRoot.panBypass ? Theme.textMain : Theme.textMuted
                            font.pixelSize: 7; font.bold: true
                        }
                        MouseArea { anchors.fill: parent; onClicked: globalTrackModel.toggle_pan_bypass(selIdx) }
                    }
                }

                // Horizontal Pan Azimuth Slider
                Slider {
                    Layout.fillWidth: true
                    implicitHeight: 16
                    from: -1.0; to: 1.0
                    value: trackInspectorRoot.panPos
                    onValueChanged: {
                        if (Math.abs(value - trackInspectorRoot.panPos) > 0.01) {
                            globalTrackModel.set_pan(selIdx, value);
                        }
                    }
                }
            }
        }
    }
}
