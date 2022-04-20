import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12

Rectangle {
    z: 1

    property string curTime
    property var locale: Qt.locale()
    color: "dimgray"
    opacity: 0.7
    radius: 2


    Label {
        id: lbl_time
        anchors.fill: parent
        text: curTime

        color: "white"
        wrapMode: Text.Wrap
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    Timer {
        interval: 500;
        running: true;
        repeat: true
        onTriggered: curTime = new Date().toLocaleString(locale, "ddd hh:mm:ss dd.MM.yyyy")
    }
}
