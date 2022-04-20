import QtQuick
import QtQuick.Layouts 1.12
import QtQuick.Controls
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12


import "../functions.js" as Funcs

CheckBox {
    id: _control

    property int cb_width: _control.width
    property int cb_height: _control.height
    property int index: 0
    property int currentIndex: 1
    font.pixelSize: 10

    indicator: Rectangle {
        id: _indicator_out
        implicitWidth: cb_width / 2
        implicitHeight: cb_height / 2
        anchors.verticalCenter: _control.verticalCenter
        anchors.left: _control.left
        anchors.leftMargin: _control.text === "" ? _control.width / 2 - cb_width / 4 : 0

        radius: 15
        border.color: _control.down ? "lightgray" : "dimgray"
        border.width: 2

        Rectangle {
            width: _indicator_out.width - 4 - _indicator_out.border.width
            height: _indicator_out.height - 4 - _indicator_out.border.width
            anchors.horizontalCenter: _indicator_out.horizontalCenter
            anchors.verticalCenter: _indicator_out.verticalCenter
            radius: 15
            color: _control.down ? "lightgray" : "dimgray"
            visible: _control.checked
        }
    }
    contentItem: Text {
        anchors.left: _indicator_out.right
        anchors.top: _control.top
        anchors.bottom: _control.bottom
        anchors.right: _control.right
        wrapMode: Text.WrapAnywhere
        text: _control.text
        font: _control.font
        opacity: enabled ? 1.0 : 0.3
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        elide: Text.ElideLeft
        color: _control.down ? "yellow" : "white"
        visible: _control.text === "" ? false : true
    }
    background: Rectangle {
        id: _back_cbIsAvailable
        anchors.fill: _control
        radius: 2
        color: Funcs.setColor(_back_cbIsAvailable, index, currentIndex)
        border.color: Funcs.setBorderColor(_control.activeFocus);
        opacity: _control.down ? 0.5 : 1
    }
}
