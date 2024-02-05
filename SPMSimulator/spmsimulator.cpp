#include "spmsimulator.h"
#include <QTimer>
#include <QDebug>

#define pDebug PMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).debug
#define pvDebug QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).debug

SPMSimulator::SPMSimulator(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &SPMSimulator::newConnection);

    if (server->listen(QHostAddress::Any, 55540)) {
        qDebug() << "Server is listening on port 55540";
    } else {
        qDebug() << "Server could not start!";
    }


    pvDebug() << "Hello" << "World";
    pDebug() << "Hello" << "World";

    for(int i=0; i<11; i++){
        runTestQueue << 0;
    }

//    PMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).debug("%d", 10001);
}

void SPMSimulator::newConnection()
{

    QTcpSocket *client = server->nextPendingConnection();
    connect(client, &QTcpSocket::readyRead, this, &SPMSimulator::readyRead);
    connect(client, &QTcpSocket::disconnected, client, &SPMSimulator::deleteLater);

    clients.clear();
    clients.append(client);
    qDebug() << "New client connected";

}

void SPMSimulator::readyRead()
{

    for (QTcpSocket *client : clients) {
        if (client->bytesAvailable() > 0) {
            QByteArray data = client->readAll();
            qDebug() << "Received data from client:" << data;

            if(data.startsWith("Start_of_Line")){
                client->write("Ack_Start_of_Line");
                initTestQueue();
                QTimer::singleShot(2000, this, SLOT(sendStartTest()));
            }
            else if(data == "Get_data"){
                client->write("Get_data_V");
            }
            else if(data.startsWith("Get_QR_data")){
                client->write("Get_QR_data");
            }
            else if(data.startsWith("Part_Result_Status_")){
                QString tests;
                auto status = data.split('_').last();

                QByteArray newStatus = status;
                if(lastStatus.length() > 0 && lastStatus.at(0) != '-')
                    newStatus[1] = lastStatus[0];

                if(lastStatus.length() > 1 && lastStatus.at(1) != '-')
                    newStatus[2] = lastStatus[1];

                qDebug() << QString(newStatus);


                runTestQueue << 1;
                for(int i=0; i<12;i ++){
                    tests += runTestQueue.at(11 - i) ? "P" : "-";
                }

//                data.split()
                client->write(QString("Part_Result_update  " + QString(newStatus)).toStdString().c_str());
                qDebug() << "Part_Result_update" << status << lastStatus << newStatus;
                lastStatus = newStatus;
            }
            else if(data.startsWith("Part_Result_Read_Success")){
//                runTestQueue << 1;
                QString tests;
                for(int i=0; i<12;i ++)
                    tests += QString("%1").arg(runTestQueue.at(11 - i));

                auto data = QString("Start_test_" + tests);
                client->write(data.toStdString().c_str());
                qDebug() << "sendintStartTest values" << data;

                runTestQueue.dequeue();

//                client->write("Part_Result_update");
            }
//            else if(data.startsWith("Start_of_Line")){
//                client->write("Ack_Start_of_Line");
//            }

        }
    }

}

void SPMSimulator::sendStartTest()
{
    clients.at(0)->write("Start_test");
}

void SPMSimulator::initTestQueue()
{
    runTestQueue.clear();
    for(int i=0; i<11; i++){
        runTestQueue << 0;
    }
}

//void PMessageLogger::debug(const char *msg, ...) const
//{
////    Q_ATTRIBUTE_FORMAT_PRINTF(2, 3);
//}

//QDebug PMessageLogger::debug(CategoryFunction catFunc) const
//{
//    qDebug() << "debug";
//}

QDebug PMessageLogger::debug() const
{
//    QDebug dbg = QDebug(QtDebugMsg);
//    QMessageLogContext &ctxt = dbg.stream->context;
//    ctxt.copyContextFrom(context);
//    return dbg;
    return QMessageLogger::debug();
//    qDebug() << " WOrkd";
}

QDebug PMessageLogger::debug(CategoryFunction catFunc) const
{
    qDebug() << " WOrkd";
}
