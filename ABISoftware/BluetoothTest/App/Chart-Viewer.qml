import QtQuick 2.0
import QtQuick.Controls 2.2
import QtCharts 2.0
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

                ChartView {
                    id: chart
                    title: "Series Velocity"
                    width: firstRect.width
                    anchors.top: firstRect.top
                    anchors.bottom: toggleButton.top
                    antialiasing: true
                    legend.alignment: Qt.AlignBottom
                    legend.markerShape: Legend.MarkerShapeCircle

                    ValueAxis {
                        id: axisX
                        labelFormat: "%d"
                        tickCount: 2
                        Component.onCompleted: applyNiceNumbers()
                    }

                    ValueAxis {
                        id: axisY
                        min: 0
                        max: 300
                        labelFormat: "%d"
                    }



                    ScatterSeries {
                        id: scatterseries
                        name: "MPV(Mean Propulsive Velocity)"
                        pointLabelsVisible: true
                        pointLabelsFormat: "@xPoint"
                        markerSize: (window.width *window.height )/ 65000.0
                        axisX: axisX
                        axisY: axisY
                    }

                    SplineSeries{
                        id: lineseries
                        axisX: axisX
                        axisY: axisY
                    }
            }

            Button{
                property bool toggleNumber: true
                id: toggleButton
                height: firstRect * .10
                anchors.bottom: firstRect.bottom
                anchors.horizontalCenter: firstRect.horizontalCenter
                onClicked: {
                    if(toggleNumber == true){
                        timer.stop()
                        toggleNumber = false
                    }else{
                        timer.start()
                        toggleNumber = true
                    }
                }
            }

            Timer {
                id: timer
                interval: 500; running: true; repeat: true
                onTriggered: firstRect.append()
            }


            Connections{
                target: connhandling
                onComputedValueChanged: {

                }
            }

            property int valx: 1
            property int valy: 200

            function append(){
                valy = Math.random() * (200 - 100) + 100
                scatterseries.append(valx, valy)
                lineseries.append(valx++, valy)
                axisX.max = (scatterseries.at(scatterseries.count-1).x)+1
                axisX.applyNiceNumbers()
            }
        }
    }
}
