import QtQuick
import QtQuick.Layouts 1.12
import QtQuick.Controls
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12

import "../Elements" as Elements
import "../functions.js" as Funcs
import Playlists 1.0

Item {
    id: _item_findEdit
    signal closeFindEdit;
    property bool isBtnBackVisible: true
    RowLayout {
        anchors.fill: parent
        spacing: 1
        Elements.ToolBtn {
            Layout.fillHeight: true
            Layout.maximumWidth: parent.height
            Layout.minimumWidth: parent.height
            btn_text: qsTr("")
            checkable: false
            ico_path: "qrc:/Qml/Ico/back.png"
            showHint: true
            hintText: qsTr("Back")
            onClicked: _item_findEdit.closeFindEdit();
            visible: isBtnBackVisible
        }
        Elements.MyTextEdit {
            id: _edit_find

            focus: true
            placeholderText: qsTr("Enter channel name")
            is_editable: true
            font.pixelSize: 12
            font.bold: false
            topPadding: 3
            bottomPadding: 3
            back_color: Funcs.setColor(_edit_find, 0, 0)
            border_color: Funcs.setBorderColor(true);
            Layout.fillWidth: true
            Layout.fillHeight: true
            Keys.onReleased: {
//                console.log("released: " + _edit_find.text);
                if (_cb_isFilter.checked)
                    channels.filter(_edit_find.text);
                if (_cb_isFind.checked)
                    channels.find(_edit_find.text);
                _edit_find.focus = true;
            }

            Elements.ToolBtn {
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                Layout.maximumHeight: parent.height
                Layout.maximumWidth: parent.height
                width: parent.height
                anchors.margins: 1

                btn_text: qsTr("")
                checkable: false
                ico_path: "qrc:/Qml/Ico/cancel.png"
                onClicked: {
                    _edit_find.text = "";
                    channels.filter(_edit_find.text);
                }
            }
        }
        ButtonGroup {
            id: _btnGrp
            exclusive: true
            checkState: _cb_isFilter.checkState
        }
        Elements.MyCheckBox {
            id: _cb_isFind
            Layout.fillHeight: true
            Layout.maximumWidth: 35
            Layout.minimumWidth: 35
            cb_height: 30
            cb_width: 30
            checked: false
            text: qsTr("find")
            ButtonGroup.group: _btnGrp
            onClicked: {
                channels.filter("");
                channels.find(_edit_find.text);
            }
        }
        Elements.MyCheckBox {
            id: _cb_isFilter
            Layout.fillHeight: true
            Layout.maximumWidth: 35
            Layout.minimumWidth: 35
            cb_height: 30
            cb_width: 30
            checked: true
            text: qsTr("filter")
            ButtonGroup.group: _btnGrp

        }
    }
}
