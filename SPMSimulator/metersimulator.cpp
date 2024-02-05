#include "metersimulator.h"

MeterSimulator::MeterSimulator(int portNumber, int testType, QObject *parent) : QTcpServer(parent)
{
    if (!listen(QHostAddress::Any, portNumber)) {
        qWarning() << "Failed to start server: " << errorString() << portNumber;
        return;
    }

    tmrSendPacket = new QTimer(this);
    tmrSendPacket->setSingleShot(true);
    tmrSendPacket->setInterval(100);

    connect(tmrSendPacket, SIGNAL(timeout()), this, SLOT(sendPacket()));

    qDebug() << QString("MeterSimulator listening on port %1...").arg(portNumber);
}

void MeterSimulator::sendPacket(QTcpSocket *socket, QByteArray &data)
{
    packets << data;
    sockets << socket;

    tmrSendPacket->start();
//    QTimer::singleShot(1000, this, SLOT(sendPacket()));
}

void MeterSimulator::sendPacket()
{
    QByteArray data;
    QTcpSocket *socket;

    if(packets.count() <= 0)
        return;

    data = packets.dequeue();
    socket = sockets.dequeue();

    qDebug() << "sendingPacket" << data.toHex() << socket->localPort() << socket->peerPort();
    socket->write(data);

    tmrSendPacket->start();
//    QTimer::singleShot(1000, this, SLOT(sendPacket()));
}

void MeterSimulator::incomingConnection(qintptr socketDescriptor)
{
    // A new client connection is incoming

    QTcpSocket* clientSocket = new QTcpSocket(this);
    if (clientSocket->setSocketDescriptor(socketDescriptor)) {
        // Client connected successfully
        qDebug() << "New client connected: " << clientSocket->peerAddress().toString();

        connect(clientSocket, &QTcpSocket::readyRead, this, &MeterSimulator::readClientData);
        connect(clientSocket, &QTcpSocket::disconnected, this, &MeterSimulator::clientDisconnected);
    } else {
        qWarning() << "Failed to set socket descriptor for a new client";
        delete clientSocket;
    }
}

void MeterSimulator::readClientData()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket){
        qDebug() << "Not Client Socket Returning";
        return;
    }

    QByteArray data = clientSocket->readAll();
    qDebug() << "Received data from client: " << data.toHex() << "on" << serverPort();
