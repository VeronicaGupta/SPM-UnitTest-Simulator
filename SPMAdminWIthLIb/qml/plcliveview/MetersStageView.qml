import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle{
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    anchors.margins: 20
    height: width * 16/100
    radius: 20

    border.color: Qt.lighter("blue")
    property alias animate: animation

    Column{
        id: meterViewItem
        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 0
        anchors.topMargin: 10

        property int meterWidth: (width / 12) - 5

        height: parent.height - 10

        ListModel{
            id:stageModel;
            ListElement{stageName: "Glue"; stage: "S12"}
            ListElement{stageName: "Rejection"; stage: "S11"}
            ListElement{stageName: "Nicsync"; stage: "S10"}
            ListElement{stageName: "Laser"; stage: "S9"}
            ListElement{stageName: "HighError"; stage: "S8"}
            ListElement{stageName: "LowError"; stage: "S7"}
            ListElement{stageName: "Starting"; stage: "S6"}
            ListElement{stageName: "NeutCal"; stage: "S5"}
            ListElement{stageName: "PhaseCal"; stage: "S4"}
            ListElement{stageName: "HV"; stage: "S3"}
            ListElement{stageName: "Meggar"; stage: "S2"}
            ListElement{stageName: "Function"; stage: "S1"}
        }

        ListView{
            id: listView
            width: parent.width
            height: 60

            orientation: ListView.Horizontal
            spacing: 5
            model: stageModel
            delegate: MeterView{ width: meterViewItem.meterWidth; height: 50; text: "S" + (index - 12) + "(" +  String.fromCharCode(76 - index) + ")"  +"\n" + stageName }
        }

        Rectangle{
            width: parent.width - 5
            height: meterViewItem.meterWidth + 10
            clip: true

            Row{
                id:row
                spacing: 5
                width: parent.width

                Repeater{
                    model: listModel
                    MeterView{ width: meterViewItem.meterWidth; height: width * 1.1; text: "Meter-" + serial + "\n" + errorStatus; test2: model.status; color: getColor(errorStatus); show: model.visible}
                }

                SequentialAnimation {
                    id: animation
                    NumberAnimation { target: row; property: "x"; from: 0; to: (meterViewItem.meterWidth + 5) * -1; duration: 2000 }
                    onFinished: {
                        row.x = 0
                        listModel.remove(0, 1)
                    }
                }
            }
        }
    }

    function getColor(error){
        var outColor

        if(error.length === 0 || error === "-")
            outColor = "skyblue"
        else if(error === "P")
            outColor = "green"
        else
            outColor = "red"

        return outColor
    }
}
