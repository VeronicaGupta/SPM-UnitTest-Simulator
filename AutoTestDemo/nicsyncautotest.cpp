#include "nicsyncautotest.h"

NicsyncAutoTest::NicsyncAutoTest(QString portName, int N, QString serial, int customer, QObject *parent)
    : QObject{parent}
{
    passCounter = 0;
    failCounter = 0;
    testCounter = 0;
    PORTNAME = portName;
    MAXTESTS = N;


    TESTNAME = "NicSync";
    tag = "AUTO :: NS :";
    Serial = serial;
    Customer = (EnCustomer)customer;
    nicSyncTest = new NicSyncTest(PORTNAME, Customer, false);
    connect(nicSyncTest, SIGNAL(nicSycnTestResult(NicSyncTestResult*)), this, SLOT(nicSyncTestResult(NicSyncTestResult*)));


    connect(this, SIGNAL(testComplete()), this, SLOT(unitTest()));
    connect(this, SIGNAL(unitTestUpdate(QString)), parent, SLOT(unitTestUpdateDisplay(QString)));

    qDebug() << tag << "PortName" << PORTNAME << ":: Cycles" << MAXTESTS << "::" << TESTNAME << "Test Started..";
    emit unitTestUpdate(QStringLiteral("%1 Unit Test Started:: Device Port %2 :: Cycles %3").arg(TESTNAME).arg(PORTNAME).arg(MAXTESTS));
}

void NicsyncAutoTest :: unitTest()
{
    if (MAXTESTS>testCounter){
        nicSyncTest->setSerialToVerify(Serial);
        nicSyncTest->setQRToVerify(Serial, QDateTime::currentDateTime().toString());
        nicSyncTest->startTest(Serial);
    } else {
        nicSyncTest->closeSerialPort();

        qDebug() << tag << "Unit Test Result :: " << passCounter << "/" << MAXTESTS << "passed.";

        emit unitTestUpdate(QStringLiteral("%1 Unit Test Result :: %2 / %3 passed").arg(TESTNAME).arg(passCounter).arg(testCounter));
    }
}

void NicsyncAutoTest::nicSyncTestResult(NicSyncTestResult *result)
{
    if(result->isTestPass){
        passCounter++;
    }
    else{
        failCounter++;
    }

    qDebug() << tag << "Complete" << ++testCounter;

    emit unitTestUpdate(QStringLiteral("%1 Unit Test Cycles :: %2 completed").arg(TESTNAME).arg(testCounter));

    emit testComplete();
}

