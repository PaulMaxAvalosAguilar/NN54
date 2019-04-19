import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtCharts 2.0

Window {
    id:window
    visible: true
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    title: "Encoder App"

    property int footerHeight: footer.height

    StackView {
        id: stack

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: footer.top
        anchors.top: parent.top
        initialItem: HomePage{

        }
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
            font.pointSize: 13
            anchors.left: parent.left
            anchors.right: parent.right
            text: "V.B.T Encoder by S(T)"
            horizontalAlignment: Text.AlignHCenter
        }
    }

    Drawer {
        id: drawer
        width: 0.5 * window.width
        height: window.height - footer.height

        Rectangle{
            id: openAppRect
            height: textLabel.height
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            color:"#3548f2"
            Label{
                id: textLabel
                text: "App to open:"
                font.pointSize: 25
                anchors.left: parent.left
                anchors.right: parent.right
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        ListView {
            focus: true
            currentIndex: -1
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom:  parent.bottom
            anchors.top: openAppRect.bottom

            delegate: ItemDelegate {
                width: parent.width
                text: model.text
                highlighted: ListView.isCurrentItem
                onClicked: {
                    drawer.close()
                    open(model.text)
                }
            }

            model: ListModel {
                ListElement {
                    text: "Ble-Scanner"
                }

                ListElement {
                    text: "Chart-Viewer"
                }
            }
        }
    }

    function open(name){
        stack.push("qrc:/" + name + ".qml")
    }
}

/*
Window {
    id: window
    width: 640
    height: 480
    visible: true


    Flickable{
        id:flick
        width : parent.width
        height : window.height
        contentHeight: chartview.height
        contentWidth:  chartview.width
        ChartView {
            id: chartview
            width: window.width * 1
            height: window.height
            antialiasing: true

            BarSeries {
                id: mySeries

                BarSet { label: "Bob"; values: [2, 2, 3, 4, 5, 6] }
            }

        }

        ScrollBar.horizontal: ScrollBar {
            id: hbar
            hoverEnabled: true
            active: hovered || pressed
            orientation: Qt.Horizontal
        }
    }
}
*/

