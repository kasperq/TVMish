import QtQuick
import QtQuick.Layouts 1.12
import QtQuick.Controls
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12

//import "../Elements" as Elements
import Playlists 1.0

Item {
    id: plListsView
    visible: true

    property int curIndex: plistsView.currentIndex
    signal indexChanged(int currentIndex)        
    property int width_isCurrent : 50
    property int width_cursor : 12
    property string color_current : "dimgray"
    property string color_hoverAndEnabled : "gray"
    property string color_hoverAndDisabled : "dimgray"
    property string color_notHoverAndNotCurInd : "dimgray"
    property string color_notHoverAndCurInd : "#3f3f40"


    ListView {
        id: plistsView
        clip: true
        anchors.fill: parent        
        highlightFollowsCurrentItem: true
        keyNavigationEnabled: true
        headerPositioning: ListView.OverlayHeader
        flickableDirection: Flickable.AutoFlickDirection
        ScrollBar.vertical: ScrollBar {}


        onCurrentIndexChanged: {
//            console.log("onCurrentIndexChanged");
//            (plistsView.currentIndex);
            plLists.indexChanged(plistsView.currentIndex);
            plistsView.model.list = plLists;

        }   
//        Component.onCompleted: {
//            plistsView.model.list = plLists;
//        }

        model: PlaylistsModel  {
            id: plModel
            list: plLists        }

        header: Rectangle {
            id: head
            height: 20
            width: ListView.view.width
            radius: 2
            border.color: "darkgray"
            color: "gray"
            z: 20

            RowLayout {
                anchors.fill: parent                
                Label {
                    Layout.minimumWidth: width_cursor
                    Layout.maximumWidth: width_cursor
                    Layout.fillHeight: true
                }
                Label {
                    Layout.fillWidth: true
                    Layout.leftMargin: 30
                    Layout.fillHeight: true
                    text: qsTr("naim")
                    color: "white"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }
                Label {
                    id: head_current
                    Layout.fillHeight: true
                    Layout.minimumWidth: width_isCurrent
                    Layout.maximumWidth: width_isCurrent
                    opacity: 1
                    text: qsTr("current")
                    color: "white"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }

        delegate: ColumnLayout {
            id: listDelegate
            width: ListView.view.width
            anchors.topMargin: 2
            anchors.leftMargin: 3
            z: 1
            clip: false

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

            RowLayout {
                focus: true
                Layout.minimumHeight: 30
                Layout.maximumHeight: 30
                Layout.fillWidth: true
                spacing: 1
                // cursor
                TextField {
                    id: lbl_cursor
                    text: plistsView.currentIndex == index ? ">" : "  "
//                    text: model.id_playlist
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
                        onClicked: {
//                            console.log("mouseclick");
                            plistsView.currentIndex = index
                        }
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
//                        console.log("editing finished");
                    }
                    onPressed: {
                        plistsView.currentIndex = index;
                    }
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
                     onClicked: {
//                         console.log(model.is_current);
                         plLists.setIsCurrent(index, checked);
                     }
                }
                MouseArea {
                    hoverEnabled: true
                    Layout.fillWidth: true
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    cursorShape: Qt.ArrowCursor
                    onClicked: {
//                        console.log("mouseclick");
                        plistsView.currentIndex = index
                    }
//                    onHoveredChanged: {
//                        back_cbIsCur.color = setColor(cb_isCurrent, index);
//                        back_naim.color = setColor(edit_naim, index);
//                        back_cur = setColor(lbl_cursor, index);
//                    }
                }
            }
            Connections {
                target: plLists
                function onSelectItem(index) {
//                    console.log("PlaylistsView: onSelectItem");
                    if (plistsView.currentIndex !== index)
                        plistsView.currentIndex = index;
//                    plistsView.model.list = plLists;
                }
                function onItemChanged(index) {
//                    console.log("PlaylistsView: item changed");
                    plistsView.model.list = plLists;
                }
                function onListChanged() {
//                    console.log("PlaylistsView: list changed");
                    plistsView.model.list = plLists;
                }
            }
//        }
//            PlFilesView {
//                id: plFilesForm
//                Layout.fillWidth: true
//                Layout.leftMargin: 30
//                Layout.minimumHeight: 50
////                visible: false
//                visible: plistsView.currentIndex == index ? true : false
//            }

//            states: [
//                State {
//                    name: "CURRENT"
//                    when: plistsView.currentIndex == index
//                    PropertyChanges {
//                        target: filesRect;
//                        visible: true
//                    }
//                },
//                State {
//                    name: "NOT_CURRENT"
//                    when: plistsView.currentIndex != index
//                    PropertyChanges {
//                        target: filesRect;
//                        visible: false
//                    }
//                }
//            ]
        }
    }

}
