import QtQuick 2.0
import QtQuick.Window 2.2
import sdt.albumModel 1.0
import QtQuick.Controls 2.2

Window {
    id:window
    visible: true
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    title: qsTr("Testing App")

    StackView {
        id: stack

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: footer.bottom
        anchors.top: parent.top
        initialItem: firstPage

    }


    Rectangle{
        id: footer
        color: "#00ff1b"
        height: applicationFooterText.height
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        Label{
            id: applicationFooterText
            anchors.left: parent.left
            anchors.right: parent.right
            text: "V.B.T Encoder by S(T)"
            horizontalAlignment: Text.AlignHCenter
        }
    }


    Page{
        id: firstPage

        header: Label{
            text:"FIRST-PAGE"
            horizontalAlignment: Text.AlignHCenter
            font.pointSize:  30
        }


        Label{
            id: randomText
            height: window/10
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right:  parent.right
            text:"Random-top-text"
            horizontalAlignment: Text.AlignHCenter
            MouseArea{
                anchors.fill: parent
                onClicked: console.log("Random text clicked")
            }
        }


        ListView{
            id: listView
            anchors.bottom: parent.bottom
            anchors.top: randomText.bottom
            anchors.left: parent.left
            anchors.right:  parent.right
            spacing: 5
            model: AlbumModel{}
            delegate:Rectangle{
                id:delegateRectangle
                width: parent.width
                height: delegateText.height
                color: "#d0d1d2"
                border.width: 2
                border.color: "black"
                Label{
                    id:delegateText
                    text: model.name
                    anchors.left: parent.left
                    anchors.right: viewButton.left
                    horizontalAlignment: Text.AlignHCenter
                    MouseArea{
                        anchors.fill: parent
                        onClicked: console.log("Clicked\n" + model.name)
                    }
                }
                Button{
                    id: viewButton
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: parent.width/4
                    text: "Click on me!"
                    onClicked: {
                        stack.push("qrc:/SecondPage.qml")
                    }
                }
            }
        }
    }
}



