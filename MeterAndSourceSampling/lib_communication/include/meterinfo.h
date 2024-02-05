#ifndef METERINFO_H
#define METERINFO_H

#include <QObject>
#include <QVariant>
#include <QQueue>
#include <QDateTime>

class TestResult{

public:
    explicit TestResult();
    void resetValue();

public:
    QString log;
    QDateTime startTestTime;
    QDateTime endTestTime;
    int testDuration;

    virtual QVariantMap getResult();

//protected:
    bool isTestPass;
    QString lastState;
};

class FunctionTestResult: public TestResult{
public:
    FunctionTestResult(){};
    void resetValue();
    QVariantMap getResult();

    typedef struct {
        unsigned coverClosed:1;
        unsigned pushButton:1;
        unsigned magnetStat1:1;
        unsigned magnetStat2:1;
        unsigned phaseCurrentOff:1;
        unsigned neutralCurrentOff:1;
        unsigned phaseCurrentOn:1;
        unsigned neutralCurrentOn:1;
    }FunctionTestInfo;

    FunctionTestInfo result;
    int pcbNumber;
    double voltage; //must be double
    double phaseCurrent;
    double neutralCurrent;
    int rtcDriftSecond;
};

class CalibrationResult: public TestResult{
public:
    CalibrationResult(){}
    QVariantMap getResult();

};

class ErrorTestResult: public TestResult{
public:
    ErrorTestResult();
    void resetValue();
    QVariantMap getResult();

    float meterEnergyWH;
    float startingCumlativeEnergy;
    float phaseEnergyVAH;
    float neutralEnergyVAH;
    float phaseReactiveEnergy;
    float neutralReactiveEnergy;
    float phaseWattHour;
    float neutralWattHour;
    float cumulativeEnergy;
    float testVoltage;
    float testCurrent;
    float theoryEnergy;
    float powerWH;
    uint32_t timeMS;
};

class LaserEngraveResult: public TestResult{
public:
    LaserEngraveResult();
    void resetValue();
    QVariantMap getResult();

    QString serialNumber;
    QString loaNumber;
    QString loaNumber2;
    QString mfgDate;
};

class NicSyncTestResult: public TestResult{
public:
    QVariantMap getResult();
    void resetValue();

    QString meterMemoryNumber;
    QString meterNumberPrint;
    QString meterQRNumber;
    double noLoadPhaseCurrent;
    double noLoadNeutralCurrent;

    qint32 networkAddress;
    quint8 networkChannel;

    QString rfKey;
};

class MeterInfo : public QObject
{
    Q_OBJECT
public:
    explicit MeterInfo(bool isValid=true, QObject *parent = nullptr);

public slots:
    int getTmpSerial(){return meterTmpSerialNumber;}
    int getFinalSerial(){return meterFinalSerialNumber;}
signals:

public:
    int meterTmpSerialNumber;
    int meterFinalSerialNumber;
    QString meterSerialPrefix;

    QString meterID;
    QString batchID;
    QString currentBatchSequence;
    QString pcbNumber;
    QString replayBatchNumber;
    QString meterNumber;
    QString nicNumber;
    QString sealNumber;
    QString status;
    QString deviceErrorAt;
    QString errorCode;
    QString tenantID;
    QString creationTime;
    QString manufacturingOrderNumber;
    QString loaNumber1;
    QString loaNumber2;
    QString prefixJA;

    void setFunctionTestResult(FunctionTestResult &functionTestResult);
    void setMeggarTestResult(TestResult &testResult);
    void setHVTestResult(TestResult &testResult);
    void setPhaseCalibrationTestResult(CalibrationResult &calibration);
    void setNeutralCalibrationTestResult(CalibrationResult &calibration);
    void setStartingCurrentTestResult(ErrorTestResult errorTest);
    void setLowCurrentTestResult(ErrorTestResult errorTest);
    void setHighCurrentTestResult(ErrorTestResult errorTest);
    void setLaserEngraveResult(LaserEngraveResult testResult);
    void setNicSyncTestResult(NicSyncTestResult nicSyncTest);
    void setDLMSTestResult(NicSyncTestResult nicSyncTest);

    void saveLogs();

    FunctionTestResult functionTestResult;          //Test1
    TestResult hvTestResult;                        //Test2
    TestResult megarTestResult;                     //Test3
    CalibrationResult phaseCalibrationResult;             //Test4
    CalibrationResult neutralCalibrationResult;           //Test5
    ErrorTestResult startingCurrentTestResult;      //Test6
    ErrorTestResult lowCurrentTestResult;           //Test7
    ErrorTestResult highCurrentTestResult;          //Test8
    LaserEngraveResult laserEngraveResult;          //Test9
    NicSyncTestResult nicSyncTestResult;            //Test10
                                                    //Test11 Rejection
    QVariant gluingResult;                          //Test12

    bool isValid;
    bool isPass;
    QString failedAt;
};

typedef QQueue<MeterInfo*> MeterInfos;

#endif // METERINFO_H
