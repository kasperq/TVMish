import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Universal 2.12

//import "./Elements" as Elements

Item {
    id: plLstMngToolbars    
    property int curIndex
    property int toolbar_height: 30

    Rectangle {
        id: _toolbar_playlists
        height: 30
        Layout.fillWidth: true
        radius: 2
        color: "darkgray"
        opacity: 1
        z: 100
        RowLayout {
            id: rwBtn
            spacing: 1
            layoutDirection: Qt.LeftToRight
            Layout.alignment: Qt.AlignLeft
            anchors.fill: parent

            ToolBtn {
                id: btn_addPl
                Layout.fillHeight: true
                Layout.minimumWidth: _toolbar_playlists.height
                Layout.maximumWidth: _toolbar_playlists.height
                btn_height: 30
                btn_width: 30
                btn_text: qsTr("")
                hintText: qsTr("Add new playlist")
                ico_path: "qrc:/Qml/Ico/file-add.png"
                onClicked: {
                    //                console.log("plLists.addItem()");
                    plLists.addItem();
                }
            }

            ToolBtn {
                id: btn_delPl
                Layout.fillHeight: true
                Layout.minimumWidth: _toolbar_playlists.height
                Layout.maximumWidth: _toolbar_playlists.height
                btn_height: 30
                btn_width: 40
                btn_text: qsTr("")
                hintText: qsTr("Remove current playlist")
                ico_path: "qrc:/Qml/Ico/file-remove.png"
                onClicked: {
                    //                console.log("plLists.removeCurrentItem(): " + curIndex);
                    plLists.removeItem(curIndex);
                }
            }
            ToolBtn {
                id: btn_moveUpPl
                Layout.fillHeight: true
                Layout.minimumWidth: _toolbar_playlists.height
                Layout.maximumWidth: _toolbar_playlists.height
                btn_height: 30
                btn_width: 40
                btn_text: qsTr("")
                ico_path: "../Ico/move_up.png"
                hintText: qsTr("Move Up current playlist")
                onClicked: {
                    //                console.log("plLists.moveUp(): " + curIndex);
                    plLists.move(curIndex, "up");
                }
            }
            ToolBtn {
                id: btn_moveDownPl
                Layout.fillHeight: true
                Layout.minimumWidth: _toolbar_playlists.height
                Layout.maximumWidth: _toolbar_playlists.height
                btn_height: 30
                btn_width: 40
                btn_text: qsTr("")
                hintText: qsTr("Move Down current playlist")
                ico_path: "../Ico/move_down.png"
                onClicked: {
                    //                console.log("plLists.moveDown(): " + curIndex);
                    plLists.move(curIndex, "down");
                }
            }
            Rectangle {
                id: fillerRect
                Layout.fillWidth: true
                Layout.fillHeight: true
                opacity: 0
            }

            //        ToolButton {
            //            id: btn_refreshPl
            //            highlighted: true
            //            Layout.maximumHeight: 30
            //            Layout.maximumWidth: 40
            //            text: qsTr("Refr.")
            //        }
        }
    }
}
