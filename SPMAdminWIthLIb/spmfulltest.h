#ifndef SPMFULLTEST_H
#define SPMFULLTEST_H

#include <QObject>
#include <QDateTime>
#include "functiontest.h"
#include "calibrationerrortest.h"
#include "metererrortest.h"
#include "meterinfo.h"
#include "metercalibration.h"
#include "lasercommunication.h"
#include "nicsynctest.h"


#define TEST_FUNCTION               (1<<0)
#define TEST_PHASE_CALIBRATION      (1<<3)
#define TEST_NEUTRAL_CALIBRATION    (1<<4)
#define TEST_STARTING_CURRENT       (1<<5)
#define TEST_LOW_CURRENT_ERROR      (1<<6)
#define TEST_HIGH_CURRENT_ERROR     (1<<7)
#define TEST_LASER_ENGRAVE          (1<<8)
#define TEST_NICSYNC                (1<<9)

class SPMFullTest : public QObject
{
    Q_OBJECT
public:
    explicit SPMFullTest(QObject *parent = nullptr);

    enum TEST_RESULT{
        TEST_STATUS_IDEAL,
        TEST_STATUS_NOT_APPLICABLE,
        TEST_STATUS_RUNNING,
        TEST_STATUS_PASS,
        TEST_STATUS_FAIL
    };
    typedef QList<TEST_RESULT> TestResults;

public slots:
    void startProduction(QString batchNumber, QString loaNumber2, QString laserLayout, QString printPrefix, int serialFrom, int serialTo);
    void startTest(MeterInfos &meterInfos,int testToStart);

    void processTestResult(); // n
    void powerStablised();
    void functionTestResult(FunctionTestResult* result);
    void phaseCalibrationTestResult(CalibrationResult* result);
    void neutralCalibrationTestResult(CalibrationResult* result);
    void startingCurrentTestResult(ErrorTestResult* result);
    void lowCurrentTestResult(ErrorTestResult *result);
    void highCurrentTestResult(ErrorTestResult* result);
    void laserEngraveResult(LaserEngraveResult* testResult);
    void nicSycnTestResult(NicSyncTestResult*testResult);

    int getProductionSerial(){return productionSerial;}
    QString getPrintPrefix(){return printPrefix;}

    void calibrate();

    void setQRCode(QString qrCode);

signals:
    void spmFullTestCompleted();

private:
    void setAllTestResult(TEST_RESULT result);

private:
    QString tag;
    PowerSource* powerSource;
    FunctionTest* functionTest;
//    MeterCalibration* neutralCalibration;
//    MeterCalibration* phaseCalibration;
    MeterErrorTest* lowCurrentErrorTest;
    MeterErrorTest* highCurrentErrorTest;
    CalibrationErrorTest* calibrationErrorTest;
    MeterErrorTest* startingCurrentTest;
    MeterCalibration* calibratePhase;
    MeterCalibration* calibrateNeutral;
    MeterErrorTest* startingCurrent;
    LaserCommunication* laserEngrave;
    NicSyncTest* nicSync;

    QDateTime startTestTime;

    TEST_RESULT functionTestStatus; //n
    TEST_RESULT neutralCalibrationStatus;
    TEST_RESULT phaseCalibrationStatus;
    TEST_RESULT startingCurrentTestStatus;
    TEST_RESULT lowCurrentErrorTestStatus;
    TEST_RESULT highCurrentErrorTestStatus;
    TEST_RESULT laserEngraveStatus;
    TEST_RESULT nicSyncTestStatus;

    int passCounter;
    int failCounter;

    int testToStart;
    MeterInfos* meterInfos;

    QString batchNumber;
    QString loaNumber2;
    QString laserLayout;
    QString printPrefix;
    QString manufacturingDate;
    int serialFrom;
    int serialTo;
    int currentSerial;
    int productionSerial;
    int meterToBeProduced;
    int currentMeterCount;
};

#endif // SPMFULLTEST_H
