import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12

import "../ListView" as Lists
import "../Elements" as Elements

Page {
    id: page_tv

    RowLayout {
        anchors.fill: parent
        spacing: 1

        Lists.CategoryChannelView {
            id: _listView_catCh
            Layout.fillHeight: true
            Layout.maximumWidth: parent.width / 4
            Layout.minimumWidth: parent.width / 10
        }
        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Elements.VideoTile {
                id: _tile
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            Elements.PlayerControl {
                id: _toolbar_play
                Layout.maximumHeight: 30
                Layout.preferredHeight: parent.height / 15
                Layout.fillWidth: true

                visible: _tile.isPlaying
                // onIsFullScreenChanged: {
                //     if (!isSeeking) {
                //         _videoPlayer.pauseDownloadLinks(true);
                //         if (isM1Playing)
                //             _player1.pause();
                //         if (isM2Playing)
                //             _player2.pause();

                //         var playingUrl;
                //         var position;
                //         if (isM1Playing) {
                //             playingUrl = _player1.source;
                //             position = _player1.position;
                //         }
                //         if (isM2Playing) {
                //             playingUrl = _player2.source;
                //             position = _player2.position;
                //         }

                //         _videoPlayer.setFullScreen(true);
                //         Funcs.setScreenSize(_toolbar_play.isFullScreen, playingUrl, tracks, position);
                //     }
                // }
            }

            // Elements.PlayerMiniForm {
            //     id: _miniPlayer
            //     Layout.fillWidth: true
            //     Layout.fillHeight: true
            // }

            Lists.ScheduleView {
                id: _listView_schedule
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

}
