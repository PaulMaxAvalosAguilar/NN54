import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.12
import QtCharts 2.3

Page{

    property var traveledDist: connhandling.traveledDistance
    property var meanPropVel: connhandling.meanPropVel
    property var peakVel: connhandling.peakVel

    property var repetitions: 0
    property var traveledDistances: []
    property var meanPropVelocities: []
    property var peakVelocities: []

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
        traveledDistances.push(traveledDist)
    }

    onMeanPropVelChanged: {
        mpvlabel.text = meanPropVel/100
        modeTextField.currentIndex? scrollingArea.addToChart(meanPropVel) :""
        meanPropVelocities.push(meanPropVel)
    }

    onPeakVelChanged: {
        peaklabel.text = peakVel/100
                !modeTextField.currentIndex? scrollingArea.addToChart(peakVel): ""
        peakVelocities.push(peakVel)
    }

    ScrollView {
        id: scrollingArea
        anchors.centerIn: parent.Center
        clip: true
        width: viewRectangle.width
        height:  viewRectangle.height
        contentWidth: viewRectangle.width
        contentHeight: viewRectangle.height
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
                width:  (firstRect.width/6)*4
                height: window.height/20

                Button{
                    id:settingsButton
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    width: firstRect.width/6
                    font.pixelSize: parent.height/1.5
                    text: "Settings"

                    onClicked:{
                        scrollingArea.changeView(secondRect)
                    }
                }

                Button{
                    id: repsButton
                    anchors.top: parent.top
                    anchors.left: settingsButton.right
                    anchors.leftMargin: firstRect.width/15
                    anchors.bottom: parent.bottom
                    width: firstRect.width/6
                    font.pixelSize: parent.height/1.5
                    text: "Reps"

                    onClicked:{
                        scrollingArea.changeView(thirdRect)
                    }
                }

                Label{
                    id:soundLabel
                    color: "yellow"
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: soundCheckbox.left
                    anchors.left: repsButton.right
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: parent.height/1.5
                    clip: true
                    text: "SOUND:"
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
                height:startButtonBottPosition - quickSettingsLayout.height - anchors.topMargin - anchors.topMargin
                opacity: 1

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
                        height: parent.height/7

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
                                font.pixelSize: parent.height / 1.5
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
                                font.pixelSize: parent.height / 1.5
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
                                font.pixelSize: parent.height / 1.5
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
                                font.pixelSize: parent.height / 1.5
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
                               font.pixelSize: parent.height / 1.5
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
                                font.pixelSize: parent.height / 1.5
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
                                font.pixelSize: parent.height / 1.5
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
                                font.pixelSize: parent.height / 1.5
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
                font.pixelSize: parent.height/25
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

                    started? "": repetitions = 0
                    started? "": traveledDistances.length = 0;
                    started? "": meanPropVelocities.length = 0;
                    started? "": peakVelocities.length = 0;

                    started? timerCount = Qt.binding(function(){return timerCountTextField.value}) : ""
                    started? timerText.opacity = 0 : timerText.opacity = 1;

                    if(soundEnabled){
                        started? "": connhandling.sayCount(timerCountTextField.value);
                    }

                    started? timer.stop() : timer.start()

                    started? scrollingArea.createTable() : scrollingArea.deleteTable()
                    started? repsButton.enabled = true : repsButton.enabled = false

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
                font.pixelSize: window.height/2.5
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
                        connhandling.sayCount(timerCount)
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
            id:thirdRect
            anchors.top: parent.top
            height: viewRectangle.height
            width: viewRectangle.width
            visible: false
            color: "red"

            Rectangle{
                id: titleRectangle
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: parent.width/15
                anchors.rightMargin: parent.width/15
                height: window.height/10

                Rectangle{
                    id: repLabel
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    width: parent.width/4
                    border.color: "black"
                    border.width: 3
                    Label{
                        text: "#Rep"
                        font.pixelSize: window.height/25
                        wrapMode: Text.Wrap
                        clip: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        anchors.fill: parent
                    }
                }

                Rectangle{
                    id: traveledDistanceLabel
                    anchors.top: parent.top
                    anchors.left: repLabel.right
                    anchors.bottom: parent.bottom
                    width: parent.width/4
                    border.color: "black"
                    border.width: 3
                    Label{
                        text: "TravelDist"
                        font.pixelSize: window.height/25
                        wrapMode: Text.Wrap
                        clip: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        anchors.fill: parent
                    }
                }

                Rectangle{
                    id: peakVelLabel
                    anchors.top: parent.top
                    anchors.left: traveledDistanceLabel.right
                    anchors.bottom: parent.bottom
                    width: parent.width/4
                    border.color: "black"
                    border.width: 3
                    Label{
                        text: "PeakVel"
                        font.pixelSize: window.height/25
                        wrapMode: Text.Wrap
                        clip: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        anchors.fill: parent
                    }
                }

                Rectangle{
                    id: meanPropulsiveVelLabel
                    anchors.top: parent.top
                    anchors.left: peakVelLabel.right
                    anchors.bottom: parent.bottom
                    width: parent.width/4
                    border.color: "black"
                    border.width: 3
                    Label{
                        text: "MeanPropVel"
                        font.pixelSize: window.height/25
                        wrapMode: Text.Wrap
                        clip: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        anchors.fill: parent
                    }
                }
            }

            /*
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    for(var i = 0; i < 15; i++)
                    scrollingArea.createElement()
                }
            }
            */

            Rectangle{
                id: scrollViewRect
                anchors.top: titleRectangle.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: parent.width/15
                anchors.rightMargin: parent.width/15
                anchors.bottom: thirdRectBackButton.top

                ScrollView{
                    id: repsScrollView
                    clip:true
                    anchors.centerIn: scrollViewRect.Center
                    width: scrollViewRect.width
                    height: scrollViewRect.height
                    contentWidth: scrollViewRect.width
                    contentHeight: scrollViewRect.height
                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                    ScrollBar.vertical.policy: ScrollBar.AsNeeded

                    ColumnLayout{
                        id: repsLayout

                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.left: parent.left
                        spacing: 0

                        height: numberOfElements * elementsHeight
                        property var elementsHeight:  window.height/10
                        property var numberOfElements: 0
                        property var elementsSurpassed: 0

                        onHeightChanged: {

                            if(elementsSurpassed){
                                repsScrollView.contentHeight = (height + elementsHeight*2)
                            }

                            if(height > scrollViewRect.height){
                                elementsSurpassed = 1;
                            }

                        }
                    }
                }
            }

            Button{
                id:thirdRectBackButton
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: window.height/50
                height: window.height/17
                font.pixelSize: window.height/30
                text: "Ok"
                onClicked: {
                    scrollingArea.changeView(firstRect)
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
                property var elementsHeight:  window.height/10
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
                            font.pixelSize: window.height/25
                            wrapMode: Text.Wrap
                            clip: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            anchors.fill: parent
                        }
                    }

                    ComboBox{
                        id:modeTextField
                        font.pixelSize: window.height/25
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
                            font.pixelSize: window.height/25
                            wrapMode: Text.Wrap
                            clip: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            anchors.fill: parent
                        }
                    }

                    SpinBox{
                        id: minDistToTravelTextField
                        font.pixelSize: window.height/25
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
                            font.pixelSize: window.height/25
                            wrapMode: Text.Wrap
                            clip: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            anchors.fill: parent
                        }
                    }

                    ComboBox{
                        id:desiredCountDirTextField
                        font.pixelSize: window.height/25
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
                            font.pixelSize: window.height/25
                            wrapMode: Text.Wrap
                            clip: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            anchors.fill: parent
                        }
                    }

                    ComboBox{
                        id: desiredRepDirTextField
                        font.pixelSize: window.height/25
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
                            font.pixelSize: window.height/25
                            wrapMode: Text.Wrap
                            clip: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            anchors.fill: parent
                        }
                    }

                    SpinBox{
                        id: fatigueTextField
                        font.pixelSize: window.height/25
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
                            font.pixelSize: window.height/25
                            wrapMode: Text.Wrap
                            clip: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            anchors.fill: parent
                        }
                    }

                    SpinBox{
                        id: timerCountTextField
                        font.pixelSize: window.height/25
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
                font.pixelSize: window.height/30
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
                        connhandling.saySpeed(vel)
                    }
                }
                repetitions++;                
            }

            scatterseries.append(valx, valy)
            lineseries.append(valx++, valy)
            axisX.max = (scatterseries.at(scatterseries.count-1).x)+1
            axisX.applyNiceNumbers()
        }

        function deleteTable(){

            repsLayout.numberOfElements = 0;
            for(var i = repsLayout.children.length; i > 0 ; i--) {
                console.log("destroying: " + i)
                repsLayout.children[i-1].destroy()
            }
        }

        function createTable(){
            for(var i = 0; i < repetitions; i++){
                createElement(i)
            }
        }

        property var component;
        function createElement(elementNumber){
            component = Qt.createComponent("RepsRectangle.qml");
            if(component.status === Component.Ready || component.status === Component.Error) {
                finishCreation(elementNumber);
            }else{
                component.statusChanged.connect(finishCreation(elementNumber));
            }
        }

        function finishCreation(elementNumber){
            if(component.status === Component.Ready) {
                var rectangle = component.createObject(repsLayout, {
                                                           "rep": elementNumber +1,
                                                           "traveledDistance": traveledDistances[elementNumber]/100,
                                                           "meanPropulsiveVel": meanPropVelocities[elementNumber]/100,
                                                           "peakVel": peakVelocities[elementNumber]/100
                                                       });
                repsLayout.numberOfElements++;
                if(rectangle ===null) {
                    console.log("Error creating image");
                }
            }else if(component.status === Component.Error) {
                console.log("Error loading component:", component.errorString());
            }
        }
    }
}
