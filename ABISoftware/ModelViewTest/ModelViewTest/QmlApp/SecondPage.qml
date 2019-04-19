import QtQuick 2.0
import QtQuick.Controls 2.2

Page{
    id: secondPage

    ScrollView {
        anchors.fill: parent
        clip: false
        contentWidth: window.width
        contentHeight: window.height*2
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn
        Rectangle{
            id: firstRect
            color: "#00fa1b"
            height: window.height
            width: window.width
            Rectangle{
                height: textLabel.height
                width: textLabel.width
                anchors.centerIn: parent
                color:"#3548f2"
                Label{
                    id: textLabel
                    text: "Hello everybody"
                    font.pointSize: 25
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            stack.pop()
                        }
                    }
                }
            }
        }

        Rectangle{
            height: window.height
            width: window.width
            anchors.top: firstRect.bottom
            Label{
                anchors.centerIn: parent
                text: "Velocity Based Encoder " + stack.depth
                font.pointSize: 20
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        stack.pop()
                    }
                }
            }
        }
    }
}
