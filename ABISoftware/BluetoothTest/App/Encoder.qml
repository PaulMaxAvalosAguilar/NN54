import QtQuick 2.0
import QtQuick.Controls 2.2
import QtCharts 2.3

Page{

    property var traveledDist: connhandling.traveledDistance
    property var meanPropVel: connhandling.meanPropVel
    property var peakVel: connhandling.peakVel

    property var currentView: homePageView
    property int connected: connhandling.connected
    property var started: 0

    property var startButtonTopPosition: (scrollingArea.height/2)- startbutton.height/2
    property var startButtonBottPosition: scrollingArea.height - startbutton.height - scrollingArea.height/60

    property int valx: 1
    property int valy: 0
    property int minWspd: 0
    property int maxWspd: 250
    property int maxVel: 0
    property int fatigue: 0
    property var graphNameText: ""

    onConnectedChanged: {
        connected? "": startbutton.y = startButtonTopPosition
        connected? startbutton.enabled = true : startbutton.enabled = false
        connected? started = 0 : started  = false
    }

    onTraveledDistChanged: {
        travellabel.text = traveledDist

    }

    onMeanPropVelChanged: {
        mpvlabel.text = meanPropVel/100
        modeTextField.currentIndex? scrollingArea.addToChart(meanPropVel) :""
    }

    onPeakVelChanged: {
        peaklabel.text = peakVel/100
        !modeTextField.currentIndex? scrollingArea.addToChart(peakVel): ""
    }

    ScrollView {
        id: scrollingArea
        anchors.fill: parent
        clip: false
        contentWidth: window.width
        contentHeight: (window.height - footerRectangle.height -headerRectangle.height)*1
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded

        property var currentView: firstRect

        Rectangle{
            id: firstRect
            color: "red"
            height: scrollingArea.height
            width: scrollingArea.width
            visible: true

            Button{
                id:settingsButton
                text: "Settings"
                anchors.top: parent.top
                anchors.topMargin: parent.width/65
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked:{
                    scrollingArea.changeView(secondRect)
                }
            }

            Rectangle{
                id:dataRectangle
                width: firstRect.width
                anchors.top: settingsButton.bottom
                anchors.topMargin: firstRect.height/85
                height:startButtonBottPosition - startbutton.height/2 - settingsButton.height
                opacity: 1


                ChartView {
                    id: chart
                    title: graphNameText
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: meanPropVelunitRect.top
                    antialiasing: true
                    legend.visible: false

                    ValueAxis {
                        id: axisX
                        labelFormat: "%d"
                        tickCount: 2
                        Component.onCompleted: applyNiceNumbers()
                    }

                    ValueAxis {
                        id: axisY
                        min: minWspd
                        max: maxWspd
                        labelFormat: "%d"
                    }

                    ScatterSeries {
                        id: scatterseries
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

                Rectangle{
                    id:traveledDistunitRect
                    height: parent.height /10
                    width: parent.width /5
                    anchors.right: meanPropVelunitRect.left
                    anchors.bottom: traveledDistRect.top
                    border.color: "black"
                    border.width: 1
                    Label{
                        id:travelunitlabel
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: "cm"
                        clip: true
                        font.pointSize: window.height / 60
                    }
                }

                Rectangle{
                    id:meanPropVelunitRect
                    height: parent.height /10
                    width: parent.width /5
                    anchors.bottom: meanPropVelRect.top
                    anchors.horizontalCenter: parent.horizontalCenter
                    border.color: "black"
                    border.width: 1
                    Label{
                        id:mpvunitlabel
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: "m/s"
                        clip: true
                        font.pointSize: window.height / 60
                    }
                }

                Rectangle{
                    id:peakVelunitRect
                    height: parent.height /10
                    width: parent.width /5
                    anchors.left: meanPropVelunitRect.right
                    anchors.bottom: peakVelRect.top
                    border.color: "black"
                    border.width: 1
                    Label{
                        id:peakunitlabel
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: "m/s"
                        clip: true
                        font.pointSize: window.height / 60
                    }
                }


                Rectangle{
                    id:traveledDistRect
                    height: parent.height /10
                    width: parent.width /5
                    anchors.right: meanPropVelRect.left
                    anchors.bottom: parent.bottom
                    border.color: "black"
                    border.width: 1
                    Label{
                        id:travellabel
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: "HOLA"
                        clip: true
                        font.pointSize: window.height / 60
                    }
                }

                Rectangle{
                    id:meanPropVelRect
                    height: parent.height /10
                    width: parent.width /5
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    border.color: "black"
                    border.width: 1
                    Label{
                        id:mpvlabel
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: "HOLA"
                        clip: true
                        font.pointSize: window.height / 60
                    }
                }

                Rectangle{
                    id:peakVelRect
                    height: parent.height /10
                    width: parent.width /5
                    anchors.left: meanPropVelRect.right
                    anchors.bottom: parent.bottom
                    border.color: "black"
                    border.width: 1
                    Label{
                        id:peaklabel
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: "HOLA"
                        clip: true
                        font.pointSize: window.height / 60
                    }
                }

                Behavior on opacity {
                    NumberAnimation{ duration: 250}
                }
            }

            RoundButton{
                id: startbutton
                x: (parent.width/2) -width/2
                y:  startButtonTopPosition
                height: window.height/8
                width: height
                text: started? "Stop": "Start"
                enabled: false
                font.pointSize: window.height/60
                onClicked: {
                    started? connhandling.sendStop() : connhandling.sendStart(minDistToTravelTextField.value,
                                                                              desiredCountDirTextField.currentIndex,
                                                                              desiredRepDirTextField.currentIndex)
                    started? y = Qt.binding(function(){return startButtonTopPosition}) :
                             y = Qt.binding(function(){return startButtonBottPosition})
                    started? settingsButton.enabled = true : settingsButton.enabled = false
                    started? dataRectangle.opacity = 0 : dataRectangle.opacity = 1

                    started? "": scatterseries.clear()
                    started? "": lineseries.clear()
                    started? "": valx = 1

                    started? "": travellabel.text = ""
                    started? "": mpvlabel.text = ""
                    started? "": peaklabel.text = ""

                    started? "": maxVel = 0
                    started? "": fatigue = 0

                    modeTextField.currentIndex? graphNameText = "MPV" :graphNameText = "PV"

                    started? started = 0 : started = 1
                }

                Behavior on y {
                    NumberAnimation {duration:250}
                }
            }
        }

        Rectangle{
            id:secondRect
            height: scrollingArea.height
            width: scrollingArea.width
            color:"red"
            visible:false

            Rectangle{
                id:settingsRect
                color:"red"
                height: elementsHeight * 4
                width: parent.width/1.5
                x: (parent.width/2) -width/2
                y:  (parent.height/2)-height/2

                property var elementsWidth: settingsRect.width/2
                property var elementsHeight:  parent.height/10


                Rectangle{
                    id: modeLabel
                    width: parent.elementsWidth
                    height: parent.elementsHeight
                    anchors.top: parent.top
                    anchors.left: settingsRect.left
                    border.color: "black"
                    border.width: 3
                    Label{
                        text: "Desired Count Direction:"
                        wrapMode: Text.Wrap
                        clip: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        anchors.fill: parent
                    }
                }

                ComboBox{
                    id:modeTextField
                    model: ["Olympic","PL"]
                    currentIndex: 1
                    width: parent.elementsWidth
                    height: parent.elementsHeight
                    anchors.top:parent.top
                    anchors.right:  settingsRect.right
                }

                Rectangle{
                    id: minDistToTravelLabel
                    width: parent.elementsWidth
                    height: parent.elementsHeight
                    anchors.top: modeLabel.bottom
                    anchors.left: settingsRect.left
                    border.color: "black"
                    border.width: 3
                    Label{
                        text: "Min Dist To Travel:"
                        wrapMode: Text.Wrap
                        clip: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        anchors.fill: parent
                    }
                }

                SpinBox{
                    id: minDistToTravelTextField
                    from: 0
                    to: 255
                    value: 25
                    editable: true

                    width: parent.elementsWidth
                    height: parent.elementsHeight
                    anchors.top: modeLabel.bottom
                    anchors.right: settingsRect.right
                }

                Rectangle{
                    id: desiredCountDirLabel
                    width: parent.elementsWidth
                    height: parent.elementsHeight
                    anchors.top: minDistToTravelLabel.bottom
                    anchors.left: settingsRect.left
                    border.color: "black"
                    border.width: 3
                    Label{
                        text: "Desired Count Direction:"
                        wrapMode: Text.Wrap
                        clip: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        anchors.fill: parent
                    }
                }

                ComboBox{
                    id:desiredCountDirTextField
                    model: ["Descendant","Ascendant"]
                    currentIndex: 1
                    width: parent.elementsWidth
                    height: parent.elementsHeight
                    anchors.right: settingsRect.right
                    anchors.top: minDistToTravelTextField.bottom
                }


                Rectangle{
                    id: desiredRepDirLabel
                    width: parent.elementsWidth
                    height: parent.elementsHeight
                    anchors.top: desiredCountDirLabel.bottom
                    anchors.left: settingsRect.left
                    border.color: "black"
                    border.width: 3
                    Label{
                        text: "Desired Rep Direction:"
                        wrapMode: Text.Wrap
                        clip: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        anchors.fill: parent
                    }
                }

                ComboBox{
                    id: desiredRepDirTextField
                    model: ["Descendant","Ascendant"]
                    currentIndex: 1
                    width: parent.elementsWidth
                    height: parent.elementsHeight
                    anchors.right: settingsRect.right
                    anchors.top: desiredCountDirTextField.bottom
                }
            }

            Button{
                text: "OK"
                anchors.top: settingsRect.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: parent.height/20
                height: window.height/17
                onClicked:{
                    scrollingArea.changeView(firstRect)
                }
            }

        }

        function changeView(view){
            scrollingArea.currentView.visible = false
            view.visible = true
            scrollingArea.currentView = view
        }

        function addToChart(meanPropVel){
            valy = meanPropVel

            if( valy > maxWspd) {
                maxWspd = valy+10;
            }

            if(valy > maxVel){
                maxVel = valy
            }else{
                fatigue = (  (1-(valy/maxVel))*100 )
                console.log(fatigue)
            }

            scatterseries.append(valx, valy)
            lineseries.append(valx++, valy)
            axisX.max = (scatterseries.at(scatterseries.count-1).x)+1
            axisX.applyNiceNumbers()
        }
    }
}
