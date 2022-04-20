import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12

import "../ListView" as Lists
import "../Elements" as Elements

Page {
    id: page_tv

    RowLayout {
        anchors.fill: parent
        spacing: 1

        Lists.CategoryChannelView {
            id: _listView_catCh
            Layout.fillHeight: true
            Layout.maximumWidth: parent.width / 3
            Layout.minimumWidth: parent.width / 4
        }
        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Elements.PlayerMiniForm {
                id: _miniPlayer
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Lists.ScheduleView {
                id: _listView_schedule
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

}
