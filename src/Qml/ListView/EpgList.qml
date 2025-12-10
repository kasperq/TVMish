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
    id: _frm_epgs
    spacing: 1

    signal indexChanged(int currentIndex)

    property int width_isAvailable : 30
    property int width_naim : 200
    property int width_url : 100
    property int width_dateTimeUpdate : 30
    property int width_dateTimeEnd : 30
    property int width_cursor : 12

    property bool isCurrent: false
    property int rowsNum: _listView_epgs.count
    property int current_index: _listView_epgs.currentIndex

    Rectangle {
        id: _toolbar_epgs
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
                id: _btn_add
                z: 9
                opacity: 1
                Layout.fillHeight: true
                Layout.minimumWidth: _toolbar_epgs.height
                Layout.maximumWidth: _toolbar_epgs.height
                btn_height: _toolbar_epgs.height
                btn_width: 100
                btn_text: qsTr("")
                checkable: false
                ico_path: "../Ico/add.png"
                showHint: true
                hintText: qsTr("Add EPG source")
                onClicked: epgs.appendNewItem();
            }
            Elements.ToolBtn {
                id: _btn_del
                z: 9
                opacity: 1
                Layout.fillHeight: true
                Layout.minimumWidth: _toolbar_epgs.height
                Layout.maximumWidth: _toolbar_epgs.height
                btn_height: _toolbar_epgs.height
                btn_width: 100
                btn_text: qsTr("")
                checkable: false
                ico_path: "../Ico/delete.png"
                showHint: true
                hintText: qsTr("Delete current EPG source")
                onClicked: epgs.deleteEpg(current_index, epgs.currentIdEpg);
            }
            Elements.ToolBtn {
                id: _btn_update
                z: 9
                opacity: 1
                Layout.fillHeight: true
                Layout.minimumWidth: _toolbar_epgs.height
                Layout.maximumWidth: _toolbar_epgs.height
                btn_height: _toolbar_epgs.height
                btn_width: 100
                btn_text: qsTr("")
                checkable: false
                ico_path: "../Ico/refresh.png"
                showHint: true
                hintText: qsTr("Update current EPG source")
                onClicked: epgs.update(current_index, epgs.currentUrl);
            }

            Rectangle {
                Layout.fillWidth: true
                opacity: 0
            }
        }
    }

    ListView {
        id: _listView_epgs
        clip: true
        Layout.fillWidth: true
        Layout.fillHeight: true
        keyNavigationEnabled: true
        headerPositioning: ListView.OverlayHeader
        flickableDirection: Flickable.AutoFlickDirection
        ScrollBar.vertical: ScrollBar {}

        onCurrentIndexChanged: {
            indexChanged(_listView_epgs.currentIndex);
            _listView_epgs.model.list = epgs;
            epgs.scroll(_listView_epgs.currentIndex);
        }

        model: EpgsModel  {
            list: epgs
        }

        delegate: RowLayout {
            width: ListView.view.width
            height: 90
            spacing: 1
            focus: true

            Elements.MyTextEdit {
                id: _lbl_cursor
                text: _listView_epgs.currentIndex === index ? ">" : "  "
                is_editable: false
                back_color: Funcs.setColor(_lbl_cursor, index, _listView_epgs.currentIndex)
                border_color: Funcs.setBorderColor(_lbl_cursor.activeFocus);
                onPressed: _listView_epgs.currentIndex = index
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
            ColumnLayout {
                id: _edit_logoNaim
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.topMargin: 1
                spacing: 1

                Elements.MyTextEdit {
                    id: _edit_naim
                    text: model.naim
                    placeholderText: qsTr("Enter EPG name")
                    is_editable: true
                    back_color: Funcs.setColor(_edit_naim, index, _listView_epgs.currentIndex)
                    border_color: Funcs.setBorderColor(_edit_naim.activeFocus);
                    onEditingFinished: { _edit_naim.focus = false; model.naim = _edit_naim.text; }
                    onPressed: _listView_epgs.currentIndex = index

                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
                Elements.MyTextEdit {
                    id: _edit_url
                    text: model.url
                    placeholderText: qsTr("Enter EPG URL")
                    is_editable: true
                    font.pixelSize: 10
                    font.bold: false
                    padding: 1
                    autoScroll: true
                    back_color: Funcs.setColor(_edit_url, index, _listView_epgs.currentIndex)
                    border_color: Funcs.setBorderColor(_edit_naim.activeFocus);
                    onEditingFinished: { _edit_url.focus = false; model.url = _edit_url.text; }
                    onPressed: _listView_epgs.currentIndex = index

                    Layout.fillWidth: true
                    Layout.topMargin: 1
                    Layout.fillHeight: true
                }
                RowLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Elements.MyTextEdit {
                        id: _edit_dateBegin
                        text: model.date_time_begin.toLocaleString(locale, "dd.MM.yyyy") + " - " + model.date_time_end.toLocaleString(locale, "dd.MM.yyyy")
                        is_editable: false
                        back_color: Funcs.setColor(_edit_dateBegin, index, _listView_epgs.currentIndex)
                        border_color: Funcs.setBorderColor(_edit_dateBegin.activeFocus);
                        onPressed: _listView_epgs.currentIndex = index

                        Layout.maximumWidth: 150
                        Layout.minimumWidth: 150
                        Layout.fillHeight: true
                    }
                    Rectangle {
                        id: _rec_splitter
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        opacity: 0
                        MouseArea {
                            anchors.fill: parent
                            onPressed: _listView_epgs.currentIndex = index
                        }
                    }
                    Elements.MyTextEdit {
                        id: _edit_dateUpd
                        text: qsTr("Updated: ") + model.date_time_update.toLocaleString(locale, "dd.MM.yyyy")
                        is_editable: false
                        back_color: Funcs.setColor(_edit_dateUpd, index, _listView_epgs.currentIndex)
                        border_color: Funcs.setBorderColor(_edit_dateUpd.activeFocus);
                        onPressed: _listView_epgs.currentIndex = index

                        Layout.maximumWidth: 150
                        Layout.minimumWidth: 150
                        Layout.fillHeight: true
                    }
                }
            }
        }

        Connections {
            target: epgs
            function onSelectItem(index) {
                if (_listView_epgs.currentIndex !== index)
                    _listView_epgs.currentIndex = index;
            }
            function onItemChanged(index) {
                _listView_epgs.model.list = epgs;
            }
            function onRowCountChanged(rows) {
                _listView_epgs.model.list = epgs;
//                rowsNum = epgs.rowCount;
//                console.log("channelsview: rowcountchanged" + rowsNum);
            }
//            function onListChanged() {
//                _listView_epgs.model.list = epgs;
//                rowsNum = epgs.rowCount;
////                console.log("channelsview: listchanged: " + rowsNum);
//            }
        }
    }
}
