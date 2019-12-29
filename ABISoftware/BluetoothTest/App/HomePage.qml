import QtQuick 2.0
import QtQuick.Controls 2.2

Page{

    ScrollView {
        id: scrollingArea
        anchors.fill: parent
        clip: false
        contentWidth: window.width
        contentHeight: (window.height - footerRectangle.height -headerRectangle.height)*1
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
