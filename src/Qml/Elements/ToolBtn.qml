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

    signal clicked

    highlighted: true
    Layout.maximumHeight: btn_height
    Layout.minimumHeight: btn_height
    Layout.maximumWidth: ico_path == "" ? btn_width : 40 + btn_width
    text: btn_text
    hoverEnabled: true
    state: "NORMAL"

    contentItem: RowLayout {
        anchors.fill: parent
        spacing: 1
        Image {
            id: backgroundImage
            Layout.preferredHeight: btn_.btn_height - 5
            Layout.maximumHeight: btn_.btn_height - 10
            Layout.maximumWidth: ico_path == "" ? 0 : 30
            fillMode: Image.PreserveAspectFit
            source: ico_path
            visible: ico_path == "" ? false : true
        }
        Text {
            Layout.fillWidth: true
            Layout.fillHeight: true
            wrapMode: Text.Wrap
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
    }

    background: Rectangle {
        id: btn_back
        anchors.fill: parent
        opacity: enabled ? 1 : 0.3
        radius: 2
    }
    MouseArea {
        id: mouseRegion
        anchors.fill: parent        

        onClicked: btn_.clicked()

        onPressed: {
            if (btn_.checkable && !btn_.checked) {
                btn_.checked = true;
            } else {
                if (btn_.checkable && btn_.checked)
                    btn_.checked = false;
            }
            btn_.state = "PRESSED";
        }
    }
    function setColor(control) {
        if (control.down) {
            btn_back.color = "lightgray";
        } else {
            if (control.hovered) {
                if (control.checked) {
                    btn_back.color = "#424949";
                } else {
                    btn_back.color = "dimgray";
                }
            } else {
                if (control.checked) {
                    btn_back.color = "darkgray";
                } else {
                    btn_back.color = "dimgray";
                }
            }
        }
    }

    function getColor(control) {
        if (control.down) {
            return "lightgray";
        } else {
            if (control.hovered) {
                if (control.pressed) {
                    return "lightgray";
                } else {
                    return "gray";
                }
            } else {
                if (control.checked) {
                    return "#424949";
                } else {
                    return "dimgray";
                }
            }
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
        }
    ]

    ToolTip {
        id: hint
        parent: btn_
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
