import QtQuick
import QtQuick.Layouts 1.12
import QtQuick.Controls
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12
import Qt.labs.platform 1.1
import QtMultimedia 



import "../Elements" as Elements
import "../functions.js" as Funcs
import Playlists 1.0

Item {
    property int curMedia : 0   // 0  - _mediaplayer1; 1 - _mediaplayer2
    property bool isPlaying: false
    property var tracks: Array()
//    property int posM: _mediaplayer.position
//    property int posM2: _mediaplayer2.position
    property int posMLast
    property int posNullCount: 0
    property bool isM1Playing: false
    property bool isM2Playing: false
    property bool isM1Pausing: false
    property bool isM2Pausing: false


    Rectangle {
        id: _rec_media1
        anchors.fill: parent
        color: "black"        

//        RowLayout {
//            anchors.fill: parent


        Video {
            id: _mediaplayer

//            anchors.top: parent.top
//            anchors.bottom: parent.bottom
//            anchors.left: parent.left
//            anchors.right: parent.right
//            width: parent.width / 2

            anchors.fill: parent

//            Layout.fillHeight: true
//            Layout.fillWidth: true
            volume: 0
            loops: MediaPlayer.Infinite

            onPositionChanged: {
//                console.log("pos: " + posM + " dur: " + _mediaplayer.duration);

                if (curMedia == 0) {
                    if (position === 0 && posMLast > 0) {
                        ++posNullCount;
                        if (posNullCount === 3) {
                            posNullCount = 0;
                            _mediaplayer.pause();
                            posMLast = 0;
                            curMedia = 1;
                            _mediaplayer2.play();
                            _mediaplayer.anchors.bottom = parent.top
//                            _mediaplayer2.pause();
                            _mediaplayer.stop();
                        }
                    } else {
                        posMLast = position;
                    }
                }

//                if (_mediaplayer.duration > 0 && curMedia == 0 && posM >= _mediaplayer.duration - 100 && posM <= _mediaplayer.duration) {
//                    console.log("pos: " + posM + " dur: " + _mediaplayer.duration);
//                    _mediaplayer2.play();
//                    _mediaplayer2.pause();
////                    _mediaplayer.anchors.bottom = parent.top
//                    curMedia = 1;
////                    _mediaplayer.pause();
////                    _mediaplayer.stop();
//                }
            }
            onStopped: {
                var curUrl = source;
                console.log("stoped m1");
                tracks.shift();
                if (tracks.length > 1) {
                    source = tracks[1];
                } else
                    source = "";
                _videoPlayer.channelPlayed(curUrl);
            }
            onPlaying: {
                isM1Playing = true;
                isM2Playing = false;
                isM1Pausing = false;
                console.log("playing m1");
            }
            onPaused: {
                isM1Pausing = true;
                isM2Pausing = false;
            }
        }
        Video {
            id: _mediaplayer2

//            anchors.top: parent.top
//            anchors.bottom: parent.bottom
//            anchors.right: parent.right
//            width: parent.width / 2

            anchors.fill: parent

//            Layout.fillHeight: true
//            Layout.fillWidth: true
            volume: 0
            loops: MediaPlayer.Infinite

            onPositionChanged: {
                if (curMedia == 1) {
                    if (position === 0 && posMLast > 0) {
                        ++posNullCount;
                        if (posNullCount === 3) {
                            posNullCount = 0;
                            _mediaplayer2.pause();
                            posMLast = 0;
                            curMedia = 0;
                            _mediaplayer.play();
                            _mediaplayer.anchors.fill = parent;
                            _mediaplayer2.stop();
                        }
                    } else {
                        posMLast = position;
                    }
                }

//                if (_mediaplayer2.duration > 0 && curMedia == 1 && posM2 >= _mediaplayer2.duration - 100 && posM2 <= _mediaplayer2.duration) {
//                    console.log("posM2: " + posM2 + " dur: " + _mediaplayer2.duration);
//                    _mediaplayer.play();
////                    _mediaplayer.anchors.fill = parent;

//                    curMedia = 0;
//                    _mediaplayer2.stop();
//                }
            }
            onStopped: {
                var curUrl = source;
                console.log("stoped m1");
                tracks.shift();
                if (tracks.length > 1) {
                    source = tracks[1];
                } else
                    source = "";

                _videoPlayer.channelPlayed(curUrl);
            }
            onPlaying: {
                isM1Playing = false;
                isM2Playing = true;
                isM2Pausing = false;
                console.log("playing m2");
            }
            onPaused: {
                isM2Pausing = true;
                isM1Pausing = false;
            }
        }
//        }

        Connections {
            target: _videoPlayer
            function onStopPlaying() {
                isPlaying = false;
                tracks.splice();
                _mediaplayer.stop();
                _mediaplayer2.stop();
                curMedia = 0;
                posMLast = 0;
                posNullCount = 0;
            }

            function onPlayUrl(url) {
                tracks.push(url);

                if (!isPlaying) {
                    isPlaying = true;
                    curMedia = 0;
                    posMLast = 0;
                    posNullCount = 0;
                    _mediaplayer.visible = true;
                    _mediaplayer.anchors.fill = _rec_media1;

                    _mediaplayer.source = url;
                    _mediaplayer.play();
                    _mediaplayer2.source = "";
                } else {
                    if (curMedia == 0 && _mediaplayer2.source == "") {
                        _mediaplayer2.source = url;
//                        _mediaplayer2.play();
//                        _mediaplayer2.pause();
                    }
                }
            }
        }
    }
    MouseArea {
        anchors.fill: parent
        onPressed: {            
//            if (isM1Playing) {
//                if (isM1Pausing)
//                    _mediaplayer.play();
//                else
//                    _mediaplayer.pause();
//            }
//            if (isM2Playing) {
//                if (isM2Pausing)
//                    _mediaplayer2.play();
//                else
//                    _mediaplayer2.pause();
//            }


                console.log("play");
                _mediaplayer.source = "http://vqwkxbrk.megogo.xyz/iptv/4A7CXCHXVV2U4Z/157/1650399100000.m3u";
                _mediaplayer.play();

        }        
    }
}
