import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1

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
            text:"Ble-Scanner"
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

            Rectangle{
                id: selectyourencoderrect
                height: textLabel.height
                anchors.left: firstRect.left
                anchors.right: firstRect.right
                anchors.top: firstRect.top
                color:"#3548f2"
                Label{
                    id: textLabel
                    text: "Click to connect:"
                    font.pointSize: 25
                    anchors.left: parent.left
                    anchors.right: parent.right
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            ListView{
                id: listView
                width: parent.width - (parent.width * 0.5)
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: busyIndication.top
                anchors.top: selectyourencoderrect.bottom
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
                    height: delegateText.height * 2
                    color: "#d0d1d2"
                    border.width: 2
                    border.color: "black"
                    Label{
                        id:delegateText
                        text: blename +" "+bleaddress
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.centerIn: parent
                        horizontalAlignment: Text.AlignHCenter                        
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
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
                anchors.bottom: buttonsLayout.top
            }

            RowLayout{
                id: buttonsLayout
                anchors.horizontalCenter: firstRect.horizontalCenter
                anchors.bottom: scannerLabel.top
                Button{
                    id: scannButton
                    text: "Scan!"
                    enabled: !blescannermodel.scanning
                    onClicked: {
                        blescannermodel.scanning = true
                        blescannermodel.startDiscovery()
                    }
                }

                Button{
                    id: disconnectButton
                    text: "Disconnect"
                    enabled: connhandling.connected
                    onClicked: {
                        connhandling.disconnect()
                    }
                }
            }

            Label{
                id: scannerLabel
                anchors.horizontalCenter: firstRect.horizontalCenter
                anchors.bottom: connectingLabel.top
                text: blescannermodel.scannerState
            }

            Label{
                id: connectingLabel
                anchors.horizontalCenter: firstRect.horizontalCenter
                anchors.bottom: connectionLabel.top
                text: connhandling.connected? "Connected to " + connhandling.devicename
                                            +" "+connhandling.deviceaddress
                                            :"Disconnected"
            }

            Label{
                id: connectionLabel
                anchors.horizontalCenter: firstRect.horizontalCenter
                y:firstRect.height - (firstRect.height * 0.1)
                text: connhandling.connecting? "Connecting to " + connhandling.devicename
                                            +" "+connhandling.deviceaddress
                                            :""
            }
        }
    }
}
