import QtQuick 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    visible: true
    width: 1280
    height: 480
    title: qsTr("Scroll")

    PLCLiveView{
        id: plc

    }

//    SerialPortSettings{

//    }
}
