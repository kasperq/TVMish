import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Universal 2.12

//import "./Elements" as Elements

Rectangle {
    id: plLstMngToolbars
    property int curIndex    

    RowLayout {
        id: rwBtn
        spacing: 1
        layoutDirection: Qt.LeftToRight
        Layout.alignment: Qt.AlignLeft

        ToolBtn {
            id: btn_addPl
            btn_height: 30
            btn_width: 30
            btn_text: qsTr("Pl.")
            ico_path: "qrc:/Qml/Ico/folder-add.ico"
            onClicked: {
                console.log("plLists.addItem()");
                plLists.addItem();
            }
        }
        ToolBtn {
            id: btn_delPl
            btn_height: 30
            btn_width: 40
            btn_text: qsTr("- Pl.")
            ico_path: ""
            onClicked: {
                console.log("plLists.removeCurrentItem(): " + curIndex);
                plLists.removeItem(curIndex);
            }
        }
        ToolBtn {
            id: btn_moveUpPl
            btn_height: 30
            btn_width: 40
            btn_text: qsTr("Up")
            ico_path: ""
            onClicked: {
                console.log("plLists.moveUp(): " + curIndex);
                plLists.move(curIndex, "up");
            }
        }
        ToolBtn {
            id: btn_moveDownPl
            btn_height: 30
            btn_width: 40
            btn_text: qsTr("Down")
            ico_path: ""
            onClicked: {
                console.log("plLists.moveDown(): " + curIndex);
                plLists.move(curIndex, "down");
            }
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
