#ifndef PLCCONTROLLER_H
#define PLCCONTROLLER_H

#include <QObject>
#include <QTcpSocket>
#include <QStateMachine>
#include <QFinalState>
#include <QQueue>
#include <QSettings>

#include "spmfulltest.h"

class MeterInfo;

class PLCController : public QTcpSocket
{
    Q_OBJECT
public:
    explicit PLCController(QObject *parent = nullptr);

public slots:
    void startProduction(QString batchNumber, QString loaNumber2, QString laserLayout, QString printPrefix, int serialFrom, int serialTo);
    void startProduction();
    void pauseProduction();
    void resumeProduction();
    void stopProduction();
    void sendCommand(QString command);

    int getTotalMeterCount(){return meterToBeProduced;}

    QQueue<MeterInfo*> getMeterInfos(){return meterStageQueue;}
    MeterInfo* getMeterInfo(int index){ return meterStageQueue.at(index);}

    //tmp
    void printMeterInfo();

    void temp1();
    void temp2();

    void calibrate();

public:
    QQueue<MeterInfo*> meterStageQueue;

private slots:
    void testCompleted();
    QString getSpmString(MeterInfo &info, TestResult &testResult, QString failString);

    void processCommands(QString command);

    void readyRead();
    void socketConnectd();
    void sockedDisconnectd();

    void delay(int second);

signals:
    void commandReceived(QString command);
    void testStarted();

public slots:
    void moveNextStage();
    void startTest();
//    void startTest(int testsToRun);

    int getTmpSerial(int index);
    int getFinalSerial(int index);

private:
    QState *plcConnecting;
    QState *plcConnectingTimeout;
    QState *plcIndexing;
    QState *plcIndexingError;
    QState *plcStartTesting;
    QState *plcGetData;

    QFinalState *failed;

    QStateMachine* stateMachine;

    SPMFullTest* spmFullTest;
    QString lastCommand;
    bool isPaused;

    int currentSerial;
    int meterToBeProduced;
    int currentMeterCount;
    QString tag;

};

#endif // PLCCONTROLLER_H
