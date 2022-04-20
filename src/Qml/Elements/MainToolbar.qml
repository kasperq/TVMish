import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ToolBar {
    RowLayout {
        anchors.fill: parent
        ToolButton {
            text: "TV"
        }
        ToolButton {
            text: "Prefs."
        }
        ToolButton {
            text: "Pl.Mang."
        }
        ToolButton {
            text: "Schdl"
        }
    }
}
