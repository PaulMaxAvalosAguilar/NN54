import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1

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
            height: scrollingArea.height
            width: scrollingArea.width


            RoundButton{

                property var started: 0
                anchors.centerIn: parent
                height: window.height/8
                width: height
                text: started? "Stop": "Start"
                font.pointSize: window.height/60
                onClicked: {
                    started? connhandling.sendStop() : connhandling.sendStart()
                    started? started = 0 : started = 1
                }
            }
        }
    }
}
