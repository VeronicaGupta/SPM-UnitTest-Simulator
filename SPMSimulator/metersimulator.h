#ifndef METERSIMULATOR_H
#define METERSIMULATOR_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QQueue>
#include <QTimer>

class MeterSimulator : public QTcpServer
{
    Q_OBJECT
public:
    explicit MeterSimulator(int portNumber, int testType = 0, QObject *parent = nullptr);
    void sendPacket(QTcpSocket* socket, QByteArray &data);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void sendPacket();
    void readClientData();

    void clientDisconnected();

private:
    QQueue<QByteArray> packets;
    QQueue<QTcpSocket*> sockets;

    QTimer* tmrSendPacket;
    int testType;
};

#endif // METERSIMULATOR_H
