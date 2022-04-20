import QtQuick
import QtQuick.Layouts 1.12
import QtQuick.Controls
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12
import Qt.labs.platform 1.1

import "../Elements" as Elements
import "../functions.js" as Funcs
import Playlists 1.0

RowLayout {
    id: _frm_catCh
    spacing: 1

    property int height_channel: 40
    property int rowsNum: 0

    ListView {
        id: _listView_categories
        Layout.fillHeight: true
        Layout.maximumWidth: 25
        Layout.minimumWidth: 25
        flickableDirection: Flickable.AutoFlickDirection

        model: CategoryModel  {
            list: categories
        }

        delegate: ColumnLayout {
            width: ListView.view.width
            Rectangle {
                Layout.minimumHeight: 60
                Layout.maximumHeight: 60
                Layout.fillWidth: true
                opacity: 1
                z: 10

                Elements.MyTextEdit {
                    id: _lbl_cat
                    text: model.naim
                    is_editable: false
                    autoScroll: true
                    activeFocusOnPress: false
                    back_color: Funcs.setColor(_lbl_cat, index, _listView_categories.currentIndex)
                    border_color: Funcs.setBorderColor(_lbl_cat.activeFocus);
                    onPressed: {
                        _listView_categories.currentIndex = index;
                        channels.filterCategory(model.id_category);
                    }
                    edit_color: "powderblue"
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    back_radius: 0
                    font.pixelSize: _listView_categories.currentIndex === index ? 12 : 10
                    font.bold: _listView_categories.currentIndex === index ? true : false
                    padding: 2

                    width: parent.height
                    height: parent.width
                    y: parent.height
                    transformOrigin: Item.TopLeft
                    rotation: -90                    
                }
            }
        }
    }

    ListView {
        id: _listView_channels
        Layout.fillHeight: true
        Layout.fillWidth: true
        headerPositioning: ListView.OverlayHeader
        flickableDirection: Flickable.AutoFlickDirection

        model: ChannelsModel  {
            list: channels
        }
        header: Elements.FindEdit {
            id: _edit_find
            width: _listView_channels.width
            height: 30
            isBtnBackVisible: false
            z: 20

            visible: true
        }

        delegate: RowLayout {
            id: _edit_logoNaim
            width: ListView.view.width
            spacing: 0
            Layout.minimumHeight: height_channel
            Layout.maximumHeight: height_channel

            TextField {
                id: _lbl_logo
                Layout.fillHeight: true
                Layout.minimumWidth: parent.height
                Layout.maximumWidth: parent.height
                readOnly: true

                onPressed: _listView_channels.currentIndex = index
                background: Rectangle {
                    id: _back_logo
                    radius: 0
                    color: Funcs.setColor(_lbl_logo, index, _listView_channels.currentIndex);
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
                id: _edit_num
                text: index + 1 + "."
                placeholderText: qsTr("")
                is_editable: false
                activeFocusOnPress: false
                back_color: Funcs.setColor(_edit_num, index, _listView_channels.currentIndex)
                border_color: Funcs.setBorderColor(_edit_num.activeFocus);
                onPressed: _listView_channels.currentIndex = index
                leftPadding: 3
                rightPadding: 3
                horizontalAlignment: Text.AlignLeft
                Layout.maximumWidth: index+1 > 99 ? 30 : index+1 > 9 ? 25 : 20
                Layout.minimumWidth: index+1 > 99 ? 30 : index+1 > 9 ? 25 : 20
                Layout.fillHeight: true
                back_radius: 0
            }
            Elements.MyTextEdit {
                id: _edit_naim
                text: model.naim
                placeholderText: qsTr("")
                is_editable: false
                autoScroll: true
                activeFocusOnPress: false
                back_color: Funcs.setColor(_edit_naim , index, _listView_channels.currentIndex);
                border_color: Funcs.setBorderColor(_edit_naim.activeFocus);
                onPressed: {
                    _listView_channels.currentIndex = index;
                    _videoPlayer.playChannel(model.url, true);
                }
                is_favorite: model.is_favorite
                back_radius: 0
                leftPadding: 1
                rightPadding: 3
                horizontalAlignment: Text.AlignLeft

                Layout.fillWidth: true
                Layout.fillHeight: true

                Elements.MyTextEdit {
                    id: _edit_archive
                    text: model.archive_days
                    placeholderText: qsTr("")
                    is_editable: false
                    activeFocusOnPress: false
                    back_color: Funcs.setColor(_edit_archive, index, _listView_channels.currentIndex)
                    border_color: Funcs.setBorderColor(_edit_archive.activeFocus);
                    onPressed: _listView_channels.currentIndex = index
                    padding: 1
                    Layout.margins: 3
                    back_radius: 0
                    visible: model.archive_days > 0 ? true : false

                    anchors.verticalCenter: parent.verticalCenter
                    height: height_channel / 2
                    width: height_channel / 3
                    anchors.right: /*model.is_favorite ? _btn_fav.left : _edit_naim.hovered || _edit_archive.hovered ? */_btn_addFav.left/* : parent.right*/

                }
                Elements.ToolBtn {
                    id: _btn_fav
                    z: 9
                    opacity: 1
                    anchors.right: parent.right
                    anchors.top: parent.top
                    height: height_channel
                    width: height_channel / 2
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
                    height: height_channel
                    width: height_channel /2
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
            }
            function onListChanged() {
                _listView_channels.model.list = channels;
                rowsNum = channels.rowCount;
            }
        }
    }
}
