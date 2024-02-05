#include "nicsyncautotest.h"
#include "QDebug"

NicSyncAutoTest::NicSyncAutoTest(QString portName, QObject *parent) : QObject(parent)
{
    passCounter = 0;
    failCounter = 0;
    nicSync = new NicSyncTest(portName, EnCustomer::CUST_JAMMU, this);

    connect(nicSync, SIGNAL(nicSyncTestResult(NicSyncTestResult*)), this, SLOT(nicSyncTestResult(NicSyncTestResult* )));

}

void NicSyncAutoTest::starTest()
{
    nicSync->startTest("");
    time = QDateTime::currentDateTime();
}

void NicSyncAutoTest::nicSyncTestResult(NicSyncTestResult *result)
{
    if(result->isTestPass)
        passCounter++;
    else
        failCounter++;

    qDebug()  << "*************************************** nicSyncTestResult" << result->isTestPass << passCounter << failCounter << time.msecsTo(QDateTime::currentDateTime()) / 1000 << "**********************";
    starTest();
}

