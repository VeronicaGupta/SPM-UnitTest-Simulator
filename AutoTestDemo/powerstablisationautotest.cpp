#include "powerstablisationautotest.h"

PowerStablisationAutoTest::PowerStablisationAutoTest(QString psPortName, int N, QString cmriPortName, QStringList voltages, QStringList currents, QObject *parent) : QObject{parent}
{
    passCounter = 0;
    failCounter = 0;
    testCounter = 0;
    PORTNAME = psPortName;
    MAXTESTS = N;
    this->voltages = voltages;
    this->currents = currents;


    TESTNAME = "PowerStablise";
    tag = "AUTO :: PS :";

    testStartTime = QDateTime::currentDateTime();

    powerSource = new PowerSource(psPortName, cmriPortName, this);
    connect(powerSource, SIGNAL(allChannelReady()), this, SLOT(StablisationTestResult()));

    connect(this, SIGNAL(testComplete()), this, SLOT(unitTest()));
    connect(this, SIGNAL(unitTestUpdate(QString)), parent, SLOT(unitTestUpdateDisplay(QString)));

    qDebug() << tag << "PortName" << PORTNAME << ":: Cycles" << MAXTESTS << "::" << TESTNAME << "Test Started..";
    emit unitTestUpdate(QStringLiteral("%1 Unit Test Started:: Device Port %2 :: Cycles %3").arg(TESTNAME).arg(PORTNAME).arg(MAXTESTS));
}

void PowerStablisationAutoTest::unitTest()
{
    if (MAXTESTS>testCounter){

        powerSource->disablePowerOutput();
        powerSource->setVoltage(voltages[0].toFloat(), voltages[1].toFloat(), voltages[2].toFloat());
        powerSource->setCurrent(currents[0].toFloat(), currents[1].toFloat(), currents[2].toFloat());
        powerSource->setChannelState(PowerSource::POWER_CHANNEL_1 | PowerSource::POWER_CHANNEL_2 | PowerSource::POWER_CHANNEL_3, true);

        testStartTime = QDateTime::currentDateTime();

        qDebug() << tag << "Test Complete" << testCounter << "in" << testStartTime;

    } else {
//        powerSource->disablePowerOutput();
//        powerSource->cmriControl->cmriReset();
        qDebug() << tag << "Unit Test Result :: " << passCounter << "/" << MAXTESTS << "passed.";

        emit unitTestUpdate(QStringLiteral("%1 Unit Test Result :: %2 / %3 passed").arg(TESTNAME).arg(passCounter).arg(testCounter));
    }
}

void PowerStablisationAutoTest::StablisationTestResult()
{
    sourceStabliseDuration = testStartTime.secsTo(QDateTime::currentDateTime());

    passCounter++;

    qDebug() << tag << "Complete" << ++testCounter;

    emit unitTestUpdate(QStringLiteral("%1 Unit Test Cycles :: %2 completed").arg(TESTNAME).arg(testCounter));

    emit testComplete();
}
