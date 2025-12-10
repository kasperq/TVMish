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

Window {
    id: _form_player
    flags: Qt.SubWindow | Qt.ForeignWindow /*| Qt.FramelessWindowHint*/
//    flags: Qt.Window
//    visibility: "FullScreen"/* "Windowed"*/

    property int curMedia : 0   // 0  - _mediaplayer1; 1 - _mediaplayer2
    property bool isPlaying: false
    property bool isSeeking: false
    property int seekingPosition: 0
    property var tracks: Array()
    property bool isM1Playing: false
    property bool isM2Playing: false
    property string curTime: new Date().toLocaleString(locale, "ddd hh:mm:ss dd.MM.yyyy")
    property bool isMute: _toolbar_play.isMute
    property bool isEntered: false
    property bool isPaused: _toolbar_play.isPaused
    property real volSize: _toolbar_play.volSize
    property real volSizeOld
    property bool fullScreen: _toolbar_play.isFullScreen

    onClosing: console.log("close")
    onActiveChanged: { /*console.log("show: " + active + " source1: " + _player1.source);
        console.log("source2: " + _player2.source);*/
    }
    onIsPlayingChanged: _toolbar_play.isPlaying = isPlaying
    onIsEnteredChanged: {
        if (isEntered)
            _timer_entered.start();
    }

    Timer {
        id: _timer_entered
        interval: 2000
        repeat: false
        onTriggered: {
            isEntered = false;
        }

    }

    Rectangle {
        id: _rec_media1
        anchors.fill: parent
        color: "black"
        focus: true        

        VideoOutput {
            id: _videoOut1
            anchors.fill: parent
        }
        VideoOutput {
            id: _videoOut2
            anchors.fill: parent
        }

        MediaPlayer {
            id: _player1
            videoOutput: _videoOut1
            audioOutput: AudioOutput {
                id: _audioOutput1
                volume: volSize
                muted: isMute
                onVolumeChanged: {
                    _popup_.btn_text = volSize * 100;

                    if (volSizeOld >= volSize * 100)
                        _popup_.ico_path = "qrc:/Qml/Ico/volume_down.png";
                    else
                        _popup_.ico_path = "qrc:/Qml/Ico/volume_up.png";
                    volSizeOld = volSize * 100;
                    _popup_.open();
                }
            }
            onMediaStatusChanged: {
                if (mediaStatus == 6) {
                    if (_player2.source == "") {
                        pause();
                        isPlaying = false;
                    } else {
                        curMedia = 1;
                        isM1Playing = false;
                        isM2Playing = true;
                        _videoOut2.visible = true;
                        _player2.audioOutput = _audioOutput1;
                        _player2.play();
                        pause();

                        stop();
                    }
                }
                if (mediaStatus == 5 && isM2Playing)
                    pause();
                if (isSeeking && mediaStatus == 5) {
                    _player1.setPosition(seekingPosition);
                    isSeeking = false;
                    seekingPosition = 0;
                    _player1.play();
                }
            }

            onPlaybackStateChanged: {
                if (playbackState == MediaPlayer.StoppedState && !isM1Playing && isPlaying) {
                    var curUrl = source;
//                    tracks.shift();
                    tracks.splice(0, 1);
                    if (tracks.length > 1) {
                        source = tracks[1];
                        play();
                    } else
                        source = "";
                    _videoPlayer.channelPlayed(curUrl);
                }
            }
        }

        MediaPlayer {
            id: _player2
            videoOutput: _videoOut2
            audioOutput: AudioOutput {
                id: _audioOutput2
                volume: volSize
                muted: isMute
            }
            onMediaStatusChanged: {
                if (mediaStatus == 6) {
                    if (_player1.source == "") {
                        isPlaying = false;
                        pause();
                    } else {
                        curMedia = 0;
                        isM2Playing = false;
                        isM1Playing = true;
                        _videoOut2.visible = false;
                        _player1.audioOutput = _audioOutput1;
                        _player1.play();
//                        _videoOut2.visible = false;
                        pause();
                        stop();
                    }
                }
                if (mediaStatus == 5 && isM1Playing)
                    pause();
            }
            onPlaybackStateChanged: {
                if (playbackState == MediaPlayer.StoppedState && !isM2Playing && isPlaying) {
                    var curUrl = source;
//                    tracks.shift();
                    tracks.splice(0, 1);
                    if (tracks.length > 1) {
                        source = tracks[1];
                        play();
                    } else
                        source = "";
                    _videoPlayer.channelPlayed(curUrl);
                }
            }
        }
        Elements.PlayerControl {
            id: _toolbar_play
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height / 8
            visible: _form_player.isEntered
            // _mplayer1: _player1
            // _mplayer2: _player2
            onIsFullScreenChanged: {
                if (!isSeeking) {
                    _videoPlayer.pauseDownloadLinks(true);
                    if (isM1Playing)
                        _player1.pause();
                    if (isM2Playing)
                        _player2.pause();

                    var playingUrl;
                    var position;
                    if (isM1Playing) {
                        playingUrl = _player1.source;
                        position = _player1.position;
                    }
                    if (isM2Playing) {
                        playingUrl = _player2.source;
                        position = _player2.position;
                    }

                    _videoPlayer.setFullScreen(false);
                    Funcs.setScreenSize(_toolbar_play.isFullScreen, playingUrl, tracks, position);
                    close();
                }
            }
        }

        Keys.onPressed: (event)=> {
                            if (event.key === Qt.Key_Escape) {
                                _toolbar_play.isFullScreen = !_toolbar_play.isFullScreen
                                event.accepted = true;
                            }
                        }

        MouseArea {
            id: _ma_m1
            anchors.fill: parent
            hoverEnabled: true
            propagateComposedEvents: true
            onMouseXChanged: isEntered = true
            onEntered: { isEntered = true; }
            onExited: { (_toolbar_play.isEntered ? isEntered = true : isEntered = false); }
            onDoubleClicked: {
                _toolbar_play.isFullScreen = !_toolbar_play.isFullScreen;
            }

            WheelHandler {
                id: _wheel_m1
                onWheel: (wheel)=> {
                             volSizeOld = _toolbar_play.volSize * 100;
                             _toolbar_play.sliderValue = volSizeOld + (wheel.angleDelta.y / 120)
                }
            }

        }


        Elements.PopupMessage {
            id: _popup_
            anchors.centerIn: parent
        }


        Connections {
            target: _videoPlayer
            function onStopPlayingPlayer() {
                isPlaying = false;
                isM1Playing = false;
                isM2Playing = false;
                _player1.stop();
                _player2.stop();
                _player1.source = "";
                _player2.source = "";
                _videoOut2.visible = false;
                tracks.splice(0, tracks.length);
                curMedia = 0;
            }

            function onPlayUrlInPlayer(url) {
                console.log("BIG: onPlayUrl");
                tracks.push(url);

                if (!isPlaying) {
                    isPlaying = true;
                    curMedia = 0;

                    _player1.source = url;
                    isM1Playing = true;
                    isM2Playing = false;
                    _videoOut2.visible = false;
                    _player1.play();
                    _player2.source = "";
                } else {
                    if (curMedia == 0) {
                        _videoOut2.visible = false;
                        isM1Playing = true;
                        isM2Playing = false;
                        if (_player1.source == "") {
                            _player1.source = url;
                            _player1.play();
                        } else {
                            if (_player2.source == "") {
                                _player2.source = url;
                                _player2.play();    // will be paused after showing picture
                            }
                        }
                    }
                    if (curMedia == 1) {
                        isM1Playing = false;
                        isM2Playing = true;
                        _videoOut2.visible = true;
                        if (_player2.source == "") {
                            _player2.source = url;
                            _player2.play();
                        } else {
                            if (_player1.source == "") {
                                _player1.source = url;
                                _player1.play();    // will be paused after showing picture
                            }
                        }
                    }
                }
            }
        }
        Connections {
            target: _tvContr
            function onPlayFullScreenPlayer(playingUrl, tracks, position) {
                isM1Playing = true;
                isM2Playing = true;
                _player1.stop();
                _player2.stop();
                _player2.source = "";
                _videoOut2.visible = false;
                isM1Playing = true;
                isM2Playing = false;
                isPlaying = true;
                isSeeking = true;
                seekingPosition = position;
                _toolbar_play.isFullScreen = true;

                if (_form_player.tracks.length > 0)
                    _form_player.tracks.splice(0, _form_player.tracks.length);
                curMedia = 0;


                _player1.source = playingUrl;
                _form_player.tracks = tracks;

                if (tracks.length > 1)
                    _player2.source = tracks[1];

                _player1.play();
                _player2.play();
                _videoPlayer.pauseDownloadLinks(false);
            }
        }
    }


//    MouseArea {
//        id: _ma_playerFS
//        anchors.fill: parent
//        hoverEnabled: true
//        onDoubleClicked: {
//            console.log("window: double click");
//            _tvContr.closeFullScreenPlayer();
//            close();
//        }
//    }
}
