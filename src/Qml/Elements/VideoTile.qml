import ACME.VideoItem 1.0

import QtQuick 2.15
import QtMultimedia

import "../Elements" as Elements
import "../functions.js" as Funcs
import Playlists 1.0

VideoItem {
    id: root
    clip: true
    // anchors.fill: parent
    property alias videoOut : videoOut   
    // property bool isPlaying: false

    onWidthChanged: { videoOut.update(); } // to change videoout size while changing main window size
    onHeightChanged: { videoOut.update(); }

    // Rectangle {
    //     id: cellRim
    //     anchors.fill: parent
    //     color: "transparent"
    //     border.color: "green"
    //     radius: 2
    // }

    VideoOutput {
        id: videoOut
        objectName: "videoOut"
        fillMode: VideoOutput.PreserveAspectFit
        anchors.fill: parent
    }

    // Elements.PlayerControl {
    //     id: _toolbar_play
    //     anchors.bottom: parent.bottom
    //     anchors.left: parent.left
    //     anchors.right: parent.right
    //     height: parent.height / 8
    //     visible: true
    //     // onIsFullScreenChanged: {
    //     //     if (!isSeeking) {
    //     //         _videoPlayer.pauseDownloadLinks(true);
    //     //         if (isM1Playing)
    //     //             _player1.pause();
    //     //         if (isM2Playing)
    //     //             _player2.pause();

    //     //         var playingUrl;
    //     //         var position;
    //     //         if (isM1Playing) {
    //     //             playingUrl = _player1.source;
    //     //             position = _player1.position;
    //     //         }
    //     //         if (isM2Playing) {
    //     //             playingUrl = _player2.source;
    //     //             position = _player2.position;
    //     //         }

    //     //         _videoPlayer.setFullScreen(true);
    //     //         Funcs.setScreenSize(_toolbar_play.isFullScreen, playingUrl, tracks, position);
    //     //     }
    //     // }
    // }

    Connections {
        target: _tvContr
        function onStartPlayingChannel(url) {
            console.log("start playing: ", url);
            root.source = url
        }
    }
}
