import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1

Page{

    property int connected: connhandling.connected
    property var started: 0

    onConnectedChanged: {
        connected? startbutton.enabled = true : startbutton.enabled = false
        connected? started = 0 : started  = false
    }

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
            color: "red"
            height: scrollingArea.height
            width: scrollingArea.width


            RoundButton{

                id: startbutton
                x: (parent.width/2) -width/2
                y:  parent.height/2-height/2
                height: window.height/8
                width: height
                text: started? "Stop": "Start"
                font.pointSize: window.height/60
                onClicked: {
                    started? connhandling.sendStop() : connhandling.sendStart()
                    started? started = 0 : started = 1
                }

            }

            Label{
                id: connectingLabel
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: startbutton.bottom
                anchors.topMargin: parent.height /60
                text: connhandling.connected? "Connected to " + connhandling.devicename
                                            +" "+connhandling.deviceaddress
                                            :"Disconnected"
            }
        }
    }
}
