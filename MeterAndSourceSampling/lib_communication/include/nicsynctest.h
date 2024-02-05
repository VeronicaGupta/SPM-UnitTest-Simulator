#ifndef NICSYNCTEST_H
#define NICSYNCTEST_H

#include <QObject>
#include <QStateMachine>
#include <QFinalState>
#include <QByteArray>
#include <QDateTime>
#include <QTimer>

#include "metercommunication.h"
#include "pvstatemachine.h"
#include "meterkeys.h"
#include "meterinfo.h"
#include "pvstate.h"

class NicSyncTest : public MeterCommunication
{
    Q_OBJECT
public:
    explicit NicSyncTest(QString port, EnCustomer customer, QObject *parent = nullptr);

public slots:
    void setSerialToVerify(QString serial);
    void startTest(QString meterSerial);
    void setQRToVerify(QString qrCode, QString manufacturingDate);

private slots:
    void fetchVI();
    void setSerial();
    void checkSerial();
    void startAccumalationEnergy();//load test
    void stopAccumalationEnergy();
    void singleWireOffSet();//test
    void readRFParameter();
    QString validateRFKeyValidate(QByteArray packet);

    void processStateMachineStoped();
    void processFetchingVI(QByteArray packet);
    bool processSetSerial(QByteArray packet);
    void processStartAccumation(QByteArray packet);

    void processFetchInput(QByteArray packet);
    void processFetchOutput(QByteArray packet);

    void processAuthKey(QByteArray packet);
    void processHLSKey(QByteArray packet);
    void processEncryptKey(QByteArray packet);
    void processSingleWireOffSet(QByteArray packet);
    void processStopEnergyAccumulation(QByteArray packet);
    void processReadRFParameter(QByteArray packet);
    void printState();

    //input output
    void fetchIN();
    void fetchOUT();

    //RF
    void fetchAuth();
    void fetchEncry();
    void fetchHLS();
    void verifyQR();

protected slots:
    void packetReceived(QByteArray packet);

signals:
    void fetchNicSyncFail();
    void fetchNicSyncPass();
    void nicSycnTestResult(NicSyncTestResult*);

    void started();
    void fetchVIPass();
    void fetchVIFail();
    void fetchsetSerialNoPass();
    void fetchsetSerialNoFail();
    void setSerialVerifyPass();
    void setSerialVerifyFail();
    void startAccumalationPass();
    void startAccumalationFail();
    void fetchingRFKeyTestPass();
    void fetchingRFKeyTestFail();
    void verifySingleWireOffSetTestPass();
    void verifySingleWireOffSetTestFail();
    void stopEnergyAccumulationPass();
    void stopEnergyAccumulationFail();
    void readRFParametersPass();
    void readRFParametersFail();
    void verifyingQRPass();
    void verifyingQRFail();

    //
    void fetchingInputPass();
    void fetchingOutputPass();
    void finishInOutPass();
    void failedInOut();

    //RF
    void fetchingAuthPass();
    void fetchingHLSPass();
    void fetchingEncryPass();
    void failedRF();

private:
    NicSyncTestResult testResult;
    QString meterSerial;
    QDateTime time;
    QDateTime nicSyncStartTime;

    PVState *authenticating;
    PVState *fetchingVI;
    PVState *startAccumalation;
    PVState *fetchingInput;
    PVState *fetchingOutput;
    PVState *fetchingInputSecond;
    PVState *fetchingAuth;
    PVState *fetchingHLS;
    PVState *fetchingEncry;
    PVState *verifyingQR;
    PVState *stopEnergyAccumulation;
    PVState *setSerialNo;
    PVState *authenticating2;
    PVState *verifySingleWireOffSetTest;
    PVState *stReadRFParameter;
    PVStateMachine stateMachine;
    QFinalState *nicsyncTestFinish;
    QFinalState *failed;

    QSerialPort serialPort;
    MeterKeys* meter_keys;
    QString qrCode;
    QString serialPrefix;
    QString manufacturingDate;
};

#endif // NICSYNCTEST_H
