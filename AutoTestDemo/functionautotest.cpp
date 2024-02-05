#include "functionautotest.h"

FunctionAutoTest::FunctionAutoTest(QString portName, int N, QObject *parent)
    : QObject{parent}
{
    passCounter = 0;
    failCounter = 0;
    testCounter = 0;
    PORTNAME = portName;
    MAXTESTS = N;


    TESTNAME = "Function";
    tag = "AUTO :: FS :";
    functionTest = new FunctionTest(PORTNAME);
    connect(functionTest, SIGNAL(functionTestResult(FunctionTestResult*)), this, SLOT(functionTestResult(FunctionTestResult*)));


    connect(this, SIGNAL(testComplete()), this, SLOT(unitTest()));
    connect(this, SIGNAL(unitTestUpdate(QString)), parent, SLOT(unitTestUpdateDisplay(QString)));

    qDebug() << tag << "PortName" << PORTNAME << ":: Cycles" << MAXTESTS << "::" << TESTNAME << "Test Started..";
    emit unitTestUpdate(QStringLiteral("%1 Unit Test Started:: Device Port %2 :: Cycles %3").arg(TESTNAME).arg(PORTNAME).arg(MAXTESTS));
}

void FunctionAutoTest::unitTest()
{
    if (MAXTESTS>testCounter){
        functionTest->startTest();
    } else {
        functionTest->closeSerialPort();

        qDebug() << tag << "Unit Test Result :: " << passCounter << "/" << MAXTESTS << "passed.";

        emit unitTestUpdate(QStringLiteral("%1 Unit Test Result :: %2 / %3 passed").arg(TESTNAME).arg(passCounter).arg(testCounter));
    }
}

void FunctionAutoTest::functionTestResult(FunctionTestResult *result)
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
