import QtQuick
import QtQuick.Layouts 1.12
import QtQuick.Controls
import Qt.labs.platform 1.1
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12

import "../Elements" as Elements

Dialog {
    id: _dlgErrorForm
//    height: 127
//    width: 200
    modal: true

    property int _btn_height: 30
    property string titleText: qsTr("Error!")
    property string errorText: qsTr("Something wrong.")
    property string ico_path
    property string okBtnText: qsTr("Ok")

    signal okClicked

    onAccepted: console.log("Ok clicked")


    contentItem: Rectangle {
        id: _errorForm
        anchors.fill: parent
        opacity: 1
        border.color: "dimgray"
        color: "#424949"
        z: 10

        ColumnLayout {
            anchors.fill: parent
            anchors.topMargin: 2
            spacing: 2
            RowLayout {
                id: titleRow
                Layout.fillWidth: true
                height: 40
                Image {
                    id: titleImage
                    Layout.preferredHeight: _btn_height - 5
                    Layout.maximumHeight: _btn_height - 10
                    Layout.maximumWidth: ico_path == "" ? 0 : 30
                    fillMode: Image.PreserveAspectFit
                    source: ico_path
                    visible: ico_path == "" ? false : true
                }
                Text {
                    id: _text_title
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    text: titleText
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: ico_path == "" ? Text.AlignHCenter : Text.AlignLeft
                    wrapMode: Text.Wrap
                }
            }
            Text {
                id: _text_error
                Layout.fillWidth: true
                Layout.maximumHeight: 40
                text: errorText
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.Wrap
            }

            Elements.ToolBtn {
                id: okBtn
                z: 9
                opacity: 1
                Layout.fillWidth: true
                Layout.minimumHeight: _btn_height
                Layout.maximumHeight: _btn_height
                btn_height: _btn_height
//                btn_width: _btn_height
                btn_text: okBtnText
                ico_path: ""
                onClicked: _dlgErrorForm.okClicked()
            }
        }
    }
    Connections {
        target: _dlgErrorForm
        function onOkClicked() {
            console.log("func okClicked");
            _dlgErrorForm.accepted();
            _dlgErrorForm.close();
        }
    }
}
