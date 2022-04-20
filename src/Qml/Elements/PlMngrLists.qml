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
    RowLayout {
//        anchors.fill: parent
        Layout.fillHeight: true
        Layout.fillWidth: true
        spacing: 1
        z: -1

        ColumnLayout {
            Layout.fillHeight: true
//            Layout.fillWidth: true
            Layout.minimumWidth: plMngLists.width * 0.4
            spacing: 1
            z: -1

            PlaylistManagerToolbar {
                id: toolbar
                Layout.minimumHeight: 30
                Layout.maximumHeight: 30
                Layout.fillWidth: true
                toolbar_height: 30
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
        LViews.ChannelsView {
            id: _view_channels
            Layout.minimumWidth: plMngLists.width * 0.6
            Layout.minimumHeight: plMngLists.height
        }
    }
}
