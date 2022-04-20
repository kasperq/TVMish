import QtQuick
import QtQuick.Layouts 1.12
import QtQuick.Controls
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12

import "../Elements" as Elements
import "../functions.js" as Funcs
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

    property string color_current : "dimgray"
    property string color_hoverAndEnabled : "gray"
    property string color_hoverAndDisabled : "dimgray"
    property string color_notHoverAndNotCurInd : "dimgray"
    property string color_notHoverAndCurInd : "#3f3f40"

    property string newFilePath

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
                console.log("okClicked");
                plFiles.appendNewItem();
                enableBtns(false);
            }
            onLocalFileSelected: {
                console.log("onLocalFileselect: " + _listView_files.currentIndex);
                plFiles.addItemFromLocalFile(_listView_files.currentIndex, _fileLocation.fileUrl);
            }
            onBuferUrlSelected: {
                console.log("onBuferUrlSelect: " + _listView_files.currentIndex);
                plFiles.addItemFromBuffer(_listView_files.currentIndex, _fileLocation.fileUrl);
            }
            onUrlFileSelected: {
                console.log("onUrlFileSelected: " + _listView_files.currentIndex);
                plFiles.addItemFromUrl(_listView_files.currentIndex, _fileLocation.fileUrl);
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
            id: _dlg_delete
            width: 350
            height: 80
            anchors.centerIn: Overlay.overlay
            titleText: qsTr("Warning");
            questionText: qsTr("Are you sure you want to delete current playlist file?")
            okBtnText: qsTr("Yes")
            cancelBtnText: qsTr("No")
            onOkClicked: {
                plFiles.removeCurrentItem(_listView_files.currentIndex);
                plFiles.scroll(_listView_files.currentIndex);
            }
        }
        Elements.SureDialog {
            id: dlg_addChannels
            width: 350
            height: 80
            anchors.centerIn: Overlay.overlay
            titleText: qsTr("Warning");
            questionText: qsTr("Are you sure you want to update channels?")
            okBtnText: qsTr("Yes")
            cancelBtnText: qsTr("No")
            onOkClicked: {
                enableBtns(false);
                channels.update(newFilePath);
            }
        }

        RowLayout {
            id: _rlay_toobar
            anchors.fill: parent
            spacing: 1
            opacity: 1
            z: 10
            Elements.ToolBtn {
                id: _btn_selectAllFiles
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
                onClicked: {
                    _listView_files.enabled = !_btn_selectAllFiles.checked;
                    if (_btn_selectAllFiles.checked)
                        plFiles.open(_btn_selectAllFiles.checked, 0);
                    else
                        plFiles.open(_btn_selectAllFiles.checked, plFiles.idFile);
                }
            }
            Elements.ToolBtn {
                id: _btn_add
                z: 10
                opacity: 1
                Layout.fillHeight: true
                Layout.minimumWidth: files_toolbar.height
                Layout.maximumWidth: files_toolbar.height
                btn_height: files_toolbar.height
                enabled: !_btn_selectAllFiles.checked
                btn_width: 20
                btn_text: qsTr("")
                hintText: qsTr("Add playlist file")
                ico_path: "../Ico/add.png"
                onClicked: {
                    if (_btn_selectAllFiles.checked) {
                        _btn_selectAllFiles.checked = false;
                    }
                    _fileLocation.file_path = plFiles.getClipboardString();
                    _fileLocation.open();
                }
            }
            Elements.ToolBtn {
                id: _btn_delete
                z: 9
                opacity: 1
                Layout.fillHeight: true
                Layout.minimumWidth: files_toolbar.height
                Layout.maximumWidth: files_toolbar.height
                enabled: !_btn_selectAllFiles.checked
                btn_height: files_toolbar.height
                btn_width: 20
                btn_text: qsTr("")
                ico_path: "../Ico/delete.png"
                hintText: qsTr("Remove playlist file with channels")
                onClicked: {
                    if (_listView_files.currentIndex == -1) {
                        dlg_error.errorText = qsTr("Choose file!");
                        dlg_error.open();
                    } else {
                        if (_btn_selectAllFiles.checked) {
                            _btn_selectAllFiles.checked = false;
                        }
                        _dlg_delete.focus = true;
                        _dlg_delete.open();                        
                    }
                }
            }
            Elements.ToolBtn {
                id: _btn_refresh
                z: 9
                opacity: 1
                Layout.fillHeight: true
                Layout.minimumWidth: files_toolbar.height
                Layout.maximumWidth: files_toolbar.height
                btn_height: files_toolbar.height
                enabled: !_btn_selectAllFiles.checked
                btn_width: 40
                btn_text: qsTr("")
                hintText: qsTr("Refresh current playlist file")
                ico_path: "../Ico/refresh.png"
                onClicked: {
                    if (_btn_selectAllFiles.checked) {
                        _btn_selectAllFiles.checked = false;
                    }
                    plFiles.refresh(_listView_files.currentIndex);
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
        id: _listView_files
        clip: true
        Layout.fillWidth: true
        Layout.fillHeight: true
        keyNavigationEnabled: true
        headerPositioning: ListView.OverlayHeader
        flickableDirection: Flickable.AutoFlickDirection
        ScrollBar.vertical: ScrollBar {}

        onCurrentIndexChanged: {
            indexChanged(_listView_files.currentIndex);
            _listView_files.model.list = plFiles;
            plFiles.scroll(_listView_files.currentIndex);
        }

        model: PlFilesModel  {
            list: plFiles            
        }

        header: Rectangle {
            id: head
            height: 20
            width: _listView_files.width
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
            anchors.topMargin: 2
            anchors.leftMargin: 3
            z: 1
            clip: false           

            RowLayout {
                Layout.fillWidth: true
                Layout.minimumHeight: 30
                Layout.maximumHeight: 30
                spacing: 1
                focus: true

                Elements.MyTextEdit {
                    id: _lbl_cursor
                    text: _listView_files.currentIndex === index ? ">" : "  "
                    is_editable: false
                    back_color: Funcs.setColor(_lbl_cursor, index, _listView_files.currentIndex)
                    border_color: Funcs.setBorderColor(_lbl_cursor.activeFocus);
                    onPressed: _listView_files.currentIndex = index
                    edit_color: "powderblue"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 16
                    font.bold: true

                    Layout.fillHeight: true
                    Layout.minimumWidth: width_cursor
                    Layout.maximumWidth: width_cursor
                    Layout.topMargin: 1
                }
                Elements.MyTextEdit {
                    id: _edit_naim
                    text: model.file_name
                    placeholderText: qsTr("Enter file name")
                    is_editable: true
                    back_color: Funcs.setColor(_edit_naim, index, _listView_files.currentIndex)
                    border_color: Funcs.setBorderColor(_edit_naim.activeFocus);
                    onEditingFinished: { _edit_naim.focus = false; model.file_name = _edit_naim.text; }
                    onPressed: _listView_files.currentIndex = index

                    Layout.minimumWidth: width_fileName
                    Layout.maximumWidth: width_fileName
                    Layout.fillHeight: true
                    Layout.topMargin: 1
                }
                Elements.MyTextEdit {
                    id: _edit_url
                    text: model.file_path
                    placeholderText: qsTr("Enter url to file playlist")
                    is_editable: true
                    font.pixelSize: 10
                    font.bold: false
                    padding: 1
                    back_color: Funcs.setColor(_edit_url, index, _listView_files.currentIndex)
                    border_color: Funcs.setBorderColor(_edit_naim.activeFocus);
                    onEditingFinished: { _edit_url.focus = false; model.file_path = _edit_url.text; }
                    onPressed: _listView_files.currentIndex = index

                    Layout.fillWidth: true
                    Layout.topMargin: 1
                    Layout.fillHeight: true
                }
                Elements.MyCheckBox {
                    id: _cb_isAvailable
                    Layout.fillHeight: true
                    Layout.minimumWidth: width_isAvailable
                    Layout.maximumWidth: width_isAvailable
                    cb_height: width_isAvailable
                    cb_width: width_isAvailable
                    Layout.topMargin: 1
                    checked: model.is_available
                    index: index
                    currentIndex: _listView_files.currentIndex

                    onClicked: {
                        model.is_available = checked;
                        _listView_files.currentIndex = index;
                    }
                }

                MouseArea {
                    hoverEnabled: true
                    Layout.fillWidth: true
                    onClicked: {
                        _listView_files.currentIndex = index
                    }
                }
            }
        }
        Connections {
            target: plFiles
            function onSelectItem(index) {
//                console.log("PlFilesView: onSelectItem");
                if (_listView_files.currentIndex !== index)
                    _listView_files.currentIndex = index;
            }
            function onItemChanged(index) {
//                console.log("PlFilesView: item changed");
                _listView_files.model.list = plFiles;
            }
            function onRowCountChanged(rows) {
//                console.log("PlFilesView: row count changed: " + rows);
                _listView_files.model.list = plFiles;
                rowsNum = plFiles.rowCount;                
                if (_btn_selectAllFiles.checked)
                    _btn_selectAllFiles.clicked();
            }
            function onErrorEmited(errorMsg) {
                console.log(errorMsg);
                dlg_error.errorText = errorMsg;                
                dlg_error.open();
            }
            function onListChanged() {
//                console.log("PlFilesView: list changed");
                _listView_files.model.list = plFiles;
                rowsNum = plFiles.rowCount;
            }
            function onFileFullyAdded(filePath) {
                console.log("file fully added: " + filePath);

                enableBtns(true);

                newFilePath = filePath;
                dlg_addChannels.focus = true;
                dlg_addChannels.open();
            }
        }
        Connections {
            target: channels
            function onChannelsAdded(result) {
                console.log("channels added: " + result);
                enableBtns(true);
            }
        }
    }
    function enableBtns(isEnabled) {
        _btn_add.enabled = isEnabled;
        _btn_delete.enabled = isEnabled;
        _btn_refresh.enabled = isEnabled;
    }

}