//    qDebug() << "Data " << QString::fromStdString(data.toStdString());


    //*** Function Test ****
    //authenticate
    if(data == QByteArray::fromHex("020124009b26e2a929fd3dfeebd28ac8a7477970553bb206f75541a7f5598d41401da638530103")){
        auto data = QByteArray::fromHex("02030600000188a803");
        sendPacket(clientSocket, data);
    }
    //getPCBNumber
    if(data == QByteArray::fromHex("02010f0501050000000b0400000000ab9703")){
        auto data = QByteArray::fromHex("02030a05000100000000bad403");
        sendPacket(clientSocket, data);
    }
    //getRTCDrift
    if(data == QByteArray::fromHex("0201140501050000000309000000000000000000e25603")){
        auto data = QByteArray::fromHex("02030f050001545116300110202300c20e03");
        sendPacket(clientSocket, data);
    }
    //setRTC
    if(data.startsWith(QByteArray::fromHex("020113050005000000"))){
        auto data = QByteArray::fromHex("020306050001b11503");
        sendPacket(clientSocket, data);
    }
    //getVI
    if(data == QByteArray::fromHex("020117050105000000010c000000000000000000000000fea603") && clientSocket->localPort()!=6006){
        auto data = QByteArray::fromHex("0203120500012b786f43a60ade3f39b4e83ff50903");
        sendPacket(clientSocket, data);
    }

    //FunctionTest
    if(data == QByteArray::fromHex("02010c050105000000150100250403")){
        auto data = QByteArray::fromHex("02030705000100bca103");
        sendPacket(clientSocket, data);
    }

    //*** Calibration Test ****
    //calibration
    if(data == QByteArray::fromHex("0201130132324270000043700000412000000069a403") || data == QByteArray::fromHex("020113013232427000004370000041200000035b3f03")){
        auto data = QByteArray::fromHex("0203270100024573f0000000000000000000000000000000000000000000000000000000000001857803");
        sendPacket(clientSocket, data);

        data = QByteArray::fromHex("0203270100024573f0004180000000000000000000000000000000000000000000000000000002d07903");
        sendPacket(clientSocket, data);

        data = QByteArray::fromHex("0203270100024573f0004180000000000000454ba4604746e24d4d1e3527000000000000000003330a03");
        sendPacket(clientSocket, data);

        data = QByteArray::fromHex("0203270100004573f0004180000000000000454ba4604746e24d4d1e3527bf389b7e0000000004c79c03");
        sendPacket(clientSocket, data);
    }

    //startEnergyAccumulation
    if(data == QByteArray::fromHex("02010c05000500000016010ae48503")){
        auto data = QByteArray::fromHex("020306050001b11503");
        sendPacket(clientSocket, data);
    }

    //stopEnergyAccumulation
    if(data == QByteArray::fromHex("02012b0501050000001720000000000000000000000000000000000000000000000000000000000000000056e003")){
        if (clientSocket->localPort()==6004){
            auto data = QByteArray::fromHex("0203260500015aaf2a3f45792a3f102700002020533f9eb32a3f127a2a3f0000000000000000c3d203");
            sendPacket(clientSocket, data);
         }
        if (clientSocket->localPort()==6005){
            auto data = QByteArray::fromHex("02032605000109ff9f41abad9f4110270000debbc84111ff9f41c5ad9f412697a33b93c8f23c1e4d03");
            sendPacket(clientSocket, data);
        }
    }

    //nicsync
    if (data == QByteArray::fromHex("02011b050105000000061000000000000000000000000000000000895003")){
        auto data = QByteArray::fromHex("02031605000141555448454e4b45595f494e54454c49990303");//
        sendPacket(clientSocket, data);
    }
    if (data == QByteArray::fromHex("02011b05010500000007100000000000000000000000000000000071b703")){
        auto data = QByteArray::fromHex("020316050001484c534b45595f494e54454c4c493031c2a003");//
        sendPacket(clientSocket, data);
    }
    if (data == QByteArray::fromHex("02011b050105000000051000000000000000000000000000000000886803")){
        auto data = QByteArray::fromHex("02031605000141555448454e4b45595f494e54454c49990303");
        sendPacket(clientSocket, data);
    }
    if (data == QByteArray::fromHex("02011f05010500000017140000000000000000000000000000000000000000f45a03")){
        auto data = QByteArray::fromHex("02031a0500010000000000000000000000007363954200000000935203");
        sendPacket(clientSocket, data);
    }
    if (data == QByteArray::fromHex("02012c0501050000000d2100000000000000000000000000000000000000000000000000000000000000000093a003")){
        auto data = QByteArray::fromHex("0203270500010a0941533233343830393600352d3000035246454e43525950545f494e54454c49186d03");
        sendPacket(clientSocket, data);
    }
    //get VI nicsync
    if(data == QByteArray::fromHex("020117050105000000010c000000000000000000000000fea603") && clientSocket->localPort()==6006){
        auto data = QByteArray::fromHex("0203120500018a7e90430000000000000000d9ca03");
        sendPacket(clientSocket, data);
    }
    if (data == QByteArray::fromHex("02011305010500000016080000000000000000cc0b03")){
        auto data = QByteArray::fromHex("020306050002838e03");
        sendPacket(clientSocket, data);
    }
    if (data == QByteArray::fromHex("02010603080b10de03")){
        auto data = QByteArray::fromHex("02030603000167cc03");
        sendPacket(clientSocket, data);
    }
    if (data == QByteArray::fromHex("02010603080c646103")){
        auto data = QByteArray::fromHex("02030603000167cc03");//
        sendPacket(clientSocket, data);
    }
    if (data == QByteArray::fromHex("02010603080b10de03")){
        auto data = QByteArray::fromHex("02030603000167cc03");
        sendPacket(clientSocket, data);
    }





}

void MeterSimulator::clientDisconnected()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket)
        return;

    qDebug() << "Client disconnected: " << clientSocket->peerAddress().toString();

    // Clean up resources
    clientSocket->deleteLater();
}
