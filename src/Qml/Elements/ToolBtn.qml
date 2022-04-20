import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Universal 2.12

ToolButton {
    id: btn_

    property string btn_text
//    property int btn_width: 50
//    property int btn_height: 30
    property int btn_width: 0
    property int btn_height: 0

    property int btn_leftMargin: 0
    property string ico_path
    property string hintText
    property bool showHint: false
    property bool isFocused: false

    signal clicked

    highlighted: true

    text: btn_text
    hoverEnabled: true
    state: "NORMAL"
    opacity: enabled ? 1 : 0.5

    contentItem: RowLayout {
        anchors.fill: parent
        spacing: 1
        Rectangle {
            id: fillerRectLeft
            Layout.fillHeight: true
            Layout.fillWidth: true
            opacity: 0
        }
        Image {
            id: backgroundImage

            Layout.fillHeight: true
            Layout.fillWidth: true

            fillMode: Image.PreserveAspectFit
            source: ico_path
            visible: ico_path == "" ? false : true
            verticalAlignment: Image.AlignVCenter
            horizontalAlignment: /*btn_.text === "" ? Image.AlignLeft : */Image.AlignHCenter
        }
        Text {
            Layout.fillWidth: true
            Layout.fillHeight: true
            wrapMode: Text.WrapAnywhere
            text: btn_.text
            font: btn_.font
            opacity: enabled ? 1.0 : 0.3
            color: {
                if (btn_.checked)
                    "yellow";
                else
                    "white";
            }
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: ico_path == "" ? Text.AlignHCenter : Text.AlignLeft
            elide: Text.ElideLeft

            visible: btn_.text === "" ? false : true
        }
        Rectangle {
            id: fillerRect
            Layout.fillHeight: true
            Layout.fillWidth: true
            opacity: 0
        }
    }

    background: Rectangle {
        id: btn_back
        anchors.fill: parent
        opacity: enabled ? 1 : 0.3
        focus: isFocused
        radius: 2
        border.color: btn_back.focus ? "#4682B4" : "dimgray"
        border.width: btn_back.focus ? 2 : 0
        color: "dimgray"
    }
    MouseArea {
        id: _mR
        anchors.fill: parent        

        onClicked: {
            btn_.clicked();
        }

        onPressed: {            
            if (btn_.checkable && !btn_.checked) {
                btn_.checked = true;
            } else {
                if (btn_.checkable && btn_.checked)
                    btn_.checked = false;
            }
            btn_.state = "PRESSED";
        }
        onHoveredChanged: btn_.state = "HOVERED"
    }
    Keys.onPressed: (event) => {
                        if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                            if (isFocused) {
                                btn_.clicked();
                            }
                            event.accepted = true;
                        }
                    }

    states: [
        State {
            name: "HOVERED"
            when: btn_.hovered
            PropertyChanges {
                target: btn_back;
                color: btn_.checked ? "darkgray" : "lightgray"
            }
        },
        State {
            name: "NORMAL"
            when: btn_.activeFocusChanged
            PropertyChanges {
                target: btn_back;                
                color: btn_.checked ? "#424949" : "dimgray"
            }
        },
        State {
            name: "PRESSED"
            PropertyChanges {
                target: btn_back;
                color: btn_.checked ? "darkgray" : "lightgray"
            }
        },
        State {
            name: "FOCUSED"
            PropertyChanges {
                target: btn_back
                border.color: btn_back.focus ? "yellow" : "#242424"
                border.width: btn_back.focus ? 4 : 1

            }
        }

    ]

    ToolTip {
        id: hint
        parent: btn_ != null ? btn_ : null
        visible: btn_.hovered && showHint
        text: hintText
        delay: 500
        timeout: 5000

        contentItem: Text {
            text: hint.text
            font: hint.font
            color: "black"
        }

        background: Rectangle {
            border.color: "black"
            color: "#DEB887"
            radius: 4
        }
    }

}
