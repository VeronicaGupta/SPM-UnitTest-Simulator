#ifndef METERCOMMUNICATION_H
#define METERCOMMUNICATION_H

#include <QObject>
#include <QSerialPort>

class QAESEncryption;
class QDataStream;
class QTimer;

typedef enum{
    RECEIVER_IDEAL,
    RECEIVER_HEADER_RECEIVED,
    RECEIVER_DATA_RECEIVED
}ReceiverSteate;

class MeterCommunication : public QObject
{
    Q_OBJECT
public:
    explicit MeterCommunication(QString portName, QObject *parent = nullptr);

    bool authenticate();
    bool sendPacket(QByteArray &packet);

signals:
    void packetReceived(QByteArray packet);
    void authenticated(bool success);

protected slots:
    void readData();

private:
    QByteArray createPacket(QByteArray data);

private:
    QSerialPort* serial;
    QAESEncryption* encrypt;
    QTimer* timer;
    ReceiverSteate receiverState;

    QByteArray receiverBuffer;
    QDataStream* receiverStream;
    quint8 dataToBeReceived;
};

#endif // METERCOMMUNICATION_H
