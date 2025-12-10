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
    property bool isEntered: false;
    property bool containsMouse: isEntered
    property int curIndex: 0
    property bool isHovered: false
//    property bool isFocused: false

    signal entered
    signal exited
    signal clicked
    signal pressed

    id: _edit_

    implicitWidth: 10
    implicitHeight: 10
    hoverEnabled: true
//    KeyNavigation.down: bottom
//    KeyNavigation.up: top
    wrapMode: TextEdit.WrapAnywhere    
    selectByMouse: true    
    mouseSelectionMode: TextEdit.SelectCharacters
//    focus: isFocused
//    focus: true
    readOnly: !is_editable
//    onClicked: { if (is_editable) _edit_.focus = true; _edit_.clicked(); }
//    onPressed: { _edit_.pressed(); }
//    onEntered: { /*console.log("entered"); isEntered = true;*/ _edit_.entered(); }
//    onExited: { /*isEntered = false;*/ _edit_.exited(); }
//    onFocusChanged: console.log("focus: " + focus)
    Keys.onPressed: (event) => {
                        if (event.key === Qt.Key_Return
                            || event.key === Qt.Key_Enter
                            || event.key === Qt.Key_Escape
                            || event.key === Qt.Key_Down
                            || event.key === Qt.Key_Up) {
                            _edit_.deselect();
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
    MouseArea {
        id: _mA
        anchors.fill: parent
        propagateComposedEvents: true
        hoverEnabled: true
        onEntered: { isEntered = true; _edit_.entered(); isHovered = true; }
        onExited: { isEntered = false; _edit_.exited(); isHovered = false; }
        onClicked: { _edit_.clicked(); if (is_editable) _edit_.forceActiveFocus(); }
        onPressed: { _edit_.pressed(); }
    }
    Shortcut {
        sequence: [StandardKey.Paste, "Ctrl+V"]
        onActivated: _edit_.paste();
    }
    Shortcut {
        sequences: [StandardKey.Cut, "Ctrl+X", "Shift+Del"]
        onActivated: _edit_.cut()
    }
    Shortcut {
        sequences: [StandardKey.Copy, "Ctrl+C"]
        onActivated: _edit_.copy()
    }

}
