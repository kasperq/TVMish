import ACME.VideoItem 1.0

import QtQuick 2.15
import QtMultimedia

VideoItem {
    id: root
    clip: true
    // anchors.fill: parent
    property alias videoOut : videoOut   

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

    Connections {
        target: _tvContr
        function onStartPlayingChannel(url) {
            console.log("start playing: ", url);
            root.source = url
        }
    }
}
