#include "spmfulltest.h"
#include <QTimer>
#include <QDebug>
#include <QSettings>

//ToDo: Print at test end which test pass and fail
//ToDo: calculate and print cycle time of test
//ToDo: calculate and print each test cycle time
//ToDo: build stastics of each stage pass count


SPMFullTest::SPMFullTest(QObject *parent)
    : QObject{parent}
{
    QSettings setting;
    passCounter = 0;
    failCounter = 0;
    tag = "SPM";

    setAllTestResult(TEST_STATUS_IDEAL);

    powerSource = new PowerSource(setting.value("powerSourcePort").toString(), setting.value("cmriSourcePort").toString(), this);
    qDebug() << tag << " testing";
    powerSource->disablePowerOutput();
    connect(powerSource, SIGNAL(allChannelReady()), this, SLOT(powerStablised()));

    functionTest = new FunctionTest(setting.value("functionTestPort").toString(), this);
    calibratePhase = new MeterCalibration(setting.value("phaseCalibrationPort").toString(), MeterCalibration::CALIBRATE_PHASE, this, "CaLPhase");
    calibrateNeutral = new MeterCalibration(setting.value("neutralCalibrationPort").toString(), MeterCalibration::CALIBRATE_NEUTRAL, this, "CaLNeutral");
    startingCurrentTest = new MeterErrorTest(220, 0.067, 80, setting.value("errorTestStartingCurrentPort").toString(), this, "SC");
    lowCurrentErrorTest = new MeterErrorTest(240, 1, 0.5, setting.value("errorTestLowCurrentPort").toString(), this, "E1L");
    highCurrentErrorTest = new MeterErrorTest(240, 30, 0.5, setting.value("errorTestHighCurrentPort").toString(), this, "E2H");
    laserEngrave = new LaserCommunication(this);
#ifndef SIMULATOR_MODE
    nicSync = new NicSyncTest(setting.value("networkSycnPort").toString(), EnCustomer::CUST_INTELLI);
#else
    nicSync = new NicSyncTest(setting.value("networkSycnPort").toString(), EnCustomer::CUST_INTELLI, false);
#endif
    laserEngrave->connectLaser();

    connect(functionTest, SIGNAL(functionTestResult(FunctionTestResult*)), this, SLOT(functionTestResult(FunctionTestResult*)));
    connect(calibratePhase, SIGNAL(calibrationTestResult(CalibrationResult*)), this, SLOT(phaseCalibrationTestResult(CalibrationResult*)));
    connect(calibrateNeutral, SIGNAL(calibrationTestResult(CalibrationResult*)), this, SLOT(neutralCalibrationTestResult(CalibrationResult*)));
    connect(startingCurrentTest, SIGNAL(errorTestResult(ErrorTestResult*)), this, SLOT(startingCurrentTestResult(ErrorTestResult*)));
    connect(lowCurrentErrorTest, SIGNAL(errorTestResult(ErrorTestResult*)), this, SLOT(lowCurrentTestResult(ErrorTestResult*)));
    connect(highCurrentErrorTest, SIGNAL(errorTestResult(ErrorTestResult*)), this, SLOT(highCurrentTestResult(ErrorTestResult*)));
    connect(laserEngrave, SIGNAL(laserEngraveResult(LaserEngraveResult*)), this, SLOT(laserEngraveResult(LaserEngraveResult*)));
    connect(nicSync, SIGNAL(nicSycnTestResult(NicSyncTestResult*)), this, SLOT(nicSycnTestResult(NicSyncTestResult*)));

}

void SPMFullTest::startProduction(QString batchNumber, QString loaNumber2, QString laserLayout, QString printPrefix, int serialFrom, int serialTo)
{
    this->batchNumber = batchNumber;
    this->loaNumber2 = loaNumber2;
    this->laserLayout = laserLayout;
    this->printPrefix = printPrefix;
    this->serialFrom = serialFrom;
    this->serialTo = serialTo;
    this->meterToBeProduced = serialTo - serialFrom;
    currentMeterCount = 0;
    currentSerial = serialFrom;
    productionSerial = currentSerial;
    qDebug() << tag << "startProduction" << batchNumber << loaNumber2 << laserLayout << printPrefix << serialFrom << serialTo << meterToBeProduced;
}

