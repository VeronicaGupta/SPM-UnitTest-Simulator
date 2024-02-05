#ifndef FUNCTIONTEST_H
#define FUNCTIONTEST_H

#include <QObject>
#include "metercommunication.h"
#include "meterinfo.h"
#include "pvstatemachine.h"
#include "pvstate.h"

class FunctionTest : public MeterCommunication
{
    Q_OBJECT
public:
    explicit FunctionTest(QString port, QObject *parent = nullptr);

public slots:
    void startTest();

    void fetchPCBNumber();
    void fetchRTCDrift();
    void setRTC();
    void fetchVI();
    void startFunctionalTest();

    bool processPCBNumber(QByteArray packet);
    bool processRTCDrift(QByteArray packet);
    bool processSetRTC(QByteArray packet);
    bool processFetchingVI(QByteArray packet);
    bool processFunctionTest(QByteArray packet);

    void processStateMachineStoped();

protected slots:
    void packetReceived(QByteArray packet);
    void printState();

signals:
    void functionTestResult(FunctionTestResult* result);

    void fetchPCBNumberPass();
    void fetchPCBNumberFail();
    void fetchRTCDriftPass();
    void fetchRTCDriftFail();
    void fetchRTCUpdateRTC();
    void setRTCPass();
    void setRTCFail();
    void fetchVIPass();
    void fatchVIFail();
    void functionTestPass();
    void functionTestFail();

private:
    FunctionTestResult testResult;
    QDateTime time;

    PVStateMachine* stateMachine;
    PVState *authenticating;
    PVState *authenticated;
    PVState *fetchingPCBNumber;
    PVState *fetchingRTCDrift;
    PVState *updateRTC;
    PVState *fetchingVI;
    PVState *doingFunctionTest;
    QFinalState *failed;
    QFinalState *functionTestFinish;

    QStringList functionTestID = {".coverClosed", ".pushButton", ".magnetStat1", ".magnetStat2", ".phaseCurrentOFF", ".neutralCurrentOFF", ".phaseCurrentON", ".neutralCurrentON = true"};

    int retryCounter;
};

#endif // FUNCTIONTEST_H
