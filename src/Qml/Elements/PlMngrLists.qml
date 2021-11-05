import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Universal 2.12

//import "Elements" as Elements
import Playlists 1.0
import "../ListView" as LViews

Item {
    id: plMngLists
    anchors.fill: parent
    anchors.leftMargin: 3
    anchors.rightMargin: 3

    PlaylistManagerToolbar {
        id: toolbar
        anchors.top: parent.top        
        height: 30
    }


    LViews.PlaylistsView {
        id: plView
        anchors.fill: parent
        anchors.topMargin: toolbar.height + 1
//        onIndexChanged : toolbar.curIndex = currentIndex
        onCurIndexChanged: toolbar.curIndex = plView.curIndex
    }
}
