import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.1

Rectangle{
    Layout.fillHeight: true
    Layout.fillWidth: true

    property var rep:0
    property var traveledDistance: 0
    property var meanPropulsiveVel: 0
    property var peakVel:0
    property var fatigue: 0

    property var elementsWidth: width/5

    property var backColor: ""

    Rectangle{
        id: repLabel
        color: backColor
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: elementsWidth
        border.color: "black"
        border.width: 1
        Label{
            text: rep
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
        color: backColor
        anchors.top: parent.top
        anchors.left: repLabel.right
        anchors.bottom: parent.bottom
        width: elementsWidth
        border.color: "black"
        border.width: 1
        Label{
            text: traveledDistance
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
        color: backColor
        anchors.top: parent.top
        anchors.left: traveledDistanceLabel.right
        anchors.bottom: parent.bottom
        width: elementsWidth
        border.color: "black"
        border.width: 1
        Label{
            text: meanPropulsiveVel
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
        color: backColor
        anchors.top: parent.top
        anchors.left: meanPropulsiveVelLabel.right
        anchors.bottom: parent.bottom
        width: elementsWidth
        border.color: "black"
        border.width: 1
        Label{
            text: peakVel
            font.pixelSize: window.height/25
            wrapMode: Text.Wrap
            clip: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
        }
    }

    Rectangle{
        id: fatigueLabel
        color: backColor
        anchors.top: parent.top
        anchors.left: peakVelLabel.right
        anchors.bottom: parent.bottom
        width: elementsWidth
        border.color: "black"
        border.width: 1
        Label{
            text: fatigue
            font.pixelSize: window.height/25
            wrapMode: Text.Wrap
            clip: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.fill: parent
        }
    }

}
