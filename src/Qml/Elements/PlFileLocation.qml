import QtQuick
import QtQuick.Layouts 1.12
import QtQuick.Controls
import Qt.labs.platform 1.1
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12

import "../Elements" as Elements

Dialog {
    id: _dlgLocationForm
    height: 127
    width: 200
    modal: true

    property int _btn_height: 30
    property string file_path: plFiles.getClipboardString()
    property url fileUrl

    signal okClicked
    signal cancelClicked
    signal localFileSelected
    signal buferUrlSelected

    onAccepted: console.log("Ok clicked")
    onRejected: console.log("Cancel clicked")



    contentItem: Rectangle {
        id: _formFileLocation
        anchors.fill: parent
        opacity: 1
        border.color: "dimgray"
        color: "#424949"
        z: 10

        ColumnLayout {
            anchors.fill: parent
            anchors.topMargin: 2
            spacing: 2
            Flickable {
                id: rect_buffText
                clip: true
                Layout.fillWidth: true
                Layout.maximumHeight: 150
                Layout.minimumHeight:  50
                Text {
                    id: lbl_buffer
                    text: file_path
                    textFormat: Text.RichText
                    verticalAlignment: Text.AlignTop
                    horizontalAlignment: Text.AlignLeft
                    elide: Text.ElideLeft
                    wrapMode: Text.Wrap
                    z: 1
                    x: -hbar.position * width
                    y: -vbar.position * height
                }
                ScrollBar {
                    id: vbar
                    hoverEnabled: true
                    active: hovered || pressed
                    orientation: Qt.Vertical
                    size: rect_buffText.height / lbl_buffer.height
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                }
                ScrollBar {
                    id: hbar
                    hoverEnabled: true
                    active: hovered || pressed
                    orientation: Qt.Horizontal
                    size: rect_buffText.width / lbl_buffer.width
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                }

            }
            ColumnLayout {
                id: panel_fileAction
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 1
                Elements.ToolBtn {
                    id: btn_fromLocal
                    z: 9
                    opacity: 1
                    Layout.fillWidth: true
                    btn_height: _btn_height
                    btn_width: panel_fileAction.width
                    btn_text: qsTr("Local Storage")
                    ico_path: ""
                    onClicked: {
                        console.log("open local: " + plFiles.curDir);
                        fileDialog.folder = plFiles.curDir;
                        fileDialog.open();
                    }
                }
                Elements.ToolBtn {
                    id: btn_fromBuf
                    z: 9
                    opacity: 1
                    Layout.fillWidth: true
                    btn_height: _btn_height
                    btn_width: panel_fileAction.width
                    btn_text: qsTr("From Buffer")
                    ico_path: ""
                    onClicked: {
                        file_path = plFiles.getClipboardString();
                        console.log("clipboard: " + file_path);
                        okBtn.clicked();
                    }
                }
                Rectangle {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    opacity: 0
                }
                FileDialog {
                    id: fileDialog
                    title: qsTr("Please choose a playlist file")
                    fileMode: FileDialog.OpenFile
                    onAccepted: {
//                        file_path = fileDialog.folder;
                        file_path = fileDialog.currentFile;
                        fileUrl = fileDialog.currentFile;
                        console.log("file_path: " + file_path);                        
                        okBtn.clicked();
                        localFileSelected();
                    }
                    onRejected: {
                        console.log("Canceled")
                    }
                }
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
                    btn_text: qsTr("OK")
                    ico_path: ""
                    onClicked: _dlgLocationForm.okClicked()
                }
                Elements.ToolBtn {
                    id: cancelBtn
                    z: 9
                    opacity: 1
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    btn_height: panel_toolbar.height
                    btn_width: panel_toolbar.width
                    btn_text: qsTr("Cancel")
                    ico_path: ""
                    onClicked: _dlgLocationForm.cancelClicked()
                }
            }
        }
    }
    Connections {
        target: _dlgLocationForm
        function onCancelClicked() {
            console.log("func cancelClicked");
            _dlgLocationForm.rejected();
            _dlgLocationForm.close();
        }
        function onOkClicked() {
            console.log("func okClicked");
            _dlgLocationForm.accepted();
            _dlgLocationForm.close();
        }
    }       
}
