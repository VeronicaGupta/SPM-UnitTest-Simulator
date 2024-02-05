import QtQuick 2.12
import QtQuick.Controls 2.12
import SPMControls 1.0

import "plcliveview"

Rectangle {
    anchors.fill: parent
    anchors.margins: 20

    property int meterNumber: 1
    property int meterToBeProduced: 300

    ListModel{
        id: listModel
        function updateStatus(status){

            if(status.length !== 12)
                return;

            var part = status
            for(var i=0; i<part.length; i++){
                var c = part.substring(i, i+1)

                console.log("****", i, c, String.fromCharCode(65 + i), part.length)
                var index = 11 - i;
                var info = "" + plcc.getTmpSerial(index) + "\n" + plcc.getFinalSerial(index)
                var tmp = listModel.get(index);

                tmp.status = info
                if(tmp.errorStatus !== c){
                    tmp.errorStatus = c
                }
                listModel.set(index, tmp)
            }
        }
    }

    Component.onCompleted: {
        for(var i=0; i<12; i++ )
            listModel.append({"serial": i, "visible": false, "blinking": false, "errorStatus": "-", "status": " AB "})
    }

    MetersStageView{
        id: meterStageView
    }

    Row{
        spacing: 5
        TextField{
            id: tfMeterCount
            onTextChanged: meterToBeProduced = parseInt(tfMeterCount.text)
        }
        CheckBox{
            id: cbTesting
            onCheckStateChanged: {
                for(var i=0; i< listModel.count; i++){
//                    listModel.i
                }
            }
        }

        Button{
            text: "Move"
            onClicked: {
//                plcc.sendCommand("Part_Result_Read_Success")
                if(meterNumber > meterToBeProduced)
                    listModel.append({"serial": meterNumber++, "visible": false, "errorStatus": "P", "blinking": (cbTesting.checkState == Qt.Checked), "status": "ab" })
                else
                    listModel.append({"serial": meterNumber++, "visible": true, "errorStatus": "P", "blinking": (cbTesting.checkState == Qt.Checked), "status": "ab"})

//                meterStageView.animate.running = true

//                var testsToRun = 0;
//                for(var i=0; i< listModel.count; i++)
//                    if(listModel.get(i).visible === true)
//                        testsToRun |= (1<< (12 - i));
            }
        }

        Button{
            id: btnStartSPM
            text: "Start SPM"
            onClicked: {
                ////    KA6097051-KA6097150
                plcc.startProduction("APDCL/CGM(PP&D)/SMART METER (DBFOOT)/PKG-1/2021/28 Dated. 12.11.2021",
                                     "APDCL/CGM(PP&D)/SMART METER (DBFOOT)/PKG-2/2021/29 Dated. 12.11.2021", "Laser", "AS", 2343501, 2344000) // for simulated mode
                plcc.startProduction()
                btnStartSPM.enabled = false;
            }
        }
        Button{
            text: "Calibrate"
            onClicked: {
                plcc.calibrate()
            }
        }

        Button{
            text: "FunctionTst"
            onClicked: {
                var info = plcc.getMeterInfo(1)
                console.log(info.getTmpSerial(), info.getFinalSerial())
            }
        }
        Button{
            text: "Pause SPM"
            onClicked: {

//                listModel.updateStatus("PPBPPC------")
                plcc.printMeterInfo()
                for(var i=0; i<listModel.count; i++){
                    var row = listModel.get(i)
                    if(i==5){
                        row.visible = true
                        row.errorStatus = "G"
                    }
                    console.log(i, row.serial, row.visible, row.errorStatus)
                }
            }
        }
        Button{
            text: "Print Model"
            onClicked: {
//                for(var i=0; i< listModel.count; i++){
//                    console.log(i, JSON.stringify(listModel.get(i)))

//                }
//                plcc.startProduction("A123", "kuchaskasmir", "A123", 1, 100)
                plcc.startTest()
            }
        }

        TextField{
            id: tfCommand
            onTextChanged: meterToBeProduced = parseInt(tfMeterCount.text)
        }
        Button{
            text: "SendCommand"
            onClicked: {
                plcc.sendCommand(tfCommand.text)
            }
        }
        Button{
            text: "Test1"
            onClicked: {
                plcc.temp1()
            }
        }
        Button{
            text: "Test1"
            onClicked: {
                plcc.temp2()
            }
        }

    }

    PLCController{
        id: plcc

        onTestStarted: {
            console.log("QML Move Meter", listModel.count)

//            var info =  plcc.getMeterInfo(11)
            var meterStatus = "" + plcc.getTmpSerial(11) + "\n" + plcc.getFinalSerial(11)

            if(meterNumber > meterToBeProduced)
                listModel.append({"serial": meterNumber++, "visible": false, "errorStatus": "-", "blinking": (cbTesting.checkState == Qt.Checked), "status": "H" + meterStatus })
            else
                listModel.append({"serial": meterNumber++, "visible": true, "errorStatus": "-", "blinking": (cbTesting.checkState == Qt.Checked), "status": "H" + meterStatus})

            if(listModel.count > 14){
                listModel.remove()
            }

            meterStageView.animate.running = true
        }

        onCommandReceived: {
            if(command.startsWith("Start_test")){

            }
            if(command.startsWith("Part_Result_update")){
                var partStatus = command.split(" ")
                var part = partStatus[partStatus.length - 1]

                console.log("*********partStatusB", part)

                listModel.updateStatus(part)

                console.log("*********partStatusA", part)
            }
        }
    }
}
