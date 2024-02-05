#include "calibrationautotest.h"
#include <QSettings>
#include <QDebug>
#include <QTimer>

CalibrationAutoTest::CalibrationAutoTest(QObject *parent)
    : QObject{parent}
{
    QSettings setting;

    powerSource = new PowerSource(setting.value("powerSourcePort").toString(), QString(), this);
    powerSource->setVoltage(240, 240, 240);
    powerSource->setCurrent(10, 10, 10);
    powerSource->disablePowerOutput();
//    powerSource->enablePowerOutput();

    phaseCalibration = new MeterCalibration(setting.value("phaseCalibrationPort").toString(), MeterCalibration::CALIBRATE_PHASE, this, "M1");
    neutralCalibration = new MeterCalibration(setting.value("neutralCalibrationPort").toString(), MeterCalibration::CALIBRATE_NEUTRAL, this, "M2");
    bothCalibrationE1 = new MeterCalibration(setting.value("errorTestLowCurrentPort").toString(), MeterCalibration::CALIBRATE_PHASE, this, "M3");
    bothCalibrationE2 = new MeterCalibration(setting.value("errorTestHighCurrentPort").toString(), MeterCalibration::CALIBRATE_NEUTRAL, this, "M4");

    setAllTestResult(TEST_RESULT_IDEAL);

    connect(powerSource, SIGNAL(allChannelReady()), this, SLOT(powerStablised()));

    connect(phaseCalibration, SIGNAL(calibrationSuccess()), this, SLOT(phaseCalibrationPass()));
    connect(phaseCalibration, SIGNAL(calibrationFail()), this, SLOT(phaseCalibrationFail()));

    connect(neutralCalibration, SIGNAL(calibrationSuccess()), this, SLOT(neutralCalibrationPass()));
    connect(neutralCalibration, SIGNAL(calibrationFail()), this, SLOT(neutralCalibrationFail()));

    connect(bothCalibrationE1, SIGNAL(calibrationSuccess()), this, SLOT(bothCalibrationE1Pass()));
    connect(bothCalibrationE1, SIGNAL(calibrationFail()), this, SLOT(bothCalibrationE1Fail()));

    connect(bothCalibrationE2, SIGNAL(calibrationSuccess()), this, SLOT(bothCalibrationE2Pass()));
    connect(bothCalibrationE2, SIGNAL(calibrationFail()), this, SLOT(bothCalibrationE2Fail()));
}

void CalibrationAutoTest::startTest()
{
    qDebug() << "*********************** CalibrationAutoTest::startTest ************************";
    setAllTestResult(TEST_RESULT_RUNNING);
    powerSource->setChannelState(PowerSource::POWER_CHANNEL_1 | PowerSource::POWER_CHANNEL_2 | PowerSource::POWER_CHANNEL_3, true);
}

void CalibrationAutoTest::restartTest()
{
    powerSource->disablePowerOutput();
    QTimer::singleShot(6000, this, SLOT(startTest()));
}

void CalibrationAutoTest::startTest(int testsToStart)
{

}

void CalibrationAutoTest::powerStablised()
{
    qDebug() << "powerStablised";
    phaseCalibration->calibrate();
    neutralCalibration->calibrate();
    bothCalibrationE1->calibrate();
    bothCalibrationE2->calibrate();
}

void CalibrationAutoTest::phaseCalibrationPass()
{
    qDebug() << "phaseCalibrationPass";
    phaseCalibrationResult = TEST_RESULT_PASS;
    processTestResult();
}

void CalibrationAutoTest::phaseCalibrationFail()
{
    qDebug() << "phaseCalibrationFail";
    phaseCalibrationResult = TEST_RESULT_FAIL;
    processTestResult();
}

void CalibrationAutoTest::neutralCalibrationPass()
{
    qDebug() << "neutralCalibrationPass";
    neutralCalibrationResult = TEST_RESULT_PASS;
    processTestResult();
}

void CalibrationAutoTest::neutralCalibrationFail()
{
    qDebug() << "neutralCalibrationFail";
    neutralCalibrationResult = TEST_RESULT_FAIL;
    processTestResult();
}

void CalibrationAutoTest::bothCalibrationE1Pass()
{
    qDebug() << "bothCalibrationE1Pass";
    bothCurrentE1TestResult = TEST_RESULT_PASS;
    processTestResult();
}

void CalibrationAutoTest::bothCalibrationE1Fail()
{
    qDebug() << "bothCalibrationE1Fail";
    bothCurrentE1TestResult = TEST_RESULT_FAIL;
    processTestResult();
}

void CalibrationAutoTest::bothCalibrationE2Pass()
{
    qDebug() << "bothCalibrationE2Pass";
    bothCurrentE2TestResult = TEST_RESULT_PASS;
    processTestResult();
}

void CalibrationAutoTest::bothCalibrationE2Fail()
{
    qDebug() << "bothCalibrationE2Fail";
    bothCurrentE2TestResult = TEST_RESULT_FAIL;
    processTestResult();
}

void CalibrationAutoTest::processTestResult()
{
    TestResults results;
    results << phaseCalibrationResult;
    results << neutralCalibrationResult;
    results << bothCurrentE1TestResult;
    results << bothCurrentE2TestResult;
    qDebug() << results;

    if(phaseCalibrationResult == TEST_RESULT_RUNNING || neutralCalibrationResult == TEST_RESULT_RUNNING
            || bothCurrentE1TestResult == TEST_RESULT_RUNNING || bothCurrentE2TestResult == TEST_RESULT_RUNNING)
        return;

    int passCount = 0;
    foreach(auto result, results){
        if(result == TEST_RESULT_PASS)
            passCount++;
    }

    qDebug() << "**************** processTestResult ***********************" << results << ((passCount == 4) ? "AllPass": "somefail") << passCount;
    restartTest();
//    emit calibrationAutoTestComplete(results);
}

void CalibrationAutoTest::setAllTestResult(TEST_RESULT result)
{
    phaseCalibrationResult = result;
    neutralCalibrationResult = result;
    bothCurrentE1TestResult = result;
    bothCurrentE2TestResult = result;
}