void SPMFullTest::startTest(MeterInfos &meterInfos, int testToStart)
{
    this->testToStart = testToStart;
    this->meterInfos = &meterInfos;
    setAllTestResult(TEST_STATUS_IDEAL);

    startTestTime = QDateTime::currentDateTime();

//    QTimer::singleShot(4000, this, SLOT(powerStablised()));
//    powerSource->setChannelState(PowerSource::POWER_CHANNEL_1 | PowerSource::POWER_CHANNEL_2 | PowerSource::POWER_CHANNEL_3, true);
    int powerSourceEn = 0;
    if((testToStart & TEST_PHASE_CALIBRATION) || testToStart & TEST_NEUTRAL_CALIBRATION)
//    if(meterInfos.at(8)->isValid || meterInfos.at(7)->isValid)
        powerSourceEn |= PowerSource::POWER_CHANNEL_1;
    if(testToStart & TEST_LOW_CURRENT_ERROR)//meterInfos.at(5)->isValid)
        powerSourceEn |= PowerSource::POWER_CHANNEL_2;
    if(testToStart & TEST_HIGH_CURRENT_ERROR)
        powerSourceEn |= PowerSource::POWER_CHANNEL_3;

    if(testToStart & TEST_FUNCTION){
        functionTestStatus = TEST_STATUS_RUNNING;
        QTimer::singleShot(3000, functionTest, SLOT(startTest()));
    }
    if(testToStart & TEST_PHASE_CALIBRATION){
        phaseCalibrationStatus = TEST_STATUS_RUNNING;
    }
    if(testToStart & TEST_NEUTRAL_CALIBRATION){
        neutralCalibrationStatus  = TEST_STATUS_RUNNING;
    }
    if(testToStart & TEST_STARTING_CURRENT){
        startingCurrentTestStatus = TEST_STATUS_RUNNING;
    }
    if(testToStart & TEST_LOW_CURRENT_ERROR){
        lowCurrentErrorTestStatus = TEST_STATUS_RUNNING;
    }
    if((testToStart & TEST_HIGH_CURRENT_ERROR)){
        highCurrentErrorTestStatus = TEST_STATUS_RUNNING;
    }
    if((testToStart & TEST_LASER_ENGRAVE)){
        laserEngraveStatus = TEST_STATUS_RUNNING;
    }
    if((testToStart & TEST_NICSYNC)){
        nicSyncTestStatus = TEST_STATUS_RUNNING;
    }

    qDebug() << tag << "PowerSource setChannel" << powerSourceEn;
    powerSource->setChannelState(powerSourceEn, true);
}

void SPMFullTest::powerStablised()
{
    qDebug() << tag << "PowerStablised duration " << startTestTime.secsTo(QDateTime::currentDateTime());

    if(testToStart & TEST_PHASE_CALIBRATION){
        QTimer::singleShot(3000, calibratePhase, SLOT(calibrate()));
    }
    if(testToStart & TEST_NEUTRAL_CALIBRATION){
        QTimer::singleShot(3000, calibrateNeutral, SLOT(calibrate()));
    }
    if(testToStart & TEST_STARTING_CURRENT){
        startingCurrentTest->startErrorTest();
    }
    if(testToStart & TEST_LOW_CURRENT_ERROR){
        lowCurrentErrorTest->startErrorTest();
    }
    if((testToStart & TEST_HIGH_CURRENT_ERROR)){
        highCurrentErrorTest->startErrorTest();
    }
    if((testToStart & TEST_LASER_ENGRAVE)){
        auto* info = meterInfos->at(3);
        info->meterFinalSerialNumber = productionSerial++;
        manufacturingDate = QDateTime::currentDateTime().toString("MM/ yyyy");
        laserEngrave->startMeterEngraving(batchNumber, loaNumber2, manufacturingDate, printPrefix + QString("%1").arg(info->meterFinalSerialNumber, 4, 10, QLatin1Char('0')));
    }
    if((testToStart & TEST_NICSYNC)){
        auto info = meterInfos->at(2);
        nicSync->startTest(QString("%1%2").arg(info->meterSerialPrefix).arg(info->meterFinalSerialNumber));
    }

    processTestResult();
}

void SPMFullTest::functionTestResult(FunctionTestResult *result)
{
//    MeterInfo info, info1;
    MeterInfo* info = meterInfos->at(11);

    info->setFunctionTestResult(*result);

    functionTestStatus = result->isTestPass ? TEST_STATUS_PASS : TEST_STATUS_FAIL;

    qDebug() << tag << "functionTestResult" << result->isTestPass;
    qDebug() << tag << info->functionTestResult.isTestPass << info->functionTestResult.voltage;

    processTestResult();
}

void SPMFullTest::phaseCalibrationTestResult(CalibrationResult *result)
{
    MeterInfo* info = meterInfos->at(8);

    info->setPhaseCalibrationTestResult(*result);

    phaseCalibrationStatus = result->isTestPass ? TEST_STATUS_PASS : TEST_STATUS_FAIL;

    qDebug() << tag << "phaseCalibrationTestResult" << result->isTestPass;
    qDebug() << tag << info->phaseCalibrationResult.isTestPass;

    processTestResult();
}

