import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle {
    anchors.fill: parent
    //    color: "red"
    anchors.margins: 20
    ListModel{
        id: listModel
    }

    ListModel {
        id: libraryModel
        ListElement {
            title: "A Masterpiece"
            author: "Gabriel"
        }
        ListElement {
            title: "Brilliance"
            author: "Jens"
        }
        ListElement {
            title: "Outstanding"
            author: "Frederik"
        }
    }

    TableView {
    //        TableViewColumn {
    //            role: "title"
    //            title: "Title"
    //            width: 100
    //        }
    //        TableViewColumn {
    //            role: "author"
    //            title: "Author"
    //            width: 200
    //        }
        model: libraryModel
    }
//    ListView{
//        id: listView
 //        anchors.leftMargin: 10
//        anchors.rightMargin: 5
//        anchors.topMargin: 10

////        orientation: ListView.Horizontal
//        spacing: 5
//        model: 12
//        delegate: Row{
//            Rectangle{
//                width: (listView.width / 12) - 5
//                height: 50
//                radius: 10
////                color: "blue"
//                Label{
//                    anchors.centerIn: parent
//                    text: "S" + (index+1)
////                    color: "white"
//                    font.bold: true
//                }
//            }

//            Rectangle{
//                width: (listView.width / 12) - 5
//                height: 50
//                radius: 10
////                color: "blue"
//                Label{
//                    anchors.centerIn: parent
//                    text: "S" + (index+1)
////                    color: "white"
//                    font.bold: true
//                }
//            }

//            Rectangle{
//                width: (listView.width / 12) - 5
//                height: 50
//                radius: 10
////                color: "blue"
//                Label{
//                    anchors.centerIn: parent
//                    text: "S" + (index+1)
////                    color: "white"
//                    font.bold: true
//                }
//            }
//        }
//    }
}
