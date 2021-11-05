import QtQuick 2.0

Component {
    id: highlight
    Rectangle {
        width: parent ? parent.width : 0
        height: parent ? parent.height : 0
        border.color: "steelblue"
        border.width: 2
        color: "transparent"
        radius: 0.3
//        y: parent.left
//        x: parent.right
        Behavior on y {
            SpringAnimation {
                spring: 3
                damping: 0.2
            }
        }
    }

}
