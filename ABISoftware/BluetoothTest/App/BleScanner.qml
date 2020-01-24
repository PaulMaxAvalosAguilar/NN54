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
            color: "#ffffff"
            height: scrollingArea.height
            width: scrollingArea.width

            Rectangle{
                id: selectyourencoderrect
                height: textLabel.height
                anchors.left: firstRect.left
                anchors.right: firstRect.right
                anchors.top: firstRect.top
                color:"yellow"
                Label{
                    id: textLabel
                    text: "Click to connect:"
                    font.pixelSize: window.height / 25
                    anchors.left: parent.left
                    anchors.right: parent.right
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            ListView{
                id: listView                
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: selectyourencoderrect.bottom
                anchors.bottom: busyIndication.top                
                spacing: 5
                snapMode: ListView.SnapToItem
                clip:true
                flickableDirection: Flickable.VerticalFlick
                boundsBehavior: Flickable.StopAtBounds
                ScrollBar.vertical: ScrollBar {}
                model: blescannermodel
                delegate:Rectangle{
                    id:delegateRectangle
                    width: parent.width
                    height: delegateText.height * 3
                    color: "#d0d1d2"
                    border.width: 2
                    border.color: "black"
                    Label{
                        id:delegateText
                        text: blename +" "+bleaddress + " " + blerssi
                        font.pixelSize: window.height/30
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.centerIn: parent
                        horizontalAlignment: Text.AlignHCenter                        
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            blescannermodel.stopDiscovery()
                            blescannermodel.connectToDevice(index)
                        }
                    }                    
                }
            }

            BusyIndicator {
                id: busyIndication
                running: blescannermodel.scanning
                anchors.left: firstRect.left
                anchors.right:  firstRect.right
                height: firstRect.height * 0.0666
                anchors.bottom: scannButton.top
            }

            Button{
                id: scannButton
                text: blescannermodel.scanning? "Stop":"Start"
                anchors.bottom: scannerLabel.top
                x: (parent.width/2) - width - (parent.width/50)
                height: window.height/20
                width: disconnectButton.width
                font.pixelSize: window.height/30
                enabled: connhandling.connected? false: true
                onClicked: {
                    blescannermodel.scanning? blescannermodel.stopDiscovery() : blescannermodel.startDiscovery()
                }
            }

            Button{
                id: disconnectButton
                text: "Disconnect"
                anchors.bottom: scannerLabel.top
                x: (parent.width/2) + (parent.width/50)
                height: window.height/20
                font.pixelSize: window.height/30
                enabled: connhandling.connected
                onClicked: {
                    blescannermodel.clear()
                    connhandling.disconnect()
                }
            }

            Label{
                id: scannerLabel
                anchors.horizontalCenter: firstRect.horizontalCenter
                anchors.bottom: connectingLabel.top
                font.pixelSize: window.height/30
                text: blescannermodel.scannerState
            }

            Label{
                id: connectingLabel
                anchors.horizontalCenter: firstRect.horizontalCenter
                anchors.bottom: connectionLabel.top
                font.pixelSize: window.height/30
                text: connhandling.connected? "Connected to " + connhandling.devicename
                                            +" "+connhandling.deviceaddress
                                            :"Disconnected"
            }

            Label{
                id: connectionLabel
                anchors.horizontalCenter: firstRect.horizontalCenter
                font.pixelSize: window.height/30
                anchors.bottom: parent.bottom
                text: connhandling.connecting? "Connecting to " + connhandling.devicename
                                            +" "+connhandling.deviceaddress
                                            :""
            }
        }
    }
}
