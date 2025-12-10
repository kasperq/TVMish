import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12

import "./Forms" as Forms
import "./Elements" as Elements

ApplicationWindow {
    id: mainForm
    width: 1000
    height: 500
    visible: true
    x: 100
    y: 100

    //    Material.theme: Material.Dark
    //    Material.accent: Material.Purple
    Universal.theme: Universal.Dark
    Universal.accent: Universal.Steel

    ColumnLayout {
        id: tabRect
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 100
        spacing: 1

        Elements.Clock {
            id: timeTxt
            Layout.fillWidth: true
            Layout.minimumHeight: 40
            Layout.maximumHeight: 40
        }

        Elements.MainMenuToolbar {
            id: tab
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    Loader {
        id: loader
        anchors.fill: parent
        anchors.leftMargin: tabRect.width
        source: "qrc:/Qml/Forms/TvForm.qml"
    }
    Component.onCompleted: {
        tab.focus = true;
        tab.currentIndex = 0;
    }

    Forms.PlayerForm {
        id: _form_playerFS
        opacity: 1

    }
    Connections {
        target: _tvContr
        function onOpenFullScreenPlayer() {
//            loader.source = "qrc:/Qml/Forms/PlayerForm.qml"
//            loader.focus = true;
            _form_playerFS.opacity = 1;
            _form_playerFS.visibility = "FullScreen";            
//            _form_playerFS.visibility = "Windowed";

        }
//        function onFullScreenClosed() {
//            console.log("onFullScreenClosed()");
//            _form_playerFS.visibility = "Hidden";
//            loader.source = "qrc:/Qml/Forms/TvForm.qml";
//        }
    }

}
