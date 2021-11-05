import QtQuick
import QtQuick.Controls
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12

TabButton {
    id: btn_

    property int curId: 0
    property string btn_text
    property int btn_width: 50
    property int btn_leftMargin: 0
    property string ico_path

    width: btn_width
    height: 30
    text: btn_text
    anchors.leftMargin: btn_leftMargin


    contentItem: RowLayout {
        anchors.fill: parent
        spacing: 1
        Image {
            id: backgroundImage
            Layout.fillHeight: true
            Layout.leftMargin: 2
            height: 30
            width: 30
            fillMode: Image.PreserveAspectFit
            source: ico_path
            visible: ico_path == "" ? false : true
        }
        Text {
            Layout.fillHeight: true
            Layout.fillWidth: true
            text: btn_.text
            font: btn_.font
            opacity: enabled ? 1.0 : 0.3
            color: "white"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: ico_path == "" ? Text.AlignHCenter : Text.AlignLeft
            elide: Text.ElideLeft
            visible: btn_.text == "" ? false : true
        }
    }

    background: Rectangle {
        anchors.fill: parent
        anchors.leftMargin: btn_.btn_leftMargin
        opacity: enabled ? 1 : 0.3
        color: {
            if (tab.currentIndex === btn_.curId) {
                if (btn_.down)
                    "lightgray";
                else
                    "slategray";
            } else {
                if (btn_.down)
                    "lightgray";
                else
                    "transparent";
            }
        }
        radius: 2
    }
}
