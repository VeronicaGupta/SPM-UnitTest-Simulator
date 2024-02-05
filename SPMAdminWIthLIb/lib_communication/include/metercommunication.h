#ifndef METERCOMMUNICATION_H
#define METERCOMMUNICATION_H

#include <QObject>
#include <QSerialPort>
#include <QDateTime>
#include "pvstatemachine.h"

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
    explicit MeterCommunication(QString portName, QString tag, QObject *parent = nullptr, bool blocking = false);
    ~MeterCommunication();

public slots:
    bool authenticate();
//    bool authenticateRetry();
    bool calibrate();
    void sendPacket(QByteArray &packet);
    void sendPacket();
    bool sendPacketWaitReply(QByteArray &packet);

    void openSerialPort();
    void closeSerialPort();

public:
    enum MEM_SELECT
    {
        MEM_SELECT_NONE,
        MEM_SELECT_ONCHIP,
        MEM_SELECT_EEPROM,
        MEM_SELECT_DATA_FLASH,
        MEM_SELECT_SERIAL_FLASH,
        MEM_SELECT_INSTANT,
    };
    static QByteArray createPacket(MEM_SELECT memory, QByteArray data);
    static QByteArray createPacket(QByteArray data, quint8 adjustPacketSize = 4);

    static void delay(int msec);

    void setupSerial();
    
signals:
    void packetReceived(QByteArray packet);
    void authenticationPass();
    void authenticationFail();
    void calibrationPass();
    void calibrationFail();

protected slots:
    virtual void readData();
    bool readPacket(QByteArray &data, int timeOut = 3000);
    void printState();
    void processAuthentication(QByteArray &packet);
    void processCalibrate(QByteArray &packet);

protected:
    QString tag;
    QDateTime startTime;

private:
    QSerialPort* serial;
    QAESEncryption* encrypt;
    QTimer* timer;
    ReceiverSteate receiverState;
    bool isAuthenticating;
    bool mBlocking;

    QDateTime mStartTime;
    QByteArray totalData;
    QByteArray receiverBuffer;
    QDataStream* receiverStream;
    quint8 dataToBeReceived;

    PVStateMachine* mStateMachine;
    QByteArray packet;
    QString portName;
};

#endif // METERCOMMUNICATION_H
