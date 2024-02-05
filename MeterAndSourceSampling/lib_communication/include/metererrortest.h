#ifndef METERERRORTEST_H
#define METERERRORTEST_H

#include <QObject>
#include <QStateMachine>
#include <QFinalState>
#include <QDateTime>
#include <QTimer>

#include "pvstatemachine.h"
#include "metercommunication.h"
#include "meterinfo.h"
#include "pvstate.h"

#define METER_ERROR_TEST_HOUR    (60)
#define METER_ERROR_TEST_MINUTE  (60)

class MeterErrorTest : public MeterCommunication
{
    Q_OBJECT
public:
    explicit MeterErrorTest(float voltage, float current, float tolerance, QString serialPort, QObject *parent = nullptr, QString tag = "");

public slots:
    void startErrorTest();
    void startEnergyAccumulation();
    void stopEnergyAccumulation();

    void printState();

protected slots:
    void packetReceived(QByteArray packet);
    void processStateMachineStoped();

protected:
    void processStartEnergyAccumulation(QByteArray &packet);
    virtual void processStopEnergyAccumulation(QByteArray &packet);

    float round(float value, int roundFactor);

signals:
    void startEnergyAccumulationSuccess();
    void startEnergyAccumulationFail();
    void stopEnergyAccumulationSuccess();
    void stopEnergyAccumulationFail();
    void energyCalculatingPass();
    void energyCalculatingFail();


//    void errorTestPass();
//    void errorTestFail();
    void errorTestResult(ErrorTestResult* testResult);

protected:
    ErrorTestResult testResult;
    float testVoltage;
    float testCurrent;
    float tolerance;
    float startingCumlativeEnergy;

private:
    MeterCommunication* meterComm;
    QTimer* tmrStopEnergyAccumulation;
    PVStateMachine* stateMachine;
    PVState *authenticating;
    PVState *startEnergyAccu;
    PVState *energyAccumulating;
    PVState *stopEnergyAccu;
    PVState *energyCalculating;
    PVState *timeOut;
    QFinalState *failed;
    QFinalState *errorTestFinish;

    QDateTime time;
    int stopEnergyAccumulationRetry;
};

#endif // METERERRORTEST_H
