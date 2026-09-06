import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import ".."

Rectangle {
    id: timelineCanvasRoot
    color: Theme.bgPanel
    clip: true

    property real pps: globalTrackModel.pixelsPerSecond || 50.0

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Main Flickable Timeline View
        Flickable {
            id: flickable
            Layout.fillWidth: true
            Layout.fillHeight: true
            contentWidth: Math.max(width, 3600)
            contentHeight: Math.max(height, trackLanesColumn.height + 95)
            boundsBehavior: Flickable.StopAtBounds

            Column {
                width: flickable.contentWidth

                // 1. Multi-Row Vertical Ruler Stack Header (Matching image2 exact rulers)
                Column {
                    width: parent.width

                    // Row 1: Timecode Ruler (00:00:00:00)
                    Rectangle {
                        width: parent.width; height: 16; color: Theme.bgHeader
                        Rectangle { anchors.bottom: parent.bottom; width: parent.width; height: 1; color: Theme.borderDark }
                        Text { x: 4; y: 2; text: "Timecode"; color: Theme.textMuted; font.pixelSize: 8; font.bold: true }

                        Row {
                            anchors.fill: parent; anchors.leftMargin: 65
                            Repeater {
                                model: Math.ceil(flickable.contentWidth / 100)
                                Rectangle {
                                    width: 100; height: 16; color: "transparent"
                                    Rectangle { anchors.left: parent.left; anchors.bottom: parent.bottom; width: 1; height: 6; color: Theme.borderDark }
                                    Text {
                                        anchors.left: parent.left; anchors.leftMargin: 4; anchors.verticalCenter: parent.verticalCenter
                                        text: "00:0" + (index * 2) + ":00:00"
                                        color: Theme.clockBlue; font.pixelSize: 8; font.family: "Monospace"
                                    }
                                }
                            }
                        }
                    }

                    // Row 2: Bars:Beats Ruler (1 2 3 4 5 6...)
                    Rectangle {
                        width: parent.width; height: 16; color: Theme.bgHeader
                        Rectangle { anchors.bottom: parent.bottom; width: parent.width; height: 1; color: Theme.borderDark }
                        Text { x: 4; y: 2; text: "Bars:Beats"; color: Theme.textMuted; font.pixelSize: 8; font.bold: true }

                        Row {
                            anchors.fill: parent; anchors.leftMargin: 65
                            Repeater {
                                model: Math.ceil(flickable.contentWidth / 60)
                                Rectangle {
                                    width: 60; height: 16; color: "transparent"
                                    Rectangle { anchors.left: parent.left; anchors.bottom: parent.bottom; width: 1; height: 8; color: Theme.clockGreen }
                                    Text {
                                        anchors.left: parent.left; anchors.leftMargin: 4; anchors.verticalCenter: parent.verticalCenter
                                        text: (index + 1).toString()
                                        color: Theme.clockGreen; font.pixelSize: 9; font.bold: true
                                    }
                                }
                            }
                        }
                    }

                    // Row 3: Time Signature Ruler
                    Rectangle {
                        width: parent.width; height: 14; color: Theme.bgHeader
                        Rectangle { anchors.bottom: parent.bottom; width: parent.width; height: 1; color: Theme.borderDark }
                        Text { x: 4; y: 1; text: "Time Signature"; color: Theme.textMuted; font.pixelSize: 8 }
                        Text { x: 80; y: 1; text: "4/4"; color: Theme.textMain; font.pixelSize: 8; font.bold: true }
                    }

                    // Row 4: Tempo Ruler
                    Rectangle {
                        width: parent.width; height: 14; color: Theme.bgHeader
                        Rectangle { anchors.bottom: parent.bottom; width: parent.width; height: 1; color: Theme.borderDark }
                        Text { x: 4; y: 1; text: "Tempo"; color: Theme.textMuted; font.pixelSize: 8 }
                        Text { x: 80; y: 1; text: "120.000 / 4"; color: Theme.clockBlue; font.pixelSize: 8; font.bold: true }
                    }

                    // Row 5: Loop / Punch Ranges Ruler
                    Rectangle {
                        width: parent.width; height: 16; color: Theme.bgHeader
                        Rectangle { anchors.bottom: parent.bottom; width: parent.width; height: 1; color: Theme.borderDark }
                        Text { x: 4; y: 2; text: "Loop/Punch Ranges"; color: Theme.textMuted; font.pixelSize: 8 }

                        // Loop Bar Overlay
                        Rectangle {
                            x: 80; y: 2; width: 280; height: 12; radius: 2
                            color: Theme.btnActive; border.color: Theme.btnActive
                            Text { anchors.centerIn: parent; text: "[L] Loop Range"; color: Theme.textMain; font.pixelSize: 8; font.bold: true }
                        }

                        // Punch Bar Overlay
                        Rectangle {
                            x: 400; y: 2; width: 180; height: 12; radius: 2
                            color: Theme.recRed; border.color: Theme.recRed
                            Text { anchors.centerIn: parent; text: "[P] Punch Range"; color: Theme.textMain; font.pixelSize: 8; font.bold: true }
                        }
                    }

                    // Row 6: Location Markers Ruler (start, weird sound, end)
                    Rectangle {
                        width: parent.width; height: 16; color: Theme.bgHeader
                        Rectangle { anchors.bottom: parent.bottom; width: parent.width; height: 1; color: Theme.borderDark }
                        Text { x: 4; y: 2; text: "Location Markers"; color: Theme.textMuted; font.pixelSize: 8 }

                        Row {
                            anchors.fill: parent; anchors.leftMargin: 65
                            Repeater {
                                model: globalTrackModel.markers()
                                Rectangle {
                                    x: modelData.positionX
                                    y: 1; width: Math.max(40, markerText.contentWidth + 10); height: 14; radius: 2
                                    color: Theme.clockGreen; border.color: Theme.clockGreen
                                    Text {
                                        id: markerText
                                        anchors.centerIn: parent
                                        text: modelData.name
                                        color: Theme.textMain; font.pixelSize: 8; font.bold: true
                                    }
                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: globalTrackModel.jumpToPosition(modelData.positionX)
                                    }
                                }
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: globalTrackModel.jumpToPosition(mouse.x)
                        }
                    }
                }

                // 2. Track Lanes Layout Column
                Column {
                    id: trackLanesColumn
                    width: parent.width

                    Repeater {
                        model: globalTrackModel

                        Rectangle {
                            id: trackLaneDelegate
                            width: trackLanesColumn.width
                            height: model.track_height || 64
                            color: index % 2 === 0 ? Theme.bgPanel : Theme.bgWindow
                            border.color: Theme.borderDark

                            property string trType: model.type || "audio"

                            // Vertical Grid Lines
                            Row {
                                anchors.fill: parent
                                Repeater {
                                    model: Math.ceil(flickable.contentWidth / 30)
                                    Rectangle {
                                        width: 30; height: parent.height; color: "transparent"
                                        Rectangle {
                                            anchors.right: parent.right; width: 1; height: parent.height
                                            color: Theme.borderDark
                                        }
                                    }
                                }
                            }

                            // If MIDI track, render piano keyboard strip on left
                            Rectangle {
                                visible: trackLaneDelegate.trType === "midi"
                                x: 0; y: 0; width: 22; height: parent.height
                                color: Theme.bgHeader; border.color: Theme.borderDark
                                Column {
                                    anchors.fill: parent
                                    Repeater {
                                        model: 8
                                        Rectangle {
                                            width: 22; height: parent.height / 8; color: index % 2 === 0 ? "#ffffff" : "#000000"; border.color: Theme.borderDark
                                        }
                                    }
                                }
                            }

                            // Region Block (Audio Waveform or MIDI Notes)
                            Rectangle {
                                x: (trackLaneDelegate.trType === "midi" ? 24 : 0) + 60 + (index * 110)
                                y: 3
                                width: 240
                                height: parent.height - 6
                                radius: 2
                                color: model.color_hex ? Qt.darker(model.color_hex, 1.8) : Theme.bgCard
                                border.color: model.color_hex || Theme.btnActive
                                border.width: 1

                                RowLayout {
                                    anchors.top: parent.top; anchors.left: parent.left; anchors.right: parent.right
                                    anchors.margins: 3
                                    Text {
                                        text: model.name + "_Take_" + (index + 1) + (trackLaneDelegate.trType === "midi" ? ".mid" : ".wav")
                                        color: Theme.textMain; font.pixelSize: 9; font.bold: true
                                        Layout.fillWidth: true; elide: Text.ElideRight
                                    }
                                }

                                // MIDI note blocks visualization if MIDI track
                                Repeater {
                                    model: trackLaneDelegate.trType === "midi" ? 12 : 0
                                    Rectangle {
                                        x: 10 + (index * 18); y: 15 + ((index * 7) % 35); width: 14; height: 5
                                        color: Theme.soloOrange; radius: 1
                                    }
                                }

                                // Audio Waveform Canvas if Audio track
                                Canvas {
                                    visible: trackLaneDelegate.trType !== "midi"
                                    anchors.fill: parent
                                    anchors.topMargin: 14
                                    onPaint: {
                                        var ctx = getContext("2d");
                                        ctx.strokeStyle = model.color_hex || Theme.clockBlue;
                                        ctx.lineWidth = 1.2;
                                        ctx.beginPath();
                                        var cy = height / 2;
                                        ctx.moveTo(0, cy);
                                        for (var x = 0; x < width; x += 3) {
                                            var amp = Math.sin(x * 0.09 + index) * (height * 0.4);
                                            ctx.lineTo(x, cy + amp);
                                        }
                                        ctx.stroke();
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Animated Playhead Line passing through all tracks
            Rectangle {
                id: playhead
                x: globalTransport.playheadPosition * timelineCanvasRoot.pps
                y: 0
                width: 2
                height: flickable.contentHeight
                color: Theme.recRed

                Canvas {
                    anchors.top: parent.top
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 10; height: 9
                    onPaint: {
                        var ctx = getContext("2d");
                        ctx.fillStyle = Theme.recRed;
                        ctx.beginPath();
                        ctx.moveTo(0, 0); ctx.lineTo(10, 0); ctx.lineTo(5, 9);
                        ctx.closePath(); ctx.fill();
                    }
                }
            }
        }

        // 3. Mini Timeline Overview Bar Stripe (Matching image2 bottom overview stripe)
        Rectangle {
            Layout.fillWidth: true
            implicitHeight: 18
            color: Theme.bgHeader
            border.color: Theme.borderDark

            // Viewport View Rectangle Frame
            Rectangle {
                x: 10; y: 2; width: 180; height: 14
                color: Theme.bgCard; border.color: Theme.btnActive; border.width: 1; radius: 2
            }

            // Playhead indicator in mini overview
            Rectangle {
                x: 10 + (globalTransport.playheadPosition * 2)
                y: 1; width: 2; height: 16; color: Theme.recRed
            }
        }
    }
}
