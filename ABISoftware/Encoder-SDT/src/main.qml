import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.12
import QtCharts 2.0

ApplicationWindow {
    id:window
    visible: true
    visibility: Qt.WindowFullScreen
    title: "Encoder SDT"

    property var currentView: homePageView
    property var connectedState: bleScannerView.connected

    Rectangle{
        id: headerRectangle
        height: headerLabel.height
        anchors.left: parent.left
        anchors.right: parent.right
        color: "black"

        ToolButton{
            id: backImgbutton
            height: parent.height
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.top: parent.top
            width: window.width * .10
            Image{
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/Images/flecha-hacia-la-izquierda.svg"
            }

            onClicked: {
                headerLabel.text = "VBT App"
                changeView(homePageView)
            }
        }

        ToolButton{
            id: drawImgbutton
            anchors.left: backImgbutton.right
            anchors.leftMargin: parent.width/60
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            width: window.width * .10
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
            text:"VBT App"
            clip: true
            color: "yellow"
            anchors.left: parent.left
            anchors.right: parent.right
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: window.height / 20
        }
    }

    Rectangle{
        id: viewRectangle
        anchors.top: headerRectangle.bottom
        anchors.bottom: footerRectangle.top
        width: window.width

        HomePage{
            id:homePageView
            visible: true
        }

        BleScanner{
            id:bleScannerView
            visible: false
        }

        Encoder{
            id: encoderView
            visible: false
        }
    }

    Rectangle{
        id: footerRectangle
        color: "black"
        height: footerLabel.height
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        Label{
            id: footerLabel
            color: "yellow"
            font.pixelSize: window.height / 25
            anchors.left: parent.left
            anchors.right: parent.right
            text: "V.B.T Encoder by S(T)"
            horizontalAlignment: Text.AlignHCenter
        }
    }

    Drawer {
        id: drawer
        width: 0.5 * window.width
        height: window.height - footerRectangle.height

        Rectangle{
            id: openAppRect
            height: textLabel.height
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            color:"black"
            Label{
                id: textLabel
                color: "yellow"
                text: "App to open:"
                font.pixelSize: window.height/20
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
                font.pixelSize: window.height/25
                highlighted: ListView.isCurrentItem
                onClicked: {
                    drawer.close()
                    headerLabel.text = model.text
                    changeView(model.identification)
                }
            }

            model: ListModel {
                Component.onCompleted: {
                    append({text: "BLE Scanner", identification: bleScannerView});
                    append({text: "Encoder", identification: encoderView})
                }
            }
        }
    }

    function changeView(newView){
        currentView.visible = false
        newView.visible = true
        currentView = newView
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

