import QtQuick
import QtQuick.Layouts 1.12
import QtQuick.Controls
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12

import "../Elements" as Elements
import Playlists 1.0


Rectangle {
    id: plFileRec
//    anchors.fill: parent

    RowLayout {
        focus: true
        Layout.minimumHeight: 30
        Layout.maximumHeight: 30
        Layout.fillWidth: true
        spacing: 1
        // cursor
        TextField {
            id: lbl_cursor
            text: plistsView.currentIndex === index ? ">" : "  "
            color: "powderblue"
            hoverEnabled: true
            Layout.fillHeight: true
            Layout.minimumWidth: width_cursor
            Layout.maximumWidth: width_cursor
            Layout.topMargin: 1
            readOnly: true
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            selectByMouse: true
            font.pixelSize: 16
            font.bold: true
            KeyNavigation.down: bottom
            KeyNavigation.up: top

            background: Rectangle {
                id: back_cur
                anchors.fill: parent
                radius: 2
                color: setColor(lbl_cursor, index)
            }

            MouseArea {
                hoverEnabled: true
                anchors.fill: parent
                onClicked: plistsView.currentIndex = index
            }
        }

        TextField {
            id: edit_naim
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.topMargin: 1
            hoverEnabled: true
            KeyNavigation.down: bottom
            KeyNavigation.up: top
            placeholderText: qsTr("Enter playlist name")
            text: model.naim
            visible: true
            wrapMode: TextEdit.Wrap
            mouseSelectionMode: TextEdit.SelectCharacters
            selectByMouse: true
            focus: true
            readOnly: false

            background: Rectangle {
                id: back_naim
                anchors.fill: parent
                radius: 2
                color: setColor(edit_naim, index)
                border.color: {
                    if (edit_naim.activeFocus) {
                        "skyblue";
                    } else {
                        "transparent";
                    }
                }
            }

            Keys.onPressed: (event) => {
                                if (event.key === Qt.Key_Return
                                    || event.key === Qt.Key_Escape
                                    || event.key === Qt.Key_Down
                                    || event.key === Qt.Key_Up) {
                                    edit_naim.editingFinished();
                                    event.accepted = true;
                                }
                            }

            onEditingFinished: {
                focus = false;
                model.naim = text;
            }
            onPressed: plistsView.currentIndex = index;
        }
        CheckBox {
            id: cb_isCurrent
            text: qsTr("")
            checked: model.is_current
            Layout.fillHeight: true
            Layout.minimumWidth: width_isCurrent
            Layout.maximumWidth: width_isCurrent
            Layout.topMargin: 1

            indicator: Rectangle {
                id: indicator_out
                implicitWidth: width_isCurrent / 2
                implicitHeight: width_isCurrent / 2
                anchors.centerIn: cb_isCurrent
                radius: 15
                border.color: cb_isCurrent.down ? "lightgray" : "dimgray"
                border.width: 2

                Rectangle {
                    width: indicator_out.width - 4 - indicator_out.border.width
                    height: indicator_out.height - 4 - indicator_out.border.width
                    anchors.horizontalCenter: indicator_out.horizontalCenter
                    anchors.verticalCenter: indicator_out.verticalCenter
                    radius: 15
                    color: cb_isCurrent.down ? "lightgray" : "dimgray"
                    visible: cb_isCurrent.checked
                }
            }
            background: Rectangle {
                id: back_cbIsCur
                anchors.fill: cb_isCurrent
                radius: 2
                color: setColor(cb_isCurrent, index)
            }
            onClicked: plLists.setIsCurrent(index, checked)
        }
        MouseArea {
            hoverEnabled: true
            Layout.fillWidth: true
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            cursorShape: Qt.ArrowCursor
            onClicked: plistsView.currentIndex = index
        }
    }
    PlFilesView {
        id: plFilesForm
        Layout.fillWidth: true
        Layout.leftMargin: 30
        Layout.minimumHeight: 30
        visible: plistsView.currentIndex === index ? true : false
    }
    function setColor(curControl, curIndex) {
        if (curControl.hovered) {
            if (curControl.activeFocus) {
                return "lightgray";
            } else {
                if (curControl.enabled) {
                    return color_hoverAndEnabled;
                } else {
                    return color_hoverAndDisabled;
                }
            }
        } else {
            if (curControl.activeFocus) {
                return "lightgray";
            } else {
                if (curIndex === plistsView.currentIndex) {
                    return color_notHoverAndCurInd;
                } else {
                    return color_notHoverAndNotCurInd;
                }
            }
        }
    }
}

