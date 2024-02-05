#include "powersourcesimulator.h"

PowerSourceSimulator::PowerSourceSimulator(int portNumber, QObject *parent) : QTcpServer(parent)
{

    if (!listen(QHostAddress::Any, portNumber)) {
        qWarning() << "Failed to start server: " << errorString();
        return;
    }

    qDebug() << QString("PowerSourceSimulator listening on port %1...").arg(portNumber);
}

void PowerSourceSimulator::incomingConnection(qintptr socketDescriptor)
{
    // A new client connection is incoming

    QTcpSocket* clientSocket = new QTcpSocket(this);
    if (clientSocket->setSocketDescriptor(socketDescriptor)){
        // Client connected successfully
        qDebug() << "New client connected: " << clientSocket->peerAddress().toString();

        connect(clientSocket, &QTcpSocket::readyRead, this, &PowerSourceSimulator::readClientData);
        connect(clientSocket, &QTcpSocket::disconnected, this, &PowerSourceSimulator::clientDisconnected);
    } else {
        qWarning() << "Failed to set socket descriptor for a new client";
        delete clientSocket;
    }
}

void PowerSourceSimulator::readClientData()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket)
        return;

    QByteArray data = clientSocket->readAll();
    qDebug() << "Received data from client: " << data << "on" << serverPort();

    if(data.startsWith("MEAS:POW:AC:ACT?"))
    {
        if(psCounter==0)
            sendData(clientSocket, QByteArray("0.0001718388, 0.0001634672, 0.00158152\r\n"));
        else if (psCounter%2==0)
            sendData(clientSocket, QByteArray("2400.000000035, 240.0000000092, 7200.0000000067\r\n"));
        else if (psCounter%2!=0)
            sendData(clientSocket, QByteArray("2400.000000035, 240.0000000092, 14400.0000000067\r\n"));
        psCounter++;
    }

    if (psCounter>1000){psCounter=0;}
}



void PowerSourceSimulator::clientDisconnected()
{
    QTcpSocket* clientSocket = qobject_cast<QTcpSocket*>(sender());
    if (!clientSocket)
        return;

    qDebug() << "Client disconnected: " << clientSocket->peerAddress().toString();

    // Clean up resources
    clientSocket->deleteLater();
}

void PowerSourceSimulator::sendData(QTcpSocket* clientSocket, QByteArray data)
{
    qDebug() << "PowerSourceSimulator::sendData" << QString::fromStdString(data.toStdString()) << "at" << psCounter << "tries";
    clientSocket->write(data);
}
