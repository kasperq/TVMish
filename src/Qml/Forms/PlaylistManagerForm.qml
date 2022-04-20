import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12

import "../Elements" as Elements
import "../ListView" as LViews
Page {
    id: page_plMngr

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: Elements.PlMngrLists {}
    }
}
