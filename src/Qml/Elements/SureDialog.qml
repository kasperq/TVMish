import QtQuick
import QtQuick.Layouts 1.12
import QtQuick.Controls
import Qt.labs.platform 1.1
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12

import "../Elements" as Elements

Dialog {
    id: _dlgQuestionForm
//    height: 127
//    width: 200
    modal: true

    property int _btn_height: 30
    property string titleText
    property string questionText
    property string ico_path
    property string okBtnText: qsTr("Ok")
    property string cancelBtnText: qsTr("Cancel")

    signal okClicked
    signal cancelClicked

    onAccepted: console.log("Ok clicked")
    onRejected: console.log("Cancel clicked")



    contentItem: Rectangle {
        id: _questionForm
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
                id: _text_question
                Layout.fillWidth: true
                Layout.maximumHeight: 40
                text: questionText
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.Wrap
            }

            RowLayout {
                id: panel_toolbar
                Layout.minimumHeight: _btn_height
                Layout.maximumHeight: _btn_height
                Layout.fillWidth: true
                spacing: 1
                Elements.ToolBtn {
                    id: okBtn
                    z: 9
                    opacity: 1
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    btn_height: panel_toolbar.height
                    btn_width: panel_toolbar.width
                    btn_text: okBtnText
                    ico_path: ""
                    onClicked: _dlgQuestionForm.okClicked()
                }
                Elements.ToolBtn {
                    id: cancelBtn
                    z: 9
                    opacity: 1
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    btn_height: panel_toolbar.height
                    btn_width: panel_toolbar.width
                    btn_text: cancelBtnText
                    ico_path: ""
                    onClicked: _dlgQuestionForm.cancelClicked()
                }
            }
        }
    }
    Connections {
        target: _dlgQuestionForm
        function onCancelClicked() {
            console.log("func cancelClicked");
            _dlgQuestionForm.rejected();
            _dlgQuestionForm.close();
        }
        function onOkClicked() {
            console.log("func okClicked");
            _dlgQuestionForm.accepted();
            _dlgQuestionForm.close();
        }
    }
}
