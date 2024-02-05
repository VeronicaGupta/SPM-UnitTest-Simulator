#ifndef POWERSOURCE_H
#define POWERSOURCE_H

#include <QTimer>
#include <QObject>
#include <QVector>
#include <QDateTime>
#include "pvstatemachine.h"
#include "cmricontrol.h"

class QSerialPort;
class PowerChannel{
public:
    float voltage;
    float current;
    bool enable;
};

class PowerSource : public QObject
{
    Q_OBJECT
public:
    explicit PowerSource(QString portName, QString cmriPortName = QString(), QObject *parent = nullptr);
    ~PowerSource();

    enum COMM_STATE
    {
        COMM_STATE_IDEAL,
        COMM_STATE_VOLTAGE,
        COMM_STATE_CURRENT,
        COMM_STATE_TIMEOUT,
        COMM_STATE_SERIAL_ERROR
    };

    enum POWER_CHANNEL
    {
        POWER_CHANNEL_1 = (1<<0),
        POWER_CHANNEL_2 = (1<<1),
        POWER_CHANNEL_3 = (1<<2)
    };

public slots:
    int setChannelState(quint8 channelNumber, bool state);
    void printState();

public slots:
    void processStabliseTimeout();

protected:
    bool setState(COMM_STATE newState);
    void validateCurrent(QString current);

public slots: //suppose to be private
    void readData();
    int getSerial();
    int setVoltage(int channel, float voltage);
    int setCurrent(int channel, float current);
    int setVoltage(float channel1Voltage, float channel2Voltage, float channel3Voltage);
    int setCurrent(float channel1Current, float channel2Current, float channel3Current);
    void setDefaultVoltageCurrent();

    int enablePowerOutput();
    int disablePowerOutput();
    void restartPowerOutput();
    int checkCurrent();
    int checkVoltage();

    int sendCommand(QString command);

signals:
    void validateCurrentStart();
    void validateCurrentPass();
    void validateCurrentFail();
    void allChannelReady();

    void currentReceived(QString current);
    void voltageReceived(QString voltage);

private:
    QSerialPort* serial;
    QString receiverBuffer;
    QString tag;
    COMM_STATE commState;
    PowerChannel powerSource[3];

    QTimer* tmrRetry;
    QTimer* tmrForceRead;
    QTimer* tmrStablisingTimeout;
    QTimer* tmrStablised;

    QDateTime startTime;

    PVStateMachine* stabliseMachine;
    quint8 activeChannel;

    CMRIControl* cmriControl;
    int stablisationCounter;
};

#endif // POWERSOURCE_H
