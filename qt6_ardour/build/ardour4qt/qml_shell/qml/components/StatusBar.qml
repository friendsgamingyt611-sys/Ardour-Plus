import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import ".."

Rectangle {
    id: statusBarRoot
    implicitHeight: 24
    color: Theme.bgHeader

    Rectangle {
        anchors.top: parent.top
        width: parent.width
        height: 1
        color: Theme.borderDark
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        spacing: 16

        // Audio Engine Status
        RowLayout {
            spacing: 6
            Rectangle {
                width: 8; height: 8; radius: 4
                color: globalSession.engineRunning ? Theme.clockGreen : Theme.muteRed
            }
            Text {
                text: "ALSA / JACK (" + (globalSession.sampleRate / 1000.0).toFixed(1) + " kHz)"
                color: Theme.textMuted
                font.pixelSize: 10
                font.family: "Monospace"
            }
        }

        // Buffer Size
        Text {
            text: "Buf: " + globalSession.bufferSize + " samples (" + globalSession.bufferLatencyMs.toFixed(1) + "ms)"
            color: Theme.textMuted
            font.pixelSize: 10
            font.family: "Monospace"
        }

        // DSP Load Meter
        RowLayout {
            spacing: 6
            Text { text: "DSP:"; color: Theme.textMuted; font.pixelSize: 10 }
            Rectangle {
                width: 60; height: 8; radius: 2; color: Theme.bgPanel
                Rectangle {
                    width: parent.width * (globalSession.dspLoad / 100.0)
                    height: parent.height; radius: 2
                    color: globalSession.dspLoad > 85 ? Theme.muteRed : Theme.clockBlue
                }
            }
            Text {
                text: globalSession.dspLoad.toFixed(1) + "%"
                color: Theme.textMain
                font.pixelSize: 10
                font.family: "Monospace"
            }
        }

        // Timecode Format Indicator
        Text {
            text: "TC: " + globalSession.tcFormat
            color: Theme.textMuted
            font.pixelSize: 10
        }

        Item { Layout.fillWidth: true }

        // Disk Remaining
        RowLayout {
            spacing: 6
            Text { text: "Free Space:"; color: Theme.textMuted; font.pixelSize: 10 }
            Text {
                text: globalSession.diskSpaceFree
                color: Theme.soloOrange
                font.pixelSize: 10
                font.family: "Monospace"
                font.bold: true
            }
        }
    }
}
