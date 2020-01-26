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

    property var elementsWidth: width/4

    Rectangle{
        id: repLabel
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
        id: peakVelLabel
        anchors.top: parent.top
        anchors.left: traveledDistanceLabel.right
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
        id: meanPropulsiveVelLabel
        anchors.top: parent.top
        anchors.left: peakVelLabel.right
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

    color: "blue"
}
