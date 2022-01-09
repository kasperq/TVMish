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
    ColumnLayout {
        anchors.fill: parent
        spacing: 1
        z: -1

        PlaylistManagerToolbar {
            id: toolbar
            Layout.preferredHeight: 30
            Layout.minimumHeight: 30
            Layout.fillWidth: true
            z: 0
        }


        LViews.PlaylistsView {
            id: plView
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 200
            //        onIndexChanged : toolbar.curIndex = currentIndex
            onCurIndexChanged: toolbar.curIndex = plView.curIndex
        }
        LViews.PlFilesView {
            id: files
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.minimumHeight: 200
//            Layout.preferredHeight: parent.height / 2

        }
//        Rectangle {
//            id: fillerRect
//            Layout.fillHeight: true
//            Layout.fillWidth: true
//        }
    }
}
