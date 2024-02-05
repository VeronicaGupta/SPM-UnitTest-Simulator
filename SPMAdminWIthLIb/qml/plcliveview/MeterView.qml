import QtQuick 2.12
import QtQuick.Controls 2.12

Item{
    id: root
    property string meterSequenceNumber
    property alias text: lbl.text
    property alias test2: lbl2.text
    property string color: "blue"

    property bool show: true
    property bool blink: false

    width: 50
    height: width * 1.5

    Rectangle{
        id: rectMeter
        anchors.fill: parent
        radius: 10
        color: show ? root.color : "white"

        SequentialAnimation {
            loops: Animation.Infinite
            PropertyAnimation {target: rectMeter; property: "opacity"; from: 1.0; to: 0.5; duration: 1000 }
            PropertyAnimation {target: rectMeter; property: "opacity"; from: 0.5; to: 1.0; duration: 1000 }
            running: blink
        }

        Label{
            id: lbl
            anchors.centerIn: parent
            text: "S" + serial
            horizontalAlignment: Label.AlignHCenter
            color: "white"
            font.bold: true
            visible: show
        }
        Label{
            id: lbl2
//            anchors.centerIn: parent
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: lbl.bottom
            horizontalAlignment: Label.AlignHCenter
            color: "white"
            font.bold: true
            visible: show
        }
    }
}
