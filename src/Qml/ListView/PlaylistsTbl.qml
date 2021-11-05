//import QtQml 2.12
import QtQuick 2.12
//import QtQuick.Window 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Qt.labs.qmlmodels 1.0

import Playlists 1.0

TableView {
    id: plTbl
    visible: true
    anchors.fill: parent
    columnSpacing: 1
    rowSpacing: 1
    clip: true

    ScrollBar.vertical: ScrollBar {}       

    model: PlaylistsModel  {
//        list: plLists
//        TableModelColumn { display: "id_playlist" }
        TableModelColumn { naim: "naim" }
//        TableModelColumn { display: "is_current" }
    }

    delegate: Rectangle {
        implicitWidth: 100
        implicitHeight: 50
        Text {
            text: display
        }
    }


}
