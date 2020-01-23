import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.12
import QtCharts 2.3

Page{

    property var traveledDist: connhandling.traveledDistance
    property var meanPropVel: connhandling.meanPropVel
    property var peakVel: connhandling.peakVel

    property var currentView: homePageView
    property int connected: connhandling.connected
    property var started: 0
    property var soundEnabled: true

    property int valx: 1
    property int valy: 0
    property int minWspd: 0
    property int maxWspd: 250
    property int maxVel: 0
    property int fatigue: 0
    property var graphNameText: ""

    property var startButtonTopPosition: (scrollingArea.height/2)- startbutton.height/2
    property var startButtonBottPosition: scrollingArea.height - startbutton.height - scrollingArea.height/60

    property var timerCount: timerCountTextField.value

    onConnectedChanged: {
        connected? "": startbutton.y = startButtonTopPosition
        //connected? startbutton.enabled = true : startbutton.enabled = false
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

            Rectangle{
                id:quickSettingsLayout
                color: "red"
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                width:  (parent.width/5)*2 + height
                height: parent.height/15

                Button{
                    id:settingsButton
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    width: firstRect.width/5
                    font.pixelSize: parent.height/1.5
                    text: "Settings"

                    onClicked:{
                        scrollingArea.changeView(secondRect)
                    }
                }

                Label{
                    id:soundLabel
                    color: "white"
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: soundCheckbox.left
                    width: firstRect.width/5
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: parent.height/1.5
                    text: "Sound:"
                }


                CheckBox{
                    id:soundCheckbox
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    indicator.height: quickSettingsLayout.height
                    indicator.width: indicator.height
                    display: AbstractButton.IconOnly
                    checked: true

                    onCheckStateChanged: function(){
                        if(checkState == Qt.Checked){
                            soundEnabled = true
                        }else{
                            soundEnabled = false
                        }
                    }
                }
            }

            Rectangle{
                id:dataRectangle
                width: firstRect.width
                anchors.top: quickSettingsLayout.bottom
                anchors.topMargin: firstRect.height/85
                height:startButtonBottPosition - startbutton.height/2 - quickSettingsLayout.height
                opacity: 0


                ChartView {
                    id: chart
                    title: graphNameText
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: grid.top
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


                GridLayout {
                        id: grid

                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: parent.height/10

                        rowSpacing: 0


                        columns: 4

                        Rectangle{
                            id:traveledDistunitRect
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            border.color: "black"
                            border.width: 1
                            Label{
                                id:travelunitlabel
                                anchors.fill: parent
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                text: "cm"
                                clip: true
                                font.pointSize: window.height / 70
                            }
                        }

                        Rectangle{
                            id:meanPropVelunitRect
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            border.color: "black"
                            border.width: 1
                            Label{
                                id:mpvunitlabel
                                anchors.fill: parent
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                text: "m/s"
                                clip: true
                                font.pointSize: window.height / 70
                            }
                        }

                        Rectangle{
                            id:peakVelunitRect
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            border.color: "black"
                            border.width: 1
                            Label{
                                id:peakunitlabel
                                anchors.fill: parent
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                text: "m/s"
                                clip: true
                                font.pointSize: window.height / 70
                            }
                        }

                        Rectangle{
                            id:fatigueunitRect
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            border.color: "black"
                            border.width: 1
                            Label{
                                id:fatigueunitlabel
                                anchors.fill: parent
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                text: "%"
                                clip: true
                                font.pointSize: window.height / 70
                            }
                        }


                        Rectangle{
                            id:traveledDistRect
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            border.color: "black"
                            border.width: 1
                            Label{
                                id:travellabel
                                anchors.fill: parent
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                wrapMode: Text.Wrap
                                font.pointSize: window.height / 70
                            }
                        }

                        Rectangle{
                            id:meanPropVelRect
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            border.color: "black"
                            border.width: 1
                            Label{
                                id:mpvlabel
                                anchors.fill: parent
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                wrapMode: Text.Wrap
                                font.pointSize: window.height / 70
                            }
                        }

                        Rectangle{
                            id:peakVelRect
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            border.color: "black"
                            border.width: 1
                            Label{
                                id:peaklabel
                                anchors.fill: parent
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                wrapMode: Text.Wrap
                                font.pointSize: window.height / 70
                            }
                        }

                        Rectangle{
                            id:fatigueRect
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            border.color: "black"
                            border.width: 1
                            Label{
                                id:fatiguelabel
                                anchors.fill: parent
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                text: "nA"
                                wrapMode: Text.Wrap
                                font.pointSize: window.height / 70
                            }
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
                //enabled: false
                enabled: true
                font.pointSize: window.height/60
                onClicked: {
                    started? connhandling.sendStop() : ""
                    started? y = Qt.binding(function(){return startButtonTopPosition}) :
                             y = Qt.binding(function(){return startButtonBottPosition})
                    started? settingsButton.enabled = true : settingsButton.enabled = false
                    started? dataRectangle.opacity = 0: ""

                    started? "": scatterseries.clear()
                    started? "": lineseries.clear()
                    started? "": valx = 1
                    started? "": maxWspd = 255

                    started? "": travellabel.text = 0
                    started? "": mpvlabel.text = 0
                    started? "": peaklabel.text = 0
                    started? "": fatiguelabel.text = "nA"

                    started? "": maxVel = 0
                    started? "": fatigue = 0

                    started? timerCount = Qt.binding(function(){return timerCountTextField.value}) : ""
                    started? timerText.opacity = 0 : timerText.opacity = 1;
                    started? "": connhandling.say(timerCountTextField.value);
                    started? timer.stop() : timer.start()

                    modeTextField.currentIndex? graphNameText = "MPV" :graphNameText = "PV"

                    started? started = 0 : started = 1

                }

                Behavior on y {
                    NumberAnimation {duration:250}
                }                
            }

            Text{
                id: timerText
                anchors.fill: parent
                font.pixelSize: parent.height/2.5
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: timerCount
                opacity: 0
            }

            Timer{
                id: timer
                interval: 1000
                onTriggered: function(){

                    if(timerCount != 0){
                        timerCount--
                        timer.start()

                        if(soundEnabled){
                        connhandling.say(timerCount)
                        }
                    }

                    if(timerCount == 0){                                            
                        connhandling.sendStart(minDistToTravelTextField.value,
                                               desiredCountDirTextField.currentIndex,
                                               desiredRepDirTextField.currentIndex);

                        timerText.opacity  = 0
                        dataRectangle.opacity = 1
                    }
                }
            }
        }

        Rectangle{
            id:secondRect
            height: scrollingArea.height
            width: scrollingArea.width
            color:"red"
            visible:false

            ColumnLayout{
                id: settingsLayout
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: parent.width/15
                anchors.rightMargin: parent.width/15
                anchors.verticalCenter:  parent.verticalCenter
                spacing: 0

                height: numberOfElements * elementsHeight
                property var elementsHeight:  parent.height/10
                property var numberOfElements: 6

                Rectangle{
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Rectangle{
                        id: modeLabel
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        width: parent.width/2
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
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        width: parent.width/2
                        model: ["Olympic","PL"]
                        currentIndex: 1
                    }
                }

                Rectangle{
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Rectangle{
                        id: minDistToTravelLabel
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        width: parent.width/2
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
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        width: parent.width/2
                        from: 0
                        to: 255
                        value: 25
                        editable: true
                    }
                }

                Rectangle{
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Rectangle{
                        id: desiredCountDirLabel
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        width: parent.width/2
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
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        width: parent.width/2
                        model: ["Descendant","Ascendant"]
                        currentIndex: 1
                    }
                }

                Rectangle{
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Rectangle{
                        id: desiredRepDirLabel
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        width: parent.width/2
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
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        width: parent.width/2
                        model: ["Descendant","Ascendant"]
                        currentIndex: 1

                    }
                }

                Rectangle{
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Rectangle{
                        id: fatigueLabel
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        width: parent.width/2
                        border.color: "black"
                        border.width: 3
                        Label{
                            text: "Fatigue:"
                            wrapMode: Text.Wrap
                            clip: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            anchors.fill: parent
                        }
                    }

                    SpinBox{
                        id: fatigueTextField
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        width: parent.width/2
                        from: 0
                        to: 100
                        value: 10
                        editable: true
                    }
                }

                Rectangle{
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Rectangle{
                        id: timerCountlabel
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        width: parent.width/2
                        border.color: "black"
                        border.width: 3
                        Label{
                            text: "Timer:"
                            wrapMode: Text.Wrap
                            clip: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            anchors.fill: parent
                        }
                    }

                    SpinBox{
                        id: timerCountTextField
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        width: parent.width/2
                        from: 0
                        to: 60
                        value: 10
                        editable: true
                    }
                }
            }

            Button{
                text: "OK"
                anchors.top: settingsLayout.bottom
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

        function addToChart(vel){
            valy = vel
            fatigue = 0;

            //Graph Y Value
            if( valy > maxWspd) {
                maxWspd = valy+10
            }

            //Fatigue Calculation
            if(valy > maxVel){
                maxVel = valy
                fatiguelabel.text = "nA"
            }else{
                fatigue = (  (1-(valy/maxVel))*100 )
                fatiguelabel.text = fatigue
            }

            //Sound Effects
            if(started == 1){
                if(soundEnabled){
                    if(fatigue > fatigueTextField.value){
                        connhandling.soundeffect()
                    }else{
                        connhandling.say(vel/100)
                    }
                }
            }

            scatterseries.append(valx, valy)
            lineseries.append(valx++, valy)
            axisX.max = (scatterseries.at(scatterseries.count-1).x)+1
            axisX.applyNiceNumbers()
        }
    }
}
