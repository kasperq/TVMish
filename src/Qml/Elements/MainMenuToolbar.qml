import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12


TabBar {
    id: tab

    property int tab_width: 100

    background: Rectangle {
        anchors.fill: parent
        opacity: 0.3
        color: "dimgray"
        radius: 2
    }

    TabBtn {
        id: btn_tv
        btn_text: qsTr("TV")
        curId: 0
        btn_height: 30
        btn_leftMargin: 10
        ico_path: "qrc:/Qml/Ico/tv.ico"

        anchors.top: parent.top
        anchors.left: parent.left
        width: implicitWidth
    }
    TabBtn {
        id: btn_prefs
        btn_text: qsTr("Prefs.")
        curId: 1
        btn_height: 30
        btn_leftMargin: 10
        ico_path: "qrc:/Qml/Ico/equalizer.ico"

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: curId * (btn_tv.height + 1)
        width: implicitWidth

    }
    TabBtn {
        id: btn_plMngr
        btn_text: qsTr("Pl.mngr.")
        curId: 2
        btn_height: 30
        btn_leftMargin: 10
        ico_path: "qrc:/Qml/Ico/list.ico"

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: curId * (btn_tv.height + 1)
        width: implicitWidth
    }
    TabBtn {
        id: btn_test
        btn_text: qsTr("Schdl.")
        curId: 3
        btn_height: 30
        btn_leftMargin: 10
        ico_path: "qrc:/Qml/Ico/calendar.ico"

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: curId * (btn_tv.height + 1)
        width: implicitWidth
    }

    onCurrentIndexChanged: {
        switch (currentIndex)
        {
        case 0:
            mainContr.openTvMode();
            loader.source = "qrc:/Qml/Forms/TvForm.qml"
            loader.focus = true;
            _tvContr.getFocus();
            break;
            //                case 1:
            //                    loader.source = "qrc:/Test_2.qml"
            //                    break;
        case 2:
            mainContr.openPlaylistManager();
            loader.source = "qrc:/Qml/Forms/PlaylistManagerForm.qml"
            break;

        case 3:
            mainContr.openTvSchedule();
            loader.source = "qrc:/Qml/Forms/EpgForm.qml"
            break;
            //                case 4:
            //                    loader.source = "qrc:/Test_3.qml"
            //                    break;
        };
    }
    Keys.onPressed: (event) => {
                        if (event.key === Qt.Key_Up) {
                            decrementCurrentIndex();
                            event.accepted = true;
                        }
                        if (event.key === Qt.Key_Down) {
                            incrementCurrentIndex();
                            event.accepted = true;
                        }
                        if (event.key === Qt.Key_Right) {
                            loader.focus = true;
                            event.accepted = true;
                        }
                    }
}


