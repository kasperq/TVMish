import QtQuick
import QtQuick.Layouts 1.12
import QtQuick.Controls
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12

import "../Elements" as Elements
import Playlists 1.0

ColumnLayout {
    id: plFilesForm
    //        anchors.fill: parent
    spacing: 1

    signal indexChanged(int currentIndex)

    property int width_isAvailable : 50
    property int width_fileName : 100
    property int width_cursor : 12
    property bool isCurrent: false
    property int rowsNum: 0

    Rectangle {
        id: files_toolbar
        height: 20
        Layout.fillWidth: true
        radius: 2
        color: "darkgray"
        opacity: 1
        z: 100
        Elements.PlFileLocation {
            id: _fileLocation
            anchors.centerIn: Overlay.overlay            
            onOkClicked: {                
                plFiles.appendNewItem();
//                plFiles.addItemFromLocalFile(plFilesView.currentIndex, _fileLocation.fileUrl);
//                plFiles.setFilePath(plFilesView.currentIndex, _fileLocation.file_path);
            }
            onLocalFileSelected: {                
                plFiles.addItemFromLocalFile(plFilesView.currentIndex, _fileLocation.fileUrl);
            }

            onCancelClicked: {
                console.log("plfilesview: cancel clicked");
            }

        }
        Elements.ErrorDialog {
            id: dlg_error
            width: 350
            height: 80
            anchors.centerIn: Overlay.overlay
            //        onOkClicked: {
            //            plFiles.removeCurrentItem(plFilesView.currentIndex);
            //        }
        }
        Elements.SureDialog {
            id: dlg_delete
            width: 350
            height: 80
            anchors.centerIn: Overlay.overlay
            titleText: qsTr("Warning");
            questionText: qsTr("Are you sure you want to delete current playlist file?")
            okBtnText: qsTr("Yes")
            cancelBtnText: qsTr("No")
            onOkClicked: {
                plFiles.removeCurrentItem(plFilesView.currentIndex);
            }
        }

        RowLayout {
            anchors.fill: parent
            spacing: 1
            opacity: 1
            z: 10
            Elements.ToolBtn {
                id: allChannelsBtn
                z: 9
                opacity: 1
                Layout.fillHeight: true
                btn_height: files_toolbar.height
                btn_width: 100
                btn_text: qsTr("Show all chanels")
                checkable: true
                ico_path: ""
                showHint: true
                hintText: qsTr("Show chanels from all files in current playlist")
//                onClicked: {
//                    if (allChannelsBtn.checked) {
//                        allChannelsBtn.checked = false;
//                    }
//                }
            }
            Elements.ToolBtn {
                id: addBtn
                z: 10
                opacity: 1
                Layout.fillHeight: true
                btn_height: files_toolbar.height
                btn_width: 20
                btn_text: qsTr("+")
                ico_path: ""
                onClicked: {
                    console.log("changed target: " + allChannelsBtn.checked);
                    if (allChannelsBtn.checked) {
                        allChannelsBtn.checked = false;
                    }
                    _fileLocation.open();
//                    plFiles.addItem();
                }
            }
            Elements.ToolBtn {
                id: delBtn
                z: 9
                opacity: 1
                Layout.fillHeight: true
                btn_height: files_toolbar.height
                btn_width: 20
                btn_text: qsTr("-")
                ico_path: ""
                onClicked: {
                    if (allChannelsBtn.checked) {
                        allChannelsBtn.checked = false;
                    }
                    dlg_delete.open();
//                    plFiles.removeCurrentItem(plFilesView.currentIndex);
                }
            }
            Elements.ToolBtn {
                id: refreshBtn
                z: 9
                opacity: 1
                Layout.fillHeight: true
                btn_height: files_toolbar.height
                btn_width: 40
                btn_text: qsTr("refr.")
                ico_path: ""
                onClicked: {
                    if (allChannelsBtn.checked) {
                        allChannelsBtn.checked = false;
                    }
//                    plFiles.update(plFilesView.currentIndex);
                }
            }
            Rectangle {
                Layout.fillWidth: true
                opacity: 0
            }
        }
    }


    ListView {
        id: plFilesView
        clip: true
        Layout.fillWidth: true
//        Layout.minimumHeight: {
//            if (plFiles.rowCount === 0)
//                plFiles.rowCount * 20;
//            else
//                plFiles.rowCount * (lbl_cursor.height + 1) + 50;
//        }
        Layout.fillHeight: true
        keyNavigationEnabled: true
        headerPositioning: ListView.OverlayHeader
        flickableDirection: Flickable.AutoFlickDirection
        ScrollBar.vertical: ScrollBar {}

        onCurrentIndexChanged: {
            console.log("PlFilesView: currentFilesIndex1: " + plFilesView.currentIndex);
            indexChanged(plFilesView.currentIndex);
            plFilesView.model.list = plFiles;
        }
//        Component.onCompleted: {
//            console.log("onCompleted");
////            plFilesView.model.list = plFiles;
//        }

        model: PlFilesModel  {
            list: plFiles            
        }

        header: Rectangle {
            id: head
            height: 20
            width: ListView.view.width
            radius: 2
            border.color: "darkgray"
            color: "gray"
            z: 20
            visible: rowsNum > 0 ? true : false

            RowLayout {
                anchors.fill: parent
                spacing: 0
                Label {
                    Layout.minimumWidth: width_cursor
                    Layout.maximumWidth: width_cursor
                    Layout.fillHeight: true
                }
                Label {
                    Layout.fillHeight: true
                    Layout.minimumWidth: width_fileName
                    Layout.maximumWidth: width_fileName
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr("File Name")
                }
                Label {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.leftMargin: 30
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr("URL")
                }
                Label {
                    Layout.fillHeight: true
                    Layout.minimumWidth: width_isAvailable
                    Layout.maximumWidth: width_isAvailable
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr("Valid")
                }
            }
        }

        delegate: ColumnLayout {
            width: ListView.view.width            

            RowLayout {
                Layout.fillWidth: true
                focus: true

                TextEdit {
                    id: lbl_cursor
                    text: plFilesView.currentIndex == index ? ">" : "  "
                    color: "steelblue"
                    Layout.maximumWidth: 10
                    Layout.maximumHeight: 20
                    readOnly: true
                    wrapMode: Text.WordWrap
                    selectByMouse: true
                    font.pixelSize: 16
                    font.bold: true
                    KeyNavigation.down: bottom
                    KeyNavigation.up: top

                    MouseArea {
                        hoverEnabled: true
                        anchors.fill: parent
                        onClicked: {
                            plFilesView.currentIndex = index
                        }
                    }
                }

                TextField {
                    id: edit_naim
                    Layout.fillWidth: true
                    hoverEnabled: true
                    KeyNavigation.down: bottom
                    KeyNavigation.up: top
                    placeholderText: qsTr("Enter file name")
                    text: model.file_name
                    wrapMode: TextEdit.Wrap
                    mouseSelectionMode: TextEdit.SelectCharacters
                    selectByMouse: true
                    focus: true
                    readOnly: false
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
                        focus = false
                        model.file_name = text
                    }
                    onPressed: {
                        plFilesView.currentIndex = index;
                    }
//                    Component.onCompleted: console.log("plfilesview: " + model.file_name)
                }
                TextField {
                    id: edit_url
                    Layout.fillWidth: true
                    hoverEnabled: true
                    KeyNavigation.down: bottom
                    KeyNavigation.up: top
                    placeholderText: qsTr("Enter url to file playlist")
                    text: model.file_path
                    wrapMode: TextEdit.Wrap
                    mouseSelectionMode: TextEdit.SelectCharacters
                    selectByMouse: true
                    focus: true
                    readOnly: false
                    Keys.onPressed: (event) => {
                                        if (event.key === Qt.Key_Return
                                            || event.key === Qt.Key_Escape
                                            || event.key === Qt.Key_Down
                                            || event.key === Qt.Key_Up) {
                                            edit_url.editingFinished();
                                            event.accepted = true;
                                        }
                                    }

                    onEditingFinished: {
                        focus = false
                        model.file_path = text
                    }
                    onPressed: {
                        plFilesView.currentIndex = index;
                    }
                }
                CheckBox {
                    width: 30
                    height: 30
                    checked: model.is_available

                    onClicked: {
                        model.is_available = checked;
                        plFilesView.currentIndex = index;
                    }
                }

                MouseArea {
                    hoverEnabled: true
                    Layout.fillWidth: true
                    onClicked: {
//                        console.log("mouseclick 2");
                        plFilesView.currentIndex = index
                    }
                }
            }
        }
        Connections {
            target: plFiles
            function onSelectItem(index) {
//                console.log("PlFilesView: onSelectItem");
                if (plFilesView.currentIndex !== index)
                    plFilesView.currentIndex = index;
            }
            function onItemChanged(index) {
//                console.log("PlFilesView: item changed");
                plFilesView.model.list = plFiles;
            }
            function onRowCountChanged(rows) {
//                console.log("PlFilesView: row count changed: " + rows);
                plFilesView.model.list = plFiles;
                rowsNum = plFiles.rowCount;
            }
            function onErrorEmited(errorMsg) {
                console.log(errorMsg);
                dlg_error.errorText = errorMsg;
                dlg_error.open();
            }
            function onListChanged() {
//                console.log("PlFilesView: list changed");
                plFilesView.model.list = plFiles;
                rowsNum = plFiles.rowCount;
            }
        }
    }

}

