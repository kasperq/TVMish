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

Rectangle {
    // required property MediaPlayer _mplayer1
    // required property MediaPlayer _mplayer2
    property bool isPlaying: false
    property bool isMute: false
    property bool isEntered: false
    property bool isPaused: false
    property real volSize: 100.0 /*_slider_volume.value/100*/
    property real sliderValue: 100.0
    property bool isFullScreen: false

    id: _toolbar_play

    z: 1
    opacity: 0.5
    color: "dimgray"

    onIsPlayingChanged: { _btn_fullScreen.enabled = isPlaying; _btn_play.enabled = isPlaying; }

    RowLayout {
        anchors.fill: parent
        id: _rl_toolbar
        RowLayout {
            id: _rl_volume
            Layout.fillHeight: true
            Elements.ToolBtn {
                id: _btn_volume
                z: 0
                opacity: 1
                Layout.fillHeight: true
                Layout.margins: 3
                btn_height: _rl_toolbar.height
                btn_width: _rl_toolbar.height
                btn_text: qsTr("")
                checkable: false
                ico_path: (isMute) ? "qrc:/Qml/Ico/volume_off.png" : "qrc:/Qml/Ico/volume_on.png"
                showHint: true
                hintText: isMute ? qsTr("Volume on") : qsTr("Volume off")
                onClicked: {isMute = !isMute; console.log("mute"); }
                onEntered: _toolbar_play.isEntered = true;
                onExited: _toolbar_play.isEntered = false;
            }
            Slider {
                id: _slider_volume
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignVCenter
                Layout.minimumWidth: _rl_toolbar.height * 2
                Layout.maximumWidth: _rl_toolbar.height * 2
                Layout.margins: 3

                enabled: true
                to: 100.0
                value: (sliderValue >= 0 && sliderValue <= 100) ? sliderValue : value
                stepSize: 1
                onValueChanged: { volSize = _slider_volume.value / 100; }
            }
        }
        // Rectangle {
        //     id: _rect_leftFiller
        //     Layout.fillHeight: true
        //     Layout.fillWidth: true
        //     opacity: 0
        // }
        Elements.ToolBtn {
            id: _btn_play
            z: 0
            opacity: 1
            enabled: true
            Layout.fillHeight: true
            Layout.margins: 3
            btn_height: _rl_toolbar.height
            btn_width: _rl_toolbar.height
            btn_text: qsTr("")
            checkable: false
            ico_path: (isPaused) ? "qrc:/Qml/Ico/pause.png" : "qrc:/Qml/Ico/play.png"
            showHint: true
            hintText: isPaused ? qsTr("Play") : qsTr("Pause")
            onClicked: {                
                // if (isM1Playing) {
                //     if (isPaused)
                //         _mplayer1.play();
                //     else
                //         _mplayer1.pause();
                // }
                // if (isM2Playing) {
                //     if (isPaused)
                //         _mplayer2.play();
                //     else
                //         _mplayer2.pause();
                // }
                isPaused = !isPaused;
            }
            onEntered: _toolbar_play.isEntered = true;
            onExited: _toolbar_play.isEntered = false;
        }
        Rectangle {
            id: _rect_rightFiller
            Layout.fillHeight: true
            Layout.fillWidth: true
            opacity: 0
        }
        Elements.ToolBtn {
            id: _btn_fullScreen
            z: 0
            opacity: 1
            enabled: false
            Layout.fillHeight: true
            Layout.margins: 3
            btn_height: _rl_toolbar.height
            btn_width: _rl_toolbar.height
            btn_text: qsTr("")
            checkable: false
            ico_path: (isFullScreen) ? "qrc:/Qml/Ico/full-screen-exit.png" : "qrc:/Qml/Ico/full-screen.png"
            showHint: _toolbar_play.activeFocus ? true : false
            hintText: isFullScreen ? qsTr("Normal Screen") : qsTr("Full Screen")
            onClicked: {
                isFullScreen = !isFullScreen;
            }
            onEntered: _toolbar_play.isEntered = true;
            onExited: _toolbar_play.isEntered = false;
        }
    }
}
