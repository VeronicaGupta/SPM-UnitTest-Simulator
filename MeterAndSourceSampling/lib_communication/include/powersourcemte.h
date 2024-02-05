#ifndef MTEPOWERSOURCE_H
#define MTEPOWERSOURCE_H

#include <QTimer>
#include <QObject>
#include <QVector>
#include <QDateTime>
#include "pvstatemachine.h"
#include "cmricontrol.h"
#include "QDateTime"

class PowerSourceMTE : public QObject
{
    Q_OBJECT
public:
    explicit PowerSourceMTE(QString portName, QObject *parent = nullptr);

public slots:
    void readData();
    void validateCurrent(QString current);
    void sendCommand(QString command);

    void setVoltage(int channel, float voltage);
    void setCurrent(int channel, float current);
//    int setVoltage(float channel1Voltage, float channel2Voltage, float channel3Voltage);
//    int setCurrent(float channel1Current, float channel2Current, float channel3Current);
    void setDefaultVoltageCurrent();

    void enablePowerOutput();


signals:
    void currentReceived(QString current);

private:
    QSerialPort* serial;
    QString receiverBuffer;
    QString tag;
    QDateTime startTestTime;

};

#endif // MTEPOWERSOURCE_H
