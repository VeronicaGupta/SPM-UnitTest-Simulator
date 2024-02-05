#include "errorautotest.h"
#include <QSettings>
#include <QDebug>

ErrorAutoTest::ErrorAutoTest(QObject *parent)
    : QObject{parent}
{
    QSettings setting;
    passCounter = 0;
    failCounter = 0;
    powerSource = new PowerSource(setting.value("powerSourcePort").toString(), setting.value("cmriSourcePort").toString(), this);
    powerSource->disablePowerOutput();

    connect(powerSource, SIGNAL(allChannelReady()), this, SLOT(powerStablised()));

    lowCurrentErrorTest = new MeterErrorTest(240, 1, 0.5, setting.value("errorTestLowCurrentPort").toString(), this, "E1L");
    highCurrentErrorTest = new MeterErrorTest(240, 60, 0.5, setting.value("errorTestHighCurrentPort").toString(), this, "E2H");

    connect(lowCurrentErrorTest, SIGNAL(errorTestResult(ErrorTestResult*)), this, SLOT(lowCurrentTestResult(ErrorTestResult*)));
    connect(highCurrentErrorTest, SIGNAL(errorTestResult(ErrorTestResult*)), this, SLOT(highCurrentTestResult(ErrorTestResult*)));
}

void ErrorAutoTest::startTest()
{
    qDebug() << "*********************** ErrorAutoTest::startTest ************************";
    lowCurrentErrorTestResult = TEST_RESULT_RUNNING;
    highCurrentErrorTestResult = TEST_RESULT_RUNNING;
    powerSource->setChannelState(PowerSource::POWER_CHANNEL_1 | PowerSource::POWER_CHANNEL_2 | PowerSource::POWER_CHANNEL_3, true);
    startTime = QDateTime::currentDateTime();
}

void ErrorAutoTest::restartTest()
{
    powerSource->disablePowerOutput();
    QTimer::singleShot(6000, this, SLOT(startTest()));
}

void ErrorAutoTest::powerStablised()
{
    qDebug() << "PowerStablised";
    lowCurrentErrorTest->startErrorTest();
    highCurrentErrorTest->startErrorTest();
}

void ErrorAutoTest::processTestResult()
{
    TestResults results;
    results << lowCurrentErrorTestResult;
    results << highCurrentErrorTestResult;

    qDebug() << results;

    if(lowCurrentErrorTestResult == TEST_RESULT_RUNNING || highCurrentErrorTestResult == TEST_RESULT_RUNNING)
        return;

    int passCount = 0;
    foreach(auto result, results){
        if(result == TEST_RESULT_PASS)
            passCount++;
    }

    if(passCount == 2)
        passCounter++;
    else
        failCounter++;

    qDebug() << "**************** Stop Test - processTestResult ***********************" << results
             << ((passCount == 2) ? "AllPass": "somefail") << passCount << "pass" << passCounter << "fail" << failCounter << startTime.msecsTo(QDateTime::currentDateTime());

    restartTest();
}

void ErrorAutoTest::lowCurrentTestResult(ErrorTestResult *result)
{
    lowCurrentErrorTestResult = result->isTestPass ? TEST_RESULT_PASS : TEST_RESULT_FAIL;
    qDebug() << "lowCurrentErrorTest : " << result->isTestPass;
    processTestResult();
}

void ErrorAutoTest::highCurrentTestResult(ErrorTestResult *result)
{
    highCurrentErrorTestResult = result->isTestPass ? TEST_RESULT_PASS : TEST_RESULT_FAIL;
    qDebug() << "highCurrentErrorTest : " << result->isTestPass;
    processTestResult();
}
