import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12

import "./Forms" as Forms

Rectangle {
    id: mainMenuToolbar

    property string curTime
    property var locale: Qt.locale()

    RowLayout {
        id: rwBtn
        spacing: 1
        layoutDirection: Qt.LeftToRight
        Layout.alignment: Qt.AlignLeft

        ToolButton {
            id: btn_tv
            highlighted: true           
            Layout.maximumWidth: 70
            Layout.maximumHeight: 30
            text: qsTr("TV")

            onClicked: {
                console.log("TV");
                stack_main.pop();
                stack_main.push(page_tv);
            }
        }        

        ToolButton {
            id: btn_prefs
            highlighted: true
            Layout.maximumWidth: 70
            Layout.maximumHeight: 30
            text: qsTr("Prefs.")
        }
        ToolButton {
            id: btn_plMngr
            highlighted: true
            Layout.maximumWidth: 70
            Layout.maximumHeight: 30
            text: qsTr("Pl.Mngr.")
            onClicked: {
                console.log("pl.mngr");
                stack_main.pop();
                mainContr.openPlaylistManager();
                stack_main.push(page_plMngr);
            }
        }
        ToolButton {
            id: btn_schdl
            highlighted: true
            Layout.maximumWidth: 70
            Layout.maximumHeight: 30
            text: qsTr("Schdl")
        }
    }
    Timer {
        interval: 500;
        running: true;
        repeat: true
        onTriggered: curTime = new Date().toLocaleString(locale, "ddd hh:mm:ss dd.MM.yyyy")
    }

    Rectangle {
        anchors.centerIn: parent
        Label {
            id: lbl_time
            anchors.fill: parent
            text: curTime
        }
    }

    Forms.TvForm {
        id: page_tv
    }
    Forms.PlaylistManagerForm {
        id: page_plMngr
    }
}


