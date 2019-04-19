import QtQuick 2.0
import QtQuick.Controls 2.2

Page{

    header:Rectangle{
        id: headerRectangle
        height: headerLabel.height
        anchors.left: parent.left
        anchors.right: parent.right
        color: "#dd248d"
        border.color: "#256fdd"

        ToolButton{
            id: backImgbutton
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            width: scrollingArea.width * .05
            Image{
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/Images/back.svg"
            }

            onClicked: stack.pop()
        }

        ToolButton{
            id: drawImgbutton
            anchors.left: backImgbutton.right
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            width: scrollingArea.width * .05
            Image{
                height: parent.height
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/Images/baseline-menu-24px.svg"
            }

            onClicked: drawer.open()
        }

        Label{
            id: headerLabel
            text:"Encoder App"
            anchors.left: parent.left
            anchors.right: parent.right
            horizontalAlignment: Text.AlignHCenter
            font.pointSize:  30
        }
    }

    ScrollView {
        id: scrollingArea
        anchors.fill: parent
        clip: false
        contentWidth: window.width
        contentHeight: (window.height - footerHeight -headerRectangle.height)*1
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded

        Rectangle{
            id: firstRect
            color: "#ffffff"
            height: scrollingArea.height
            width: scrollingArea.width

            Text{
                anchors.left: firstRect.left
                anchors.right: firstRect.right
                anchors.top: firstRect.top
                anchors.bottom: firstRect.bottom
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text:"Home Page\nClick to open action selection"
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        drawer.open()
                    }
                }
            }
        }
    }
}
