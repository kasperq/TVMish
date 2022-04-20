import QtQuick
import QtQuick.Layouts 1.12
import QtQuick.Controls
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12
import Qt.labs.platform 1.1

import "../Elements" as Elements
import "../functions.js" as Funcs
import Playlists 1.0

ColumnLayout {
    id: _frm_channels
    spacing: 1    

    signal indexChanged(int currentIndex)

    property int width_isAvailable : 30
    property int width_naim : 200
    property int width_url : 100
    property int width_archive : 30
    property int width_logo : 30
    property int width_category : 70
    property int width_cursor : 12

    property bool isCurrent: false
    property int rowsNum: 0
    property int current_index: _listView_channels.currentIndex

    Rectangle {
        id: _toolbar_channels
        height: 30
        Layout.fillWidth: true
        radius: 2
        color: "darkgray"
        opacity: 1
        z: 100


        RowLayout {
            id: _rl_btns
            anchors.fill: parent
            Layout.fillWidth: true
            spacing: 1
            opacity: 1
            z: 10
            Elements.ToolBtn {
                id: _btn_moveUp
                z: 9
                opacity: 1
                Layout.fillHeight: true
                Layout.minimumWidth: _toolbar_channels.height
                Layout.maximumWidth: _toolbar_channels.height
                btn_height: _toolbar_channels.height
                btn_width: 100
                btn_text: qsTr("")
                checkable: false
                enabled: channels.isAllFilesSelected
                ico_path: "../Ico/move_up.png"
                showHint: true
                hintText: qsTr("Move Up current channel")
                onClicked: channels.move(_listView_channels.currentIndex, "up");
            }
            Elements.ToolBtn {
                id: _btn_moveDown
                z: 9
                opacity: 1
                Layout.fillHeight: true
                Layout.minimumWidth: _toolbar_channels.height
                Layout.maximumWidth: _toolbar_channels.height
                btn_height: _toolbar_channels.height
                btn_width: 50
                btn_text: qsTr("")
                checkable: false
                enabled: channels.isAllFilesSelected
                ico_path: "../Ico/move_down.png"
                showHint: true
                hintText: qsTr("Move Down current channel")
                onClicked: channels.move(_listView_channels.currentIndex, "down");
            }
            Elements.ToolBtn {
                id: _btn_delCurr
                z: 9
                opacity: 1
                Layout.fillHeight: true
                Layout.minimumWidth: _toolbar_channels.height
                Layout.maximumWidth: _toolbar_channels.height
                btn_height: _toolbar_channels.height
                btn_width: 100
                btn_text: qsTr("")
                checkable: false
                ico_path: "../Ico/delete.png"
                showHint: true
                hintText: qsTr("Delete current channel")
                onClicked: channels.deleteChannel();
            }
            Elements.ToolBtn {
                id: _btn_find
                z: 9
                opacity: 1
                Layout.fillHeight: true
                Layout.minimumWidth: _toolbar_channels.height
                Layout.maximumWidth: _toolbar_channels.height
                btn_text: qsTr("")
                checkable: false
                ico_path: "qrc:/Qml/Ico/find.png"
                showHint: true
                hintText: qsTr("Find channel")
                onClicked: {
                    _rl_btns.enabled = false;
                    _edit_find.visible = true;
                    _edit_find.focus = true;
                }
            }

            Rectangle {
                Layout.fillWidth: true
                opacity: 0
            }
        }
    }
    Elements.FindEdit {
        id: _edit_find
        Layout.fillWidth: true
        Layout.maximumHeight: 30
        Layout.minimumHeight: 30

        visible: false
        onCloseFindEdit: {
//            _rl_btns.visible = true;
            _rl_btns.enabled = true;
            _edit_find.visible = false;
            channels.filter("");
        }
    }

    ListView {
        id: _listView_channels
        clip: true
        Layout.fillWidth: true
        Layout.fillHeight: true
        keyNavigationEnabled: true
        headerPositioning: ListView.OverlayHeader
        flickableDirection: Flickable.AutoFlickDirection
        ScrollBar.vertical: ScrollBar {}

        onCurrentIndexChanged: {
            indexChanged(_listView_channels.currentIndex);
            _listView_channels.model.list = channels;
            channels.scroll(_listView_channels.currentIndex);            
        }

        model: ChannelsModel  {
            list: channels
        }

        header: Rectangle {
            id: head
            height: 20
            width: _listView_channels.width
            radius: 2
            border.color: "darkgray"
            color: "gray"
            z: 20

            RowLayout {
                anchors.fill: parent
                spacing: 0
                Label {
                    Layout.minimumWidth: width_cursor
                    Layout.maximumWidth: width_cursor
                    Layout.fillHeight: true
                    background: Rectangle {
                        color: "gray"
                        border.color: "darkgray"
                        radius: 2
                    }
                }
                Label {
                    Layout.fillHeight: true
                    Layout.minimumWidth: width_naim + width_logo + 1
                    Layout.maximumWidth: width_naim + width_logo + 1
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr("Channel name")
                }
                Label {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr("URL")
                }
                Label {
                    Layout.fillHeight: true
                    Layout.minimumWidth: width_archive
                    Layout.maximumWidth: width_archive
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 9
                    text: qsTr("Archive")
                }
                Label {
                    Layout.fillHeight: true
                    Layout.minimumWidth: width_isAvailable
                    Layout.maximumWidth: width_isAvailable
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr("Valid")
                }
                Label {
                    Layout.fillHeight: true
                    Layout.minimumWidth: width_category
                    Layout.maximumWidth: width_category
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr("Category")
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
                    text: _listView_channels.currentIndex === index ? ">" : "  "
                    is_editable: false
                    back_color: Funcs.setColor(_lbl_cursor, index, _listView_channels.currentIndex)
                    border_color: Funcs.setBorderColor(_lbl_cursor.activeFocus);
                    onPressed: _listView_channels.currentIndex = index
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
                RowLayout {
                    id: _edit_logoNaim
                    Layout.fillHeight: true
                    Layout.topMargin: 1
                    spacing: 1
                    Layout.minimumWidth: width_logo + width_naim + 1
                    Layout.maximumWidth: width_logo + width_naim + 1

                    TextField {
                        id: _lbl_logo
                        Layout.fillHeight: true
                        Layout.minimumWidth: width_logo
                        Layout.maximumWidth: width_logo
                        readOnly: true

                        onPressed: _listView_channels.currentIndex = index
                        background: Rectangle{
                            radius: 2
                            color: Funcs.setColor(_lbl_logo, index, _listView_channels.currentIndex)
                            border.color: Funcs.setBorderColor(_lbl_logo.activeFocus);
                            Image {
                                id: _img_logo
                                anchors.fill: parent
                                fillMode: Image.PreserveAspectFit
                                source: model.logo_path
                                Connections {
                                    target: channels
                                    function onLogoChanged(indexChange, logoPath) {
                                        if (indexChange === index) {
                                            model.logo_path = logoPath;
                                            _img_logo.source = logoPath;
                                        }
                                    }
                                }
                            }
                        }
                        Elements.ToolBtn {
                            id: _btn_addLogo
                            z: 9
                            opacity: 1
                            anchors.right: parent.right
                            anchors.top: parent.top
                            Layout.maximumHeight: parent.height
                            Layout.maximumWidth: parent.width
                            width: 15
                            height: 15
                            btn_text: qsTr("")
                            checkable: false
                            ico_path: "qrc:/Qml/Ico/add.png"
                            showHint: true
                            hintText: qsTr("Add new logo for the channel")
                            visible: _lbl_logo.hovered
                            onClicked: _dlg_file.open();

                            FileDialog {
                                id: _dlg_file
                                title: qsTr("Please choose a image for logo")
                                fileMode: FileDialog.OpenFile
                                onAccepted: {
                                    var file_path = _dlg_file.currentFile;
                                    channels.setLogo(index, file_path);
                                }
                                onRejected: console.log("Canceled")
                            }
                        }
                    }

                    Elements.MyTextEdit {
                        id: _edit_naim
                        text: model.naim
                        placeholderText: qsTr("Enter file name")
                        is_editable: true                        
                        back_color: Funcs.setColor(_edit_naim, index, _listView_channels.currentIndex)
                        border_color: Funcs.setBorderColor(_edit_naim.activeFocus);
                        onEditingFinished: { _edit_naim.focus = false; model.naim = _edit_naim.text; }
                        onPressed: _listView_channels.currentIndex = index
                        is_favorite: model.is_favorite

                        Layout.minimumWidth: width_naim
                        Layout.maximumWidth: width_naim
                        Layout.fillHeight: true

                        Elements.ToolBtn {
                            id: _btn_fav
                            z: 9
                            opacity: 1

                            anchors.right: parent.right
                            anchors.top: parent.top
                            Layout.maximumHeight: parent.height
                            Layout.maximumWidth: parent.width
                            width: 15
                            height: 15

                            btn_text: qsTr("")
                            checkable: false
                            ico_path: "qrc:/Qml/Ico/favorite.png"
                            showHint: true
                            hintText: qsTr("Channel is favorite")
                            visible: model.is_favorite
                            onClicked: channels.setIsFavorite(index, false);
                        }
                        Elements.ToolBtn {
                            id: _btn_addFav
                            z: 9
                            opacity: 1
                            anchors.right: parent.right
                            anchors.top: parent.top
                            Layout.maximumHeight: parent.height
                            Layout.maximumWidth: parent.width
                            width: 15
                            height: 15
                            btn_text: qsTr("")
                            checkable: false
                            ico_path: "qrc:/Qml/Ico/favorite-add.png"
                            showHint: true
                            hintText: qsTr("Add channel to favorites")
                            visible: {
                                if (!model.is_favorite && _edit_naim.hovered)
                                    true;
                                else
                                    false;
                            }
                            onClicked: channels.setIsFavorite(index, true);
                        }
                    }                    
                }
                Elements.MyTextEdit {
                    id: _edit_url
                    text: model.logo_path/*url*/
                    placeholderText: qsTr("Enter channel URL")
                    is_editable: true
                    font.pixelSize: 10
                    font.bold: false
                    padding: 1
                    autoScroll: true
                    back_color: Funcs.setColor(_edit_url, index, _listView_channels.currentIndex)
                    border_color: Funcs.setBorderColor(_edit_naim.activeFocus);
                    onEditingFinished: { _edit_url.focus = false; model.url = _edit_url.text; }
                    onPressed: _listView_channels.currentIndex = index

                    Layout.fillWidth: true
                    Layout.topMargin: 1
                    Layout.fillHeight: true                    
                }
                Elements.MyTextEdit {
                    id: _edit_archive
                    text: model.archive_days
                    placeholderText: qsTr("Enter days in archive")
                    is_editable: true                    
                    back_color: Funcs.setColor(_edit_archive, index, _listView_channels.currentIndex)
                    border_color: Funcs.setBorderColor(_edit_naim.activeFocus);
                    onEditingFinished: { _edit_archive.focus = false; model.archive_days = _edit_archive.text; }
                    onPressed: _listView_channels.currentIndex = index

                    Layout.preferredWidth: width_archive
                    Layout.maximumWidth: width_archive
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
                    currentIndex: _listView_channels.currentIndex

                    onClicked: {
                        model.is_available = checked;
                        _listView_channels.currentIndex = index;
                    }
                }
                Elements.MyTextEdit {
                    id: _edit_category
                    text: model.category_naim
                    placeholderText: qsTr("Enter channel category")
                    is_editable: true
                    padding: 1
                    autoScroll: true
                    back_color: Funcs.setColor(_edit_category, index, _listView_channels.currentIndex)
                    border_color: Funcs.setBorderColor(_edit_naim.activeFocus);
                    onEditingFinished: { _edit_category.focus = false; model.category_naim = _edit_category.text; }
                    onPressed: _listView_channels.currentIndex = index
                    font.pixelSize: 10
                    font.bold: false
                    Layout.preferredWidth: width_category
                    Layout.maximumWidth: width_category
                    Layout.topMargin: 1
                    Layout.fillHeight: true
                }

                MouseArea {
                    hoverEnabled: true
                    Layout.fillWidth: true
                    onClicked: {
//                        console.log("curInd: " + _listView_channels.currentIndex + " index: " + index);
                        _listView_channels.currentIndex = index;

                    }
                }
            }
        }

        Connections {
            target: channels
            function onSelectItem(index) {
                if (_listView_channels.currentIndex !== index)
                    _listView_channels.currentIndex = index;
            }
            function onItemChanged(index) {                
                _listView_channels.model.list = channels;
            }
            function onRowCountChanged(rows) {
                _listView_channels.model.list = channels;
                rowsNum = channels.rowCount;
//                console.log("channelsview: rowcountchanged" + rowsNum);
            }
            function onListChanged() {
                _listView_channels.model.list = channels;
                rowsNum = channels.rowCount;
//                console.log("channelsview: listchanged: " + rowsNum);
            }
            function onIsAllFilesSelectedChanged(isAll) {                
                _btn_moveDown.enabled = isAll;
                _btn_moveUp.enabled = isAll;
            }
        }
    }
}
