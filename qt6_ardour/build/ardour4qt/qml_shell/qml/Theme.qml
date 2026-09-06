pragma Singleton
import QtQuick 2.15

QtObject {
    id: theme

    // Official Ardour GTK2/GTK4 Color Tokens (dark-ardour.colors / colours.css)
    readonly property color bgWindow:       "#1e1e1e" // neutral:background
    readonly property color bgPanel:        "#242424" // theme:bg
    readonly property color bgCard:         "#313131" // theme:bg1 / neutral:midground
    readonly property color bgHeader:       "#161616" // theme:bg2
    readonly property color bgCanvas:       "#242424" // theme:bg
    readonly property color bgInput:        "#1e1e1e" // neutral:background
    readonly property color bgTrackHeader:  "#2a2a2a" // neutral:background2
    readonly property color bgTrackSelected:"#3584e4" // theme:contrasting selection

    readonly property color borderDark:     "#161616"
    readonly property color borderLight:    "#393939"
    readonly property color borderFocus:    "#3584e4"

    readonly property color textMain:       "#f1f3f5" // neutral:foreground (soft off-white)
    readonly property color textMuted:      "#868e96" // neutral:foreground2
    readonly property color textSubtle:     "#6c757d"

    readonly property color clockBlue:      "#66a3d8" // theme:contrasting clock
    readonly property color clockGreen:     "#2ec27e" // alert:green
    readonly property color clockYellow:    "#f5c211" // alert:yellow

    readonly property color btnNormal:      "#393939" // widget:bg
    readonly property color btnHover:       "#454545"
    readonly property color btnActive:      "#3584e4" // widget:blue / accent

    readonly property color soloOrange:     "#fd7e14" // widget:orange
    readonly property color muteRed:        "#c01c28" // alert:red
    readonly property color recRed:         "#c01c28" // alert:red
    readonly property color loopBlue:       "#3584e4" // accent
    readonly property color punchRose:      "#fa5252" // theme:contrasting

    readonly property font defaultFont: Qt.font({ family: "Sans-Serif", pixelSize: 11 })
    readonly property font clockFont:   Qt.font({ family: "Monospace", pixelSize: 13, bold: true })
    readonly property font miniFont:    Qt.font({ family: "Sans-Serif", pixelSize: 9 })
}
