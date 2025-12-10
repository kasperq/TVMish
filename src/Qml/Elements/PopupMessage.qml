import QtQuick
import QtQuick.Layouts 1.12
import QtQuick.Controls
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12

Popup {
    id: _popup

    property string btn_text
    property string ico_path

    width: (btn_text == "" || ico_path == "") ? 30 : (60)
    height: 30
    modal: false
    focus: true
//    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

    contentItem: RowLayout {
        anchors.fill: parent
        spacing: 1
        Rectangle {
            id: fillerRectLeft
            Layout.fillHeight: true
            Layout.fillWidth: true
            opacity: 0
        }
        Image {
            id: _img_back

            Layout.fillHeight: true
            Layout.fillWidth: true

            fillMode: Image.PreserveAspectFit
            source: ico_path
            visible: ico_path == "" ? false : true
            verticalAlignment: Image.AlignVCenter
            horizontalAlignment: /*btn_.text === "" ? Image.AlignLeft : */Image.AlignHCenter
        }
        Text {
            id: _txt_back
            Layout.fillWidth: true
            Layout.fillHeight: true
            wrapMode: Text.WrapAnywhere
            text: btn_text
//            font: btn_.font
            opacity: enabled ? 1.0 : 0.3
            color: "white"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: ico_path == "" ? Text.AlignHCenter : Text.AlignLeft
            elide: Text.ElideLeft

            visible: btn_text === "" ? false : true
        }
        Rectangle {
            id: fillerRect
            Layout.fillHeight: true
            Layout.fillWidth: true
            opacity: 0
        }
    }

    background: Rectangle {
        id: btn_back
        anchors.fill: parent
        opacity: enabled ? 1 : 0.3
        radius: 2
        border.color: "dimgray"
        border.width: 0
        color: "dimgray"
    }

    onOpened: close();

    enter: Transition {
            NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 }
        }
    exit: Transition {
            NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 1000 }
        }

}
