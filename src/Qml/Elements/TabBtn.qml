import QtQuick
import QtQuick.Controls
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12

TabButton {
    id: btn_

    property int curId: 0
    property string btn_text
    property int btn_height: 30
    property int btn_leftMargin: 0
    property string ico_path

    height: btn_height
    text: btn_text
    anchors.leftMargin: btn_leftMargin
    implicitWidth: tab.width

    contentItem: RowLayout {
        anchors.fill: parent
        Image {
            id: backgroundImage
            Layout.preferredHeight: parent.height
            Layout.preferredWidth: parent.height
            Layout.leftMargin: 2

            fillMode: Image.PreserveAspectFit
            source: ico_path
            visible: ico_path == "" ? false : true
        }
        Text {
            Layout.fillHeight: true
            Layout.fillWidth: true
            text: btn_text
            font: btn_.font
            opacity: enabled ? 1.0 : 0.3
            color: "white"


            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: ico_path == "" ? Text.AlignHCenter : Text.AlignLeft
            visible: btn_text === "" ? false : true
        }
    }


    background: Rectangle {        
        anchors.fill: parent
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
