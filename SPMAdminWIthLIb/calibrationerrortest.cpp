#include "calibrationerrortest.h"
#include <QSettings>
#include <QDateTime>
#include <QDebug>

CalibrationErrorTest::CalibrationErrorTest(QObject *parent)
    : QObject{parent}
{
    QSettings setting;
    tag = "CET";

    powerSource = new PowerSource(setting.value("powerSourcePort").toString(), QString(), this);
    phaseCalibration = new MeterCalibration(setting.value("phaseCalibrationPort").toString(), MeterCalibration::CALIBRATE_PHASE, this, "CP");
    neutralCalibration = new MeterCalibration(setting.value("neutralCalibrationPort").toString(), MeterCalibration::CALIBRATE_NEUTRAL, this, "CN");
    lowCurrentErrorTest = new MeterErrorTest(240, 1, 0.5, setting.value("errorTestLowCurrentPort").toString(), this, "EL");
    highCurrentErrorTest = new MeterErrorTest(240, 60, 0.5, setting.value("errorTestHighCurrentPort").toString(), this, "EH");

    powerSource->disablePowerOutput();

    setAllTestResult(TEST_RESULT_IDEAL);

    connect(powerSource, SIGNAL(allChannelReady()), this, SLOT(powerStablised()));

    connect(phaseCalibration, SIGNAL(calibrationSuccess()), this, SLOT(phaseCalibrationPass()));
    connect(phaseCalibration, SIGNAL(calibrationFail()), this, SLOT(phaseCalibrationFail()));

    connect(neutralCalibration, SIGNAL(calibrationSuccess()), this, SLOT(neutralCalibrationPass()));
    connect(neutralCalibration, SIGNAL(calibrationFail()), this, SLOT(neutralCalibrationFail()));

    connect(lowCurrentErrorTest, SIGNAL(errorTestPass()), this, SLOT(lowCurrentTestPass()));
    connect(lowCurrentErrorTest, SIGNAL(errorTestFail()), this, SLOT(lowCurrentTestFail()));

    connect(highCurrentErrorTest, SIGNAL(errorTestPass()), this, SLOT(highCurrentTestPass()));
    connect(highCurrentErrorTest, SIGNAL(errorTestFail()), this, SLOT(highCUrrentTestFail()));

    testsToStart = 0;
}

void CalibrationErrorTest::startTest()
{
    startTest(PHASE_CALIBRATION_TEST | NEUTRAL_CALIBRATION_TEST | LOW_CURRENT_TEST | HIGH_CURRENT_TEST);
}

void CalibrationErrorTest::startTest(int testsToStart)
{
    this->testsToStart = testsToStart;
    setAllTestResult(TEST_RESULT_RUNNING);

    powerSource->setChannelState(PowerSource::POWER_CHANNEL_1 | PowerSource::POWER_CHANNEL_2 | PowerSource::POWER_CHANNEL_3, true);
    startTestTime = QDateTime::currentDateTime();
    qDebug() << tag << startTestTime;
}

void CalibrationErrorTest::powerStablised()
{
    qDebug() << tag << "powerStablised";
    if(testsToStart & PHASE_CALIBRATION_TEST)
        phaseCalibration->calibrate();

    if(testsToStart & NEUTRAL_CALIBRATION_TEST)
        neutralCalibration->calibrate();

    if(testsToStart & LOW_CURRENT_TEST)
        lowCurrentErrorTest->startErrorTest();

    if(testsToStart & HIGH_CURRENT_TEST)
        highCurrentErrorTest->startErrorTest();
}

void CalibrationErrorTest::phaseCalibrationPass()
{
    phaseCalibrationResult = TEST_RESULT_PASS;
    qDebug() << tag << "CalibrationErrorTest:phaseCalibrationPass";
    qDebug() << tag << QDateTime::currentDateTime().msecsTo(startTestTime);
    processTestResult();
}

void CalibrationErrorTest::phaseCalibrationFail()
{
    phaseCalibrationResult = TEST_RESULT_FAIL;
    qDebug() << tag << "CalibrationErrorTest:phaseCalibrationFail";
    processTestResult();
}

void CalibrationErrorTest::neutralCalibrationPass()
{
    neutralCalibrationResult = TEST_RESULT_PASS;
    qDebug() << tag << "CalibrationErrorTest:neutralCalibrationPass";
    qDebug() << tag << QDateTime::currentDateTime().msecsTo(startTestTime);
    processTestResult();
}

void CalibrationErrorTest::neutralCalibrationFail()
{
    neutralCalibrationResult = TEST_RESULT_FAIL;
    qDebug() << tag << "CalibrationErrorTest:neutralCalibrationFail";
    processTestResult();
}

void CalibrationErrorTest::lowCurrentTestPass()
{
    lowCurrentErrorTestResult = TEST_RESULT_PASS;
    processTestResult();
}

void CalibrationErrorTest::lowCurrentTestFail()
{
    lowCurrentErrorTestResult = TEST_RESULT_FAIL;
    processTestResult();
}

void CalibrationErrorTest::highCurrentTestPass()
{
    highCurrentErrorTestResult = TEST_RESULT_PASS;
    processTestResult();
}

void CalibrationErrorTest::highCUrrentTestFail()
{
    highCurrentErrorTestResult = TEST_RESULT_FAIL;
    processTestResult();
}

void CalibrationErrorTest::processTestResult()
{
    TestResults results;
    results << phaseCalibrationResult;
    results << neutralCalibrationResult;
    results << lowCurrentErrorTestResult;
    results << highCurrentErrorTestResult;
    qDebug() << tag << results;

    if((phaseCalibrationResult == TEST_RESULT_RUNNING && phaseCalibrationResult != TEST_RESULT_NOT_APPLICABLE)
            || neutralCalibrationResult == TEST_RESULT_RUNNING
            || lowCurrentErrorTestResult == TEST_RESULT_RUNNING || highCurrentErrorTestResult == TEST_RESULT_RUNNING)
        return;

    int passCount = 0;
    foreach(auto result, results){
        if(result == TEST_RESULT_PASS)
            passCount++;
    }
    calibrationErrorTestResult(results);

    qDebug() << tag << "**************** processTestResult ***********************" << results << ((passCount == 4) ? "AllPass": "somefail") << passCount;
}

void CalibrationErrorTest::setAllTestResult(TEST_RESULT result)
{
    phaseCalibrationResult = result;
    neutralCalibrationResult = result;
    lowCurrentErrorTestResult = result;
    highCurrentErrorTestResult = result;
}
