import QtQuick
import QtQuick.Layouts 1.12
import QtQuick.Controls
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Universal 2.12


import "../functions.js" as Funcs

TextField {
    property bool is_editable: true
    property string back_color
    property string edit_color
    property string border_color
    property bool is_favorite: false
    property int back_radius: 2


    id: _edit_

    implicitWidth: 10
    implicitHeight: 10
    hoverEnabled: true
//    KeyNavigation.down: bottom
//    KeyNavigation.up: top
    wrapMode: TextEdit.WrapAnywhere
    mouseSelectionMode: TextEdit.SelectCharacters
    selectByMouse: true
    focus: true
    readOnly: !is_editable
    Keys.onPressed: (event) => {
                        if (event.key === Qt.Key_Return
                            || event.key === Qt.Key_Enter
                            || event.key === Qt.Key_Escape
                            || event.key === Qt.Key_Down
                            || event.key === Qt.Key_Up) {
                            _edit_.editingFinished();
                            event.accepted = true;
                        }
                    }

    background: Rectangle {
        id: _back_
        radius: back_radius
        color: back_color
        border.color: border_color
    }
}