void SPMFullTest::neutralCalibrationTestResult(CalibrationResult *result)
{
    MeterInfo* info = meterInfos->at(7);

    info->setNeutralCalibrationTestResult(*result);

    neutralCalibrationStatus = result->isTestPass ? TEST_STATUS_PASS : TEST_STATUS_FAIL;

    qDebug() << tag << "neutralCalibrationTestResult" << result->isTestPass;
    qDebug() << tag << info->neutralCalibrationResult.isTestPass;

    processTestResult();
}

void SPMFullTest::startingCurrentTestResult(ErrorTestResult *result)
{
    MeterInfo* info = meterInfos->at(6);
//    ErrorTestResult info1;

    qDebug() << tag << " ????????????????? " << result->meterEnergyWH << result->isTestPass;
//    result->isTestPass = true;

    info->setStartingCurrentTestResult(*result);

    startingCurrentTestStatus = result->isTestPass ? TEST_STATUS_PASS : TEST_STATUS_FAIL;

    qDebug() << tag << "startingCurrentTestResult" << result->isTestPass;
    qDebug() << tag << info->startingCurrentTestResult.isTestPass;

    processTestResult();
}

void SPMFullTest::lowCurrentTestResult(ErrorTestResult *result)
{
    MeterInfo* info = meterInfos->at(5);

    info->setLowCurrentTestResult(*result);

    lowCurrentErrorTestStatus = result->isTestPass ? TEST_STATUS_PASS : TEST_STATUS_FAIL;

    qDebug() << tag << "lowCurrentTestResult" << result->isTestPass;
    qDebug() << tag << info->lowCurrentTestResult.isTestPass;

    processTestResult();
}

void SPMFullTest::highCurrentTestResult(ErrorTestResult *result)
{
    MeterInfo* info = meterInfos->at(4);

    info->setHighCurrentTestResult(*result);

    highCurrentErrorTestStatus = result->isTestPass ? TEST_STATUS_PASS : TEST_STATUS_FAIL;

    qDebug() << tag << "highCurrentTestResult" << result->isTestPass;
    qDebug() << tag << info->highCurrentTestResult.isTestPass;

    processTestResult();
}

void SPMFullTest::laserEngraveResult(LaserEngraveResult *testResult)
{
    MeterInfo* info = meterInfos->at(3);
    testResult->isTestPass = true;

    qDebug() << tag << "R" << " 1 testResult->isTestPass" << testResult->isTestPass;
    info->setLaserEngraveResult(*testResult);

    laserEngraveStatus = testResult->isTestPass ? TEST_STATUS_PASS : TEST_STATUS_FAIL;

    qDebug() << tag << "laserEngraveResult" << testResult->isTestPass;
    qDebug() << tag << info->laserEngraveResult.isTestPass;

    QSettings setting;
    setting.setValue("currentSerial", productionSerial);
    setting.sync();

    processTestResult();
}

void SPMFullTest::nicSycnTestResult(NicSyncTestResult *testResult)
{
    MeterInfo* info = meterInfos->at(2);

//    testResult->isTestPass = true;

    info->setNicSyncTestResult(*testResult);

    nicSyncTestStatus = testResult->isTestPass ? TEST_STATUS_PASS : TEST_STATUS_FAIL;

    qDebug() << tag << "nicSycnTestResult" << testResult->isTestPass;
    qDebug() << tag << info->laserEngraveResult.isTestPass;

    processTestResult();
}

void SPMFullTest::calibrate()
{
    calibratePhase->calibrate();
}

void SPMFullTest::setQRCode(QString qrCode)
{
    nicSync->setQRToVerify(qrCode, manufacturingDate);
}

void SPMFullTest::processTestResult()
{
    qDebug() << tag <<  functionTestStatus << phaseCalibrationStatus << neutralCalibrationStatus << startingCurrentTestStatus << lowCurrentErrorTestStatus << highCurrentErrorTestStatus << laserEngraveStatus << nicSyncTestStatus;
    if(functionTestStatus == TEST_STATUS_RUNNING || phaseCalibrationStatus == TEST_STATUS_RUNNING || neutralCalibrationStatus == TEST_STATUS_RUNNING || startingCurrentTestStatus == TEST_STATUS_RUNNING
            || lowCurrentErrorTestStatus == TEST_STATUS_RUNNING || highCurrentErrorTestStatus == TEST_STATUS_RUNNING || laserEngraveStatus == TEST_STATUS_RUNNING || nicSyncTestStatus == TEST_STATUS_RUNNING)
        return;

    qDebug() << tag << "duration" << startTestTime.msecsTo(QDateTime::currentDateTime());
    powerSource->disablePowerOutput();

    emit spmFullTestCompleted();
}

void SPMFullTest::setAllTestResult(TEST_RESULT result)
{
    functionTestStatus = result;
    neutralCalibrationStatus = result;
    phaseCalibrationStatus = result;
    startingCurrentTestStatus = result;
    lowCurrentErrorTestStatus = result;
    highCurrentErrorTestStatus = result;
    laserEngraveStatus = result;
    nicSyncTestStatus = result;
}
