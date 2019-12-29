import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.1

Page{

    ScrollView {
        id: scrollingArea
        anchors.fill: parent
        clip: false
        contentWidth: window.width
        contentHeight: (window.height - footerRectangle.height -headerRectangle.height)*1
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded

        Rectangle{
            id: firstRect
            color: "#ff00ff"
            height: scrollingArea.height
            width: scrollingArea.width


        }
    }
}
