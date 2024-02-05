#include "functionautotest.h"
#include <QDebug>

FunctionAutoTest::FunctionAutoTest(QString portName, QObject *parent)
    : QObject{parent}
{
    functionTest = new FunctionTest(portName, this);
    passCounter = 0;
    failCounter = 0;
    connect(functionTest, SIGNAL(functionTestResult(FunctionTestResult*)), this, SLOT(functionTestResult(FunctionTestResult*)));
}

void FunctionAutoTest::startTest()
{
    functionTest->startTest();
    time = QDateTime::currentDateTime();
}

void FunctionAutoTest::restartTest()
{

}

void FunctionAutoTest::functionTestResult(FunctionTestResult *result)
{
    if(result->isTestPass)
        passCounter++;
    else
        failCounter++;

    qDebug()  << "*************************************** functionTestResult" << result->isTestPass << passCounter << failCounter << time.msecsTo(QDateTime::currentDateTime()) / 1000 << "**********************";
    startTest();
}
