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

        TabBar {
            id: tab
            Layout.fillWidth: true
            Layout.fillHeight: true

            property int tab_width: 100
            width: tab_width

            background: Rectangle {
                anchors.fill: parent
                opacity: 0.3
                color: "dimgray"
                radius: 2
            }

            Elements.TabBtn {
                id: btn_tv
                btn_text: qsTr("TV")
                curId: 0
                btn_width: tab.tab_width - 3
                btn_leftMargin: 10
                ico_path: "qrc:/Qml/Ico/tv.ico"

                anchors.top: parent.top
                anchors.left: parent.left
            }
            Elements.TabBtn {
                id: btn_prefs
                btn_text: qsTr("Prefs.")
                curId: 1
                btn_width: tab.tab_width - 3
                btn_leftMargin: 10
                ico_path: "qrc:/Qml/Ico/equalizer.ico"

                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: curId * (btn_tv.height + 1)
            }
            Elements.TabBtn {
                id: btn_plMngr
                btn_text: qsTr("Pl.mngr.")
                curId: 2
                btn_width: tab.tab_width - 3
                btn_leftMargin: 10
                ico_path: "qrc:/Qml/Ico/list.ico"

                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: curId * (btn_tv.height + 1)
            }
            Elements.TabBtn {
                id: btn_test
                btn_text: qsTr("Schdl.")
                curId: 3
                btn_width: tab.tab_width - 3
                btn_leftMargin: 10
                ico_path: "qrc:/Qml/Ico/calendar.ico"

                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: curId * (btn_tv.height + 1)
            }

            onCurrentIndexChanged: {
                switch (currentIndex)
                {
                case 0:
                    mainContr.openTvMode();
                    loader.source = "qrc:/Qml/Forms/TvForm.qml"
                    break;
                    //            case 1:
                    //                loader.source = "qrc:/Test_2.qml"
                    //                break;
                case 2:
                    mainContr.openPlaylistManager();
                    loader.source = "qrc:/Qml/Forms/PlaylistManagerForm.qml"
                    break;
                    //            case 3:
                    //                loader.source = "qrc:/Test_3.qml"
                    //                break;
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
                            }
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
}
