#ifndef METERSTARTINGCURRENTTEST_H
#define METERSTARTINGCURRENTTEST_H

#include <QObject>
#include "metererrortest.h"

class MeterStartingCurrentTest : public MeterErrorTest
{
    Q_OBJECT
public:
    explicit MeterStartingCurrentTest(float voltage, float current, float tolerance, QString serialPort, QObject *parent = nullptr, QString tag = "");

    void processStopEnergyAccumulation(QByteArray &packet) override;

signals:

};

#endif // METERSTARTINGCURRENTTEST_H
